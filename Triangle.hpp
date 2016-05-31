#pragma once
#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include <Eigen/Dense>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream> 

#include "Shape.hpp"
#include "Ray.hpp"

class Triangle: public Shape
{
   public:
      Triangle();
      Triangle(Eigen::Vector3f pta, Eigen::Vector3f ptb, Eigen::Vector3f ptc);
      ~Triangle();
      
      static void Parse(Triangle &triangle);
      bool CalculateHit(const Ray &ray, double &t, Shape *&hitShape);
      virtual int GetShape() {
         return TRIANGLE_ID;
      }
      void GetNormal(const Ray &ray, Eigen::Vector3f *hitNormal, double t);
      void GetTransformedPts(Eigen::Vector3f *ta, Eigen::Vector3f *tb, Eigen::Vector3f *tc);

   protected:
      // Parts of a triangle
      Eigen::Vector3f a, b, c;
      Eigen::Vector3f normal;
      Eigen::Vector3f transformedNormal;
      float areaSqr;

      void Initialize();
};

#endif
