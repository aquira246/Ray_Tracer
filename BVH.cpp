#include "BVH.hpp"

#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

BVH::BVH() {

}

BVH::BVH(std::vector<Shape *> &shapes) {
    unsigned int curPos = 0;
    unsigned int total = shapes.size();
    root.BBoxes.reserve(total);

    for (unsigned int i = 0; i < total; ++i)
    {
        if (shapes[i]->GetShape() == SPHERE_ID) {
            root.BBoxes.push_back(EncaseSphere((Sphere *)shapes[i]));
            curPos++;
        } else if (shapes[i]->GetShape() == TRIANGLE_ID) {
            root.BBoxes.push_back(EncaseTriangle((Triangle *)shapes[i]));
            curPos++;
        } else if (shapes[i]->GetShape() == BOX_ID) {
            root.BBoxes.push_back(EncaseBox((Box *)shapes[i]));
            curPos++;
        }
    }
}

BVH::~BVH() {
}

void BVH::Init() {
    root.split();
}

void BVH::AddShape(std::vector<Sphere> shapes) {
    unsigned int curPos = root.BBoxes.size();

    root.BBoxes.resize(curPos + shapes.size());

    for (unsigned int i = 0; i < shapes.size(); ++i)
    {
        root.BBoxes[curPos] = EncaseSphere(&shapes[i]);
    }
}

void BVH::AddShape(std::vector<Triangle> shapes) {
    unsigned int curPos = root.BBoxes.size();

    root.BBoxes.resize(curPos + shapes.size());

    for (unsigned int i = 0; i < shapes.size(); ++i)
    {
        root.BBoxes[curPos] = EncaseTriangle(&shapes[i]);
    }
}

void BVH::AddShape(std::vector<Box> shapes) {
    unsigned int curPos = root.BBoxes.size();

    root.BBoxes.resize(curPos + shapes.size());

    for (unsigned int i = 0; i < shapes.size(); ++i)
    {
        root.BBoxes[curPos] = EncaseBox(&shapes[i]);
    }
}

// goes through the BVH and checks for hits
bool BVH::checkHit(const Ray &ray, double &t, Shape *&hitShape) {
    return root.checkHit(ray, t, std::numeric_limits<double>::max(), hitShape);
}

// shadows require less work
bool BVH::checkShadowHit(const Ray &ray, double maxT) {
    return root.checkShadowHit(ray, maxT);
}

///////////////////////////////////////////BVH_NODE TERRITORY///////////////////////////////////////////////////

void BBoxSwap(BoundingBox &a, BoundingBox &b) {
    BoundingBox hold = a;
    a = b;
    b = hold;
}

// sort's v based on the axis passed in and using the max values. axis must be 0, 1, or 2
// usnig selection sort becuase it's simple
void SortBoundingBoxByMaxAxis(std::vector<BoundingBox> &v, int axis) {
    if (axis > 2 || axis < 0) {
        cout << "ERROR! Axis is wrong size. Axis must be 0, 1, or 2" << endl;
        exit(-1);
    }

    unsigned int size = v.size();
    float smallest, idx, check;
    for (unsigned int i = 0; i < size; ++i)
    {
        smallest = v[i].getMaxs()[axis];
        idx = i;

        for (unsigned int j = i + 1; j < size; ++j)
        {
            check = v[j].getMaxs()[axis];
            if (check < smallest) {
                smallest = check;
                idx = j;
            }
        }

        if (idx != i)
            BBoxSwap(v[i], v[idx]);
    }
}

BVH_Node::BVH_Node() {

}

BVH_Node::~BVH_Node() {
    BBoxes.clear();
}

// shadows require less work than normal hit checking
bool BVH_Node::checkShadowHit(const Ray &ray, double maxT) {
    double checkingT = 0;
    Shape *testShape;

    // check if our bounding box is hit
    if (myBounds.CalculateHit(ray, checkingT, maxT, testShape)) {

        // if so then check if any of the objects inside this Node are hit
        for (unsigned int i = 0; i < BBoxes.size(); ++i)
        {
            if (BBoxes[i].CalculateHit(ray, checkingT, maxT, testShape)) {
                if (checkingT > 0 && (checkingT < maxT)) {
                    return true;
                }
            }
        }

        // then check to see the restults of checking if our children are hit
        for (unsigned int i = 0; i < children.size(); ++i)
        {
            if (children[i].checkShadowHit(ray, maxT))
                return true;
        }
    }
    
    return false;
}

