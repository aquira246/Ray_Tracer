#pragma  once
#ifndef __BRDF__
#define __BRDF__

#include <stdio.h>
#include <Eigen/Dense>

#include "Shape.hpp"

Eigen::Vector3f Diffuse(const Shape *hitShape, const Eigen::Vector3f &n, const Eigen::Vector3f &l, 
                                    const Eigen::Vector3f &lightCol);

Eigen::Vector3f BlinnSpec(const Shape *hitShape, const Eigen::Vector3f &n, const Eigen::Vector3f &l, 
                                    const Eigen::Vector3f &d, const Eigen::Vector3f &lightCol);

Eigen::Vector3f BlinnPhong(const Shape *hitShape, const  Eigen::Vector3f &n, const Eigen::Vector3f &l, 
                            const Eigen::Vector3f &d, const Eigen::Vector3f &lightCol);

Eigen::Vector3f CookTorrance(const Shape *hitShape, const Eigen::Vector3f &n, const Eigen::Vector3f &l, 
                            const Eigen::Vector3f &d, const Eigen::Vector3f &lightCol, 
                            double curIOR, double newIOR);

Eigen::Vector3f ToonSorta(const Shape *hitShape, const Eigen::Vector3f &n, const Eigen::Vector3f &l, 
                        const Eigen::Vector3f &d, const Eigen::Vector3f &lightCol, Eigen::Vector3f *retColor);

#endif