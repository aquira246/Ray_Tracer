#pragma  once
#ifndef __B_BOX__
#define __B_BOX__

#include <Eigen/Dense>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream> 

#include "Shape.hpp"
#include "Triangle.hpp"
#include "Sphere.hpp"
#include "Box.hpp"
#include "Ray.hpp"

class BoundingBox
{
    public:
        BoundingBox();
        BoundingBox(Eigen::Vector3f c1, Eigen::Vector3f c2, Shape *c);
        ~BoundingBox();

        Shape *contents;

        bool CalculateHit(const Ray &ray, double &t, double maxT, Shape *&hitShape);

        Eigen::Vector3f getMins();
        Eigen::Vector3f getMaxs();
        void setMins(Eigen::Vector3f newMins);
        void setMaxs(Eigen::Vector3f newMaxs);

    private:
        Eigen::Vector3f mins;
        Eigen::Vector3f maxs;
};

BoundingBox EncaseTriangle(Triangle *toEncase);
BoundingBox EncaseSphere(Sphere *toEncase);
BoundingBox EncaseBox(Box *toEncase);

#endif
