#pragma  once
#ifndef __Triangle__
#define __Triangle__

#include <Eigen/Dense>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream> 

#include "VectorMath.hpp"
#include "Shape.hpp"

class Triangle: public Shape
{
   public:
      Triangle();
      Triangle(Eigen::Vector3f pta, Eigen::Vector3f ptb, Eigen::Vector3f ptc);
      ~Triangle();

   protected:
   // Parts of a triangle
   Eigen::Vector3f a, b, c;
   Eigen::Vector3f normal;
   float areaSqr;

   void Initialize();
};

#endif
