#pragma once

#include "Shape.hpp"
#include "Box.hpp"
#include "Sphere.hpp"
#include "Triangle.hpp"
#include "BoundingBox.h"

class BVH_Node
{
public:
    BVH_Node();
    ~BVH_Node();

    bool checkHit(const Ray &ray, double &t, double maxT, Shape *&hitShape);
    bool checkShadowHit(const Ray &ray, double maxT);
    void splitKD();
    void splitOct();
    
    std::vector<BoundingBox> BBoxes;
    std::vector<BVH_Node> children;
    BoundingBox myBounds;
};

class BVH
{
public:
    BVH();
    BVH(std::vector<Shape *> &shapes);
    ~BVH();

    // 0 is normal BVH that splits in half multiple times
    // defaults to 0 if too big
    void Init(int setup);

    bool checkHit(const Ray &ray, double &t, Shape *&hitShape);
    bool checkShadowHit(const Ray &ray, double maxT);

    BVH_Node root;
};