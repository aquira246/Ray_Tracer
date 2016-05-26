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

Ray ComputeCameraRay(int i, int j, int width, int height, const Camera &cam);
void ComputeCameraRay_AntiAliasing(int i, int j, int width, int height, const Camera &cam, std::vector<Ray> &rays);
Ray ComputeReflectionRay(const Eigen::Vector3f &hitPt, const Eigen::Vector3f &surfaceNormal, const Eigen::Vector3f &rayDirection);
Ray ComputeRefractedRay(const Eigen::Vector3f &hitPt, const Eigen::Vector3f &surfaceNormal, const Eigen::Vector3f &rayDirection, 
                        double ior1, double ior2, bool *totalReflection);
Eigen::Vector3f ComputeGIRay(const Eigen::Vector3f &N);

#endif