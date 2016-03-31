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

        color_t ShootRayIntoScene(Ray ray, double &t);

	private:
        color_t BackgroundColor;
        color_t ComputeLighting(Shape *hitShape);
        bool CheckHit(Ray checkRay, Shape *&hitShape, double &t);
};

#endif
