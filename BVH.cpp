#include "BVH.hpp"

#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

BVH::BVH() {

}

BVH::BVH(std::vector<Sphere> spheres, std::vector<Triangle> tris, std::vector<Box> boxes) {
    unsigned int curPos = 0;
    unsigned int total = spheres.size() + tris.size() + boxes.size();
    BBoxes.resize(total);
    root.contents.resize(total);

    for (unsigned int i = 0; i < spheres.size(); ++i)
    {
        BBoxes[curPos] = EncaseSphere(&spheres[i]);
        root.contents[curPos] = &BBoxes[curPos];
        curPos++;
    }

    for (unsigned int i = 0; i < tris.size(); ++i)
    {
        BBoxes[curPos] = EncaseTriangle(&tris[i]);
        root.contents[curPos] = &BBoxes[curPos];
        curPos++;
    }

    for (unsigned int i = 0; i < boxes.size(); ++i)
    {
        BBoxes[curPos] = EncaseBox(&boxes[i]);
        root.contents[curPos] = &BBoxes[curPos];
        curPos++;
    }
}

BVH::~BVH() {
    BBoxes.clear();
}

void BVH::Init() {
    root.split();
}

void BVH::AddShape(std::vector<Sphere> shapes) {
    unsigned int curPos = BBoxes.size();
    unsigned int rootPos = root.contents.size();

    BBoxes.resize(curPos + shapes.size());
    root.contents.resize(rootPos + shapes.size());

    for (unsigned int i = 0; i < shapes.size(); ++i)
    {
        BBoxes[curPos] = EncaseSphere(&shapes[i]);
        root.contents[rootPos++] = &BBoxes[curPos++];
    }
}

void BVH::AddShape(std::vector<Triangle> shapes) {
    unsigned int curPos = BBoxes.size();
    unsigned int rootPos = root.contents.size();

    BBoxes.resize(curPos + shapes.size());
    root.contents.resize(rootPos + shapes.size());

    for (unsigned int i = 0; i < shapes.size(); ++i)
    {
        BBoxes[curPos] = EncaseTriangle(&shapes[i]);
        root.contents[rootPos++] = &BBoxes[curPos++];
    }
}

void BVH::AddShape(std::vector<Box> shapes) {
    unsigned int curPos = BBoxes.size();
    unsigned int rootPos = root.contents.size();

    BBoxes.resize(curPos + shapes.size());
    root.contents.resize(rootPos + shapes.size());

    for (unsigned int i = 0; i < shapes.size(); ++i)
    {
        BBoxes[curPos] = EncaseBox(&shapes[i]);
        root.contents[rootPos++] = &BBoxes[curPos++];
    }
}

bool BVH::checkHit(const Ray &ray, double &t, Shape *&hitShape) {
    return root.checkHit(ray, t, std::numeric_limits<double>::max(), hitShape);
}

// bool BVH::checkShadowHit(const Ray &ray, double maxT) {
//     return root.checkShadowHit(ray, maxT);
// }

///////////////////////////////////////////BVH_NODE TERRITORY///////////////////////////////////////////////////

BVH_Node::BVH_Node() {

}

BVH_Node::~BVH_Node() {

}

bool BVH_Node::checkHit(const Ray &ray, double &t, double maxT, Shape *&hitShape) {
    double checkingT = 0;
    bool hit = false;
    Shape *testShape;

    if (myBounds.CalculateHit(ray, checkingT, maxT, testShape)) {
        for (unsigned int i = 0; i < contents.size(); ++i)
        {
            if (contents[i]->CalculateHit(ray, checkingT, maxT, testShape)) {
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
            if (children[i].checkHit(ray, t, t, hitShape))
                hit = true;
        }
    }
    
    return hit;
}

void BVH_Node::split() {
    // generate a list of mins and maxes, and bounding box coordinates
    unsigned int size = contents.size();
    std::vector<Vector3f> axisMaxs;
    axisMaxs.resize(size);
    
    Vector3f mins = Vector3f(0,0,0);
    Vector3f maxs = Vector3f(0,0,0);

    if (size > 0)
    {
        mins = contents[0]->mins;
        maxs = contents[0]->maxs;

        axisMaxs[0] = maxs;
    }

    for (unsigned int i = 1; i < size; ++i)
    {
        Vector3f holdMin = contents[i]->mins;
        Vector3f holdMax = contents[i]->maxs;

        for (int j = 0; j < 3; ++j)
        {
            mins(j) = min(mins(j), holdMin(j));
            maxs(j) = min(maxs(j), holdMax(j));
        }

        axisMaxs[i] = holdMax;
    }

    // generate bounding box
    myBounds = BoundingBox(mins, maxs);

    // split contents into two groups if enough
    if (size > 2) {
        // find the axis to split on

        // make two children

        // set the children's contents

        // call split on children
    }
}

// bool BVH_Node::checkShadowHit(const Ray &ray, double maxT) {
//     double checkingT = 0;
//     Shape *testShape;

//     if (myBounds.CalculateHit(ray, checkingT, maxT, testShape)) {
//         for (unsigned int i = 0; i < contents.size(); ++i)
//         {
//             if (contents[i]->CalculateHit(ray, checkingT, maxT, testShape)) {
//                 if (checkingT > 0 && (checkingT < maxT)) {
//                     return true;
//                 }
//             }
//         }

//         for (unsigned int i = 0; i < children.size(); ++i)
//         {
//             if (children[i].checkShadowHit(ray, maxT))
//                 return true;
//         }
//     }
    
//     return false;
// }
