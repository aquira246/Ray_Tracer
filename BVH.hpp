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
    void split();
    
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

    void Init();

    void AddShape(std::vector<Sphere> shapes);
    void AddShape(std::vector<Triangle> shapes);
    void AddShape(std::vector<Box> shapes);

    bool checkHit(const Ray &ray, double &t, Shape *&hitShape);
    bool checkShadowHit(const Ray &ray, double maxT);

    BVH_Node root;
};