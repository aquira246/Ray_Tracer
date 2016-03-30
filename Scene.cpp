#include "Scene.hpp"
#include "Sphere.hpp"
#include "Triangle.hpp"

using namespace std;

Scene::Scene() {
	lights.clear();
	triangles.clear();
	spheres.clear();
}

Scene::~Scene() {
	lights.clear();
	triangles.clear();
	spheres.clear();
}
