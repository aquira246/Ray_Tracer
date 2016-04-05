#pragma  once
#ifndef __RAY__
#define __RAY__

#include <Eigen/Dense>
#include "Camera.hpp"

class Ray
{
    public:
        Ray();
        Ray(Eigen::Vector3f p, Eigen::Vector3f d);
        ~Ray();

        Eigen::Vector3f position;
        Eigen::Vector3f direction;

    private:
};

Ray ComputeCameraRay(int i, int j, int width, int height, Camera cam);

#endif