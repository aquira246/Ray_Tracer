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

      bool CalculateHit(Ray ray, double &t);
      virtual Eigen::Vector3f GetNormal(Eigen::Vector3f hitPt);

   private:
};

#endif
