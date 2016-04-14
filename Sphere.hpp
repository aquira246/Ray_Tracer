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
      bool CalculateHit(Ray ray, double &t);
      virtual Eigen::Vector3f GetNormal(Eigen::Vector3f hitPt);

	private:
};

#endif
