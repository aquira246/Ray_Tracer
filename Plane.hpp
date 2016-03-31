#pragma  once
#ifndef __Plane__
#define __Plane__

#include <Eigen/Dense>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream> 

#include "Shape.hpp"
#include "Ray.hpp"

class Plane: public Shape
{
    public:
        Plane();
        Plane(Eigen::Vector3f c, Eigen::Vector3f n, float r);
        ~Plane();

        Eigen::Vector3f normal;
        // Shape has a center and radius, the only components of a Plane

        bool CalculateHit(Ray ray, double &t);

    private:
};

#endif
