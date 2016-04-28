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

      // No matter the shape we generate a bounding sphere
      Eigen::Vector3f center;
      float radius;

      #ifndef CULLING
      bool isFlat; // flat objects need to have 2 faces checked
      #endif

      static void ParseModifiers(Shape &shape);
      
      virtual bool CalculateHit(Ray ray, double &t) {
         return 0;
      }

      virtual Eigen::Vector3f GetNormal(Eigen::Vector3f hitPt) {
         cout << "BAD! Should call Shape's Get Normal" << endl;
         return Eigen::Vector3f(0,0,0);
      }

      virtual int GetShape() {
         return SHAPE_ID;
      }

   private:
};

// return vector: inxex 1: how many answers there are
// index 2: the positive output
// index 3: the negative output
Eigen::Vector3f QuadraticFormula(double A, double B, double C);

#endif
