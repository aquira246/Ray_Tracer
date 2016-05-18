#pragma once
#ifndef __SHAPE_H__
#define __SHAPE_H__

#include <Eigen/Dense>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Parse.hpp"
#include "Finish.hpp"
#include "Ray.hpp"

#define SHAPE_ID 0
#define SPHERE_ID 1
#define TRIANGLE_ID 2
#define PLANE_ID 3
#define BOX_ID 4

class Shape
{
   public:
      Shape();
      ~Shape();
      
      // Shape visual properties
      Finish finish;
      Eigen::Vector4f color;

      Eigen::Matrix4f m0, inverseM0, inverseTransposeM0;
      bool transformed;

      static void ParseModifiers(Shape &shape);
      
      virtual bool CalculateHit(const Ray &ray, double &t, Shape *&hitShape) {
         return 0;
      }

      virtual int GetShape() {
         return SHAPE_ID;
      }

      virtual void GetNormal(const Ray &ray, Eigen::Vector3f *hitNormal, double t) {
         
      }

      void transformRay(const Ray &ray, Eigen::Vector3f *position, Eigen::Vector3f *direction);
      void transformNormal(const Eigen::Vector3f &hitNormal, Eigen::Vector3f *transformedNormal);

   private:
};

// return vector: inxex 1: how many answers there are
// index 2: the positive output
// index 3: the negative output
int QuadraticFormula(double A, double B, double C, double &plusOp, double &minOp);

#endif
