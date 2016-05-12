#pragma once
#ifndef __FINISH_H__
#define __FINISH_H__

#include <stdio.h>
#include <Eigen/Dense>
#include "Tokens.hpp"
#include "Parse.hpp"

class Finish {
   public:
      Finish();
      ~Finish();

      double ambient;
      double diffuse;
      double specular;
      double roughness;
      double reflection;
      double refraction;
      double ior;

      static void Parse(Finish &finish);
      static void PrintFinish(const Finish &finish);

   private:
};

#endif
