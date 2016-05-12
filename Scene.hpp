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

		std::vector<Camera> cameras;
		std::vector<Light> lights;
		std::vector<Triangle> triangles;
		std::vector<Sphere> spheres;
		std::vector<Plane> planes;
        std::vector<Box> boxes;
        std::vector<Shape *> shapes;

        int getShader();
        void setShader(int s);

		Eigen::Vector3f ShootRayIntoScene(const Ray &ray, double &t, double prevIOR, double curIOR, int bouncesLeft);
        bool CheckHit(const Ray &checkRay, Shape *&hitShape, double &t, Eigen::Vector3f &hitNormal);   // in public for testing reasons

    	static int Parse(FILE* infile, Scene &scene);

	private:
        int shader;
		Eigen::Vector3f BackgroundColor;
};

#endif