bool BVH_Node::checkHit(const Ray &ray, double &t, double maxT, Shape *&hitShape) {
    double checkingT = 0;
    bool hit = false;
    Shape *testShape;

    // check if our bounding box is hit
    if (myBounds.CalculateHit(ray, checkingT, maxT, testShape)) {

        // if so, check all the objects inside this bounding box for hits
        for (unsigned int i = 0; i < BBoxes.size(); ++i)
        {
            if (BBoxes[i].CalculateHit(ray, checkingT, maxT, testShape)) {
                if (checkingT > 0 && (checkingT < t || !hit)) {
                    t = checkingT;
                    maxT = t;
                    hit = true;
                    hitShape = testShape;
                }
            }
        }

        // then check to see the restults of checking if our children are hit
        for (unsigned int i = 0; i < children.size(); ++i)
        {
            if (children[i].checkHit(ray, checkingT, maxT, testShape)) {
                if (checkingT > 0 && (checkingT < t || !hit)) {
                    t = checkingT;
                    maxT = t;
                    hit = true;
                    hitShape = testShape;
                }
            }
        }
    }
    
    return hit;
}

void BVH_Node::split() {
    // generate a list of mins and maxes, and bounding box coordinates
    unsigned int size = BBoxes.size();
    
    Vector3f mins = Vector3f(0,0,0);
    Vector3f maxs = Vector3f(0,0,0);

    if (size > 0)
    {
        mins = BBoxes[0].getMins();
        maxs = BBoxes[0].getMaxs();
    }

    for (unsigned int i = 1; i < size; ++i)
    {
        Vector3f holdMin = BBoxes[i].getMins();
        Vector3f holdMax = BBoxes[i].getMaxs();

        for (int j = 0; j < 3; ++j)
        {
            mins(j) = min(mins(j), holdMin(j));
            maxs(j) = max(maxs(j), holdMax(j));
        }
    }

    // generate bounding box
    myBounds = BoundingBox(mins, maxs, NULL);

    // split contents into two groups if enough
    if (size > 2) {
        // find the axis to split on
        Vector3f extent =  maxs - mins;

        // determine which axis has the largest extent and divide based on that axis
        if (extent(0) >= extent(1) && extent(0) >= extent(2)) {
            SortBoundingBoxByMaxAxis(BBoxes, 0);
            // cout << "Sorted by 0" << endl; 
        } else if (extent(1) >= extent(2)) {
            SortBoundingBoxByMaxAxis(BBoxes, 1);
            // cout << "Sorted by 1" << endl; 
        } else {
            SortBoundingBoxByMaxAxis(BBoxes, 2);
            // cout << "Sorted by 2" << endl; 
        }

        // check if the sort works
        // for (unsigned int i = 0; i < BBoxes.size(); ++i)
        // {
        //     Vector3f hold = BBoxes[i].getMaxs();
        //     cout << hold[0] << ", " << hold[1] << ", " << hold[2] << endl;
        // }

        // make two children
        BVH_Node leftChild = BVH_Node();
        BVH_Node rightChild = BVH_Node();

        // set the children's BBoxes
        std::vector<BoundingBox> splitLeft(BBoxes.begin(), BBoxes.begin() + size/2);
        std::vector<BoundingBox> splitRight(BBoxes.begin() + size/2, BBoxes.end());

        leftChild.BBoxes = splitLeft;
        rightChild.BBoxes = splitRight;

        // clear our BBoxes
        BBoxes.clear();

        // call split on children
        leftChild.split();
        rightChild.split();

        // put children in vector
        children.push_back(leftChild);
        children.push_back(rightChild);
    }
}
