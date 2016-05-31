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
#include "BVH.hpp"

//Add shapes here as needed
#include "Triangle.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"
#include "Box.hpp"

#define GI_SAMPLE_SIZE 128
#define GI_SMALL_SAMPLE_SIZE 64

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

        BVH bvh;

        int getShader();
        void setShader(int s);

		Eigen::Vector3f ShootRayIntoScene(const Ray &ray, double &t, double prevIOR, double curIOR, int bouncesLeft, int GIBounces);
        bool CheckHit(const Ray &checkRay, Shape *&hitShape, double &t, Eigen::Vector3f &hitNormal);   // in public for testing reasons
        bool ShadowHit(const Ray &checkRay, double lightDistance);

    	static int Parse(FILE* infile, Scene &scene);

	private:
        void ReflectionAndRefraction(Shape *hitShape, Eigen::Vector3f &retColor, const Eigen::Vector3f &dir, 
                                    const Eigen::Vector3f &hitPt, Eigen::Vector3f &hitNormal,  
                                    int bouncesLeft, double prevIOR, double oldIOR, double newIOR);
        int shader;
		Eigen::Vector3f BackgroundColor;
        float SoftShadowValue(const Ray &checkRay, double lightDistance);
};

#endif
