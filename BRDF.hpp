#pragma  once
#ifndef __BRDF__
#define __BRDF__

#include <stdio.h>
#include <Eigen/Dense>

#include "Shape.hpp"

class BRDF
{
    public:
        Eigen::Vector3f BlinnPhong(Shape *hitShape, const  Eigen::Vector3f &n, 
            const Eigen::Vector3f &l, const Eigen::Vector3f &d, const Eigen::Vector3f &lightCol);
        Eigen::Vector3f CookTorrance(Shape *hitShape, const Eigen::Vector3f &n, 
            const Eigen::Vector3f &l, const Eigen::Vector3f &d, const Eigen::Vector3f &lightCol, 
            double curIOR, double newIOR);
        Eigen::Vector3f ToonSorta(Shape *hitShape, const Eigen::Vector3f &n, 
            const Eigen::Vector3f &l, const Eigen::Vector3f &d, const Eigen::Vector3f &lightCol, 
            Eigen::Vector3f *retColor);
};
#endif