#pragma  once
#ifndef __BOX__
#define __BOX__

#include <Eigen/Dense>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream> 

#include "Shape.hpp"
#include "Ray.hpp"

class Box: public Shape
{
    public:
        Box();
        Box(Eigen::Vector3f c1, Eigen::Vector3f c2, bool bounding);
        ~Box();

        static void Parse(Box &box);

        bool isBounding;
        std::vector<Shape *> contents;
        Eigen::Vector3f normal;
        Eigen::Vector3f corner1;
        Eigen::Vector3f corner2;
        virtual int GetShape() {
            return BOX_ID;
        }

        bool CalculateHit(const Ray &ray, double &t, Shape *&hitShape, Eigen::Vector3f *hitNormal);
    private:
};

#endif
