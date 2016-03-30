#pragma  once
#ifndef __Sphere__
#define __Sphere__

#include <Eigen/Dense>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream> 
#include "Shape.hpp"

class Sphere: public Shape
{
	public:
		Sphere();
		Sphere(Eigen::Vector3f c);
		Sphere(float r);
		Sphere(Eigen::Vector3f c, float r);
		~Sphere();
		
		// Shape has a center and radius, the only components of a sphere

	private:
};

#endif
