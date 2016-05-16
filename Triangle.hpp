#pragma once
#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include <Eigen/Dense>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream> 

#include "VectorMath.hpp"
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

   protected:
   // Parts of a triangle
   Eigen::Vector3f a, b, c;
   Eigen::Vector3f normal;
   float areaSqr;

   void Initialize();
};

#endif
