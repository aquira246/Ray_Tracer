#pragma once
#ifndef __SPHERE_H__
#define __SPHERE_H__

#include <Eigen/Dense>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream> 
#include "Shape.hpp"
#include "Ray.hpp"

class Sphere: public Shape
{
	public:
		Sphere();
		Sphere(Eigen::Vector3f c);
		Sphere(float r);
		Sphere(Eigen::Vector3f c, float r);
		~Sphere();
		
      static void Parse(Sphere &sphere);

		// Shape has a center and radius, the only components of a sphere
      bool CalculateHit(const Ray &ray, double &t, Shape *&hitShape);
      void GetNormal(const Ray &ray, Eigen::Vector3f *hitNormal, double t);

      virtual int GetShape() {
         return SPHERE_ID;
      }

	private:
};

#endif
