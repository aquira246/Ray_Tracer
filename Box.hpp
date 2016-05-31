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
        Box(Eigen::Vector3f c1, Eigen::Vector3f c2);
        ~Box();

        static void Parse(Box &box);
        void Init();

        Eigen::Vector3f corner1;
        Eigen::Vector3f corner2;

        float xmin, xmax, ymin, ymax, zmin, zmax;

        virtual int GetShape() {
            return BOX_ID;
        }

        void GetNormal(const Ray &ray, Eigen::Vector3f *hitNormal, double t);

        bool CalculateHit(const Ray &ray, double &t, Shape *&hitShape);

        Eigen::Vector3f leftNormal, rightNormal, topNormal, botNormal, frontNormal, backNormal;
};

#endif
