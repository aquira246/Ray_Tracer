#pragma  once
#ifndef __Scene__
#define __Scene__

#include <stdlib.h>
#include <stdio.h>
#include <Eigen/Dense>
#include <vector>
#include "Parse.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "Shape.hpp"

//Add shapes here as needed
#include "Triangle.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"
#include "Box.hpp"

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
        std::vector<Box> boxes;

		Eigen::Vector3f ShootRayIntoScene(Ray ray, double &t);

    	static int Parse(FILE* infile, Scene &scene);

	private:
		Eigen::Vector3f BackgroundColor;
        Eigen::Vector3f ComputeLighting(Shape *hitShape, Eigen::Vector3f &hitPt, Eigen::Vector3f &l, Eigen::Vector3f &d);
        bool CheckHit(Ray checkRay, Shape *&hitShape, double &t);
};

#endif
