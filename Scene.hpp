#pragma  once
#ifndef __Scene__
#define __Scene__

#include <Eigen/Dense>
#include <vector>
#include "Shape.hpp"
#include "Triangle.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"
#include "types.h"

class Scene
{
	public:
		Scene();
		~Scene();

		Camera camera;
		std::vector<Light> lights;
		std::vector<Triangle> triangles;
		std::vector<Sphere> spheres;
		std::vector<Plane> planes;

	private:
};

#endif
