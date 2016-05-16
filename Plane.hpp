#pragma once
#ifndef __PLANE_H__
#define __PLANE_H__

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

      static void Parse(Plane &plane);

      Eigen::Vector3f normal;
      // Shape has a center and radius

      bool CalculateHit(const Ray &ray, double &t, Shape *&hitShape);
      void GetNormal(const Ray &ray, Eigen::Vector3f *hitNormal, double t);

      virtual int GetShape() {
         return PLANE_ID;
      }

   private:
};

#endif
