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

bool BVH::checkHit(const Ray &ray, double &t, Shape *&hitShape) {
    return root.checkHit(ray, t, std::numeric_limits<double>::max(), hitShape);
}

bool BVH::checkShadowHit(const Ray &ray, double maxT) {
    return root.checkShadowHit(ray, maxT);
}

///////////////////////////////////////////BVH_NODE TERRITORY///////////////////////////////////////////////////

BVH_Node::BVH_Node() {

}

BVH_Node::~BVH_Node() {
    BBoxes.clear();
}

// shadows require less work than normal hit checking
bool BVH_Node::checkShadowHit(const Ray &ray, double maxT) {
    double checkingT = 0;
    Shape *testShape;

    if (myBounds.CalculateHit(ray, checkingT, maxT, testShape)) {
        for (unsigned int i = 0; i < BBoxes.size(); ++i)
        {
            if (BBoxes[i].CalculateHit(ray, checkingT, maxT, testShape)) {
                if (checkingT > 0 && (checkingT < maxT)) {
                    return true;
                }
            }
        }

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

    if (myBounds.CalculateHit(ray, checkingT, maxT, testShape)) {
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
    std::vector<Vector3f> axisMaxs;
    axisMaxs.resize(size);
    
    Vector3f mins = Vector3f(0,0,0);
    Vector3f maxs = Vector3f(0,0,0);

    if (size > 0)
    {
        mins = BBoxes[0].getMins();
        maxs = BBoxes[0].getMaxs();

        axisMaxs[0] = maxs;
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

        axisMaxs[i] = holdMax;
    }

    // generate bounding box
    myBounds = BoundingBox(mins, maxs, NULL);

    // split contents into two groups if enough
    if (size > 2) {
        // find the axis to split on

        // make two children

        // set the children's contents

        // call split on children
    }
}
