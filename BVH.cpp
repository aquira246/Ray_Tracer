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

void BVH::Init(int setup) {
    if (setup == 0)
        root.splitKD();
    else if (setup == 1) {
        cout << "WARNING! UNDER CONSTRUCTION!\n";
        root.splitOct();
    }
    else
        root.splitKD();
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

inline void BBoxSwap(BoundingBox &a, BoundingBox &b) {
    BoundingBox hold = a;
    a = b;
    b = hold;
}

// sort's v based on the axis passed in and using the max values. axis must be 0, 1, or 2
// usnig selection sort becuase it's simple
void SortBoundingBoxByAxis(std::vector<BoundingBox> &v, int axis) {
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
            check = v[j].getMaxs()[axis] + v[j].getMins()[axis];
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
                if ((checkingT < maxT) && checkingT > 0) {
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

void BVH_Node::splitKD() {
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
    if (size > 3) {
        // find the axis to split on
        Vector3f extent =  maxs - mins;

        // determine which axis has the largest extent and divide based on that axis
        if (extent(0) >= extent(1) && extent(0) >= extent(2)) {
            SortBoundingBoxByAxis(BBoxes, 0);
            // cout << "Sorted by 0" << endl; 
        } else if (extent(1) >= extent(2)) {
            SortBoundingBoxByAxis(BBoxes, 1);
            // cout << "Sorted by 1" << endl; 
        } else {
            SortBoundingBoxByAxis(BBoxes, 2);
            // cout << "Sorted by 2" << endl; 
        }

        // make two children
        BVH_Node leftChild = BVH_Node();
        BVH_Node midChild = BVH_Node();
        BVH_Node rightChild = BVH_Node();

        // set the children's BBoxes
        vector<BoundingBox>::iterator start = BBoxes.begin();
        vector<BoundingBox>::iterator midPoint1 = start + size/3;
        vector<BoundingBox>::iterator midPoint2 = midPoint1 + size/3;
        vector<BoundingBox>::iterator end = BBoxes.end();

        std::vector<BoundingBox> splitLeft(start, midPoint1);
        std::vector<BoundingBox> splitMid(midPoint1, midPoint2);
        std::vector<BoundingBox> splitRight(midPoint2, end);

        leftChild.BBoxes = splitLeft;
        midChild.BBoxes = splitMid;
        rightChild.BBoxes = splitRight;

        // clear our BBoxes
        BBoxes.clear();

        // call split on children
        leftChild.splitKD();
        midChild.splitKD();
        rightChild.splitKD();

        // put children in vector
        children.push_back(leftChild);
        children.push_back(midChild);
        children.push_back(rightChild);
    }
}

void BVH_Node::splitOct() {
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
        std::vector<BoundingBox> topFrontLeft, topFrontRight, topBackLeft, 
                                topBackRight, botFrontLeft, botFrontRight, 
                                botBackLeft, botBackRight, base;

        Vector3f mid = (maxs - mins);
        mid[0] = mid[0]/2 + mins[0];
        mid[1] = mid[1]/2 + mins[1];
        mid[2] = mid[2]/2 + mins[2];

        for (unsigned int i = 1; i < size; ++i)
        {
            Vector3f holdMin = BBoxes[i].getMins();
            Vector3f holdMax = BBoxes[i].getMaxs();
            
            // check if top or bottom
            if (holdMin[1] <= mid[1] && holdMax[1] <= mid[1])
                // check if front or back
                if (holdMin[2] <= mid[2] && holdMax[2] <= mid[2])
                    // check if we are on the left or right
                    if (holdMin[0] <= mid[0] && holdMax[0] <= mid[0])
                        botFrontLeft.push_back(BBoxes[i]);
                    else if(holdMin[0] > mid[0] && holdMax[0] > mid[0])
                        botFrontRight.push_back(BBoxes[i]);
                    else
                        base.push_back(BBoxes[i]);
                else if(holdMin[2] > mid[2] && holdMax[2] > mid[2])
                    // check if we are on the left or right
                    if (holdMin[0] <= mid[0] && holdMax[0] <= mid[0])
                        botBackLeft.push_back(BBoxes[i]);
                    else if(holdMin[0] > mid[0] && holdMax[0] > mid[0])
                        botBackRight.push_back(BBoxes[i]);
                    else
                        base.push_back(BBoxes[i]);
                else
                    base.push_back(BBoxes[i]);
            else if(holdMin[1] > mid[1] && holdMax[1] > mid[1])
                // check if front or back
                if (holdMin[2] <= mid[2] && holdMax[2] <= mid[2])
                    // check if we are on the left or right
                    if (holdMin[0] <= mid[0] && holdMax[0] <= mid[0])
                        topFrontLeft.push_back(BBoxes[i]);
                    else if(holdMin[0] > mid[0] && holdMax[0] > mid[0])
                        topFrontRight.push_back(BBoxes[i]);
                    else
                        base.push_back(BBoxes[i]);
                else if(holdMin[2] > mid[2] && holdMax[2] > mid[2])
                    // check if we are on the left or right
                    if (holdMin[0] <= mid[0] && holdMax[0] <= mid[0])
                        topBackLeft.push_back(BBoxes[i]);
                    else if(holdMin[0] > mid[0] && holdMax[0] > mid[0])
                        topBackRight.push_back(BBoxes[i]);
                    else
                        base.push_back(BBoxes[i]);
                else
                    base.push_back(BBoxes[i]);
            else
                base.push_back(BBoxes[i]);
        }

        BBoxes.clear();
        BBoxes = base;

        if (botFrontLeft.size() > 0) {
            BVH_Node child = BVH_Node();
            child.BBoxes  = botFrontLeft;
            child.splitOct();
            children.push_back(child);
        }

        if (botFrontRight.size() > 0) {
            BVH_Node child = BVH_Node();
            child.BBoxes  = botFrontRight;
            child.splitOct();
            children.push_back(child);
        }

        if (botBackLeft.size() > 0) {
            BVH_Node child = BVH_Node();
            child.BBoxes  = botBackLeft;
            child.splitOct();
            children.push_back(child);
        }

        if (botBackRight.size() > 0) {
            BVH_Node child = BVH_Node();
            child.BBoxes  = botBackRight;
            child.splitOct();
            children.push_back(child);
        }

        if (topFrontLeft.size() > 0) {
            BVH_Node child = BVH_Node();
            child.BBoxes  = topFrontLeft;
            child.splitOct();
            children.push_back(child);
        }

        if (topFrontRight.size() > 0) {
            BVH_Node child = BVH_Node();
            child.BBoxes  = topFrontRight;
            child.splitOct();
            children.push_back(child);
        }

        if (topBackLeft.size() > 0) {
            BVH_Node child = BVH_Node();
            child.BBoxes  = topBackLeft;
            child.splitOct();
            children.push_back(child);
        }

        if (topBackRight.size() > 0) {
            BVH_Node child = BVH_Node();
            child.BBoxes  = topBackRight;
            child.splitOct();
            children.push_back(child);
        }
    }
}
