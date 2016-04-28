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

        Eigen::Vector3f normal;
        Eigen::Vector3f corner1;
        Eigen::Vector3f corner2;
        virtual int GetShape() {
            return BOX_ID;
        }

        bool CalculateHit(Ray ray, double &t);
        virtual Eigen::Vector3f GetNormal(Eigen::Vector3f hitPt);

    private:
};

#endif
