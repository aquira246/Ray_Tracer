#include "Sphere.hpp"

using namespace std;

Sphere::Sphere() {
	//SetMaterialByNum(rand() % NUM_MATS);
	center = Eigen::Vector3f(0,0,0);
	radius = 1.0f;
   #ifndef CULLING
   isFlat = false;
   #endif
}
Sphere::Sphere(Eigen::Vector3f c) {
	//SetMaterialByNum(rand() % NUM_MATS);
	center = c;
	radius = 1.0f;
   #ifndef CULLING
   isFlat = false;
   #endif
}
Sphere::Sphere(float r){
	//SetMaterialByNum(rand() % NUM_MATS);
	center = Eigen::Vector3f(0,0,0);
	radius = r;
   #ifndef CULLING
   isFlat = false;
   #endif
}
Sphere::Sphere(Eigen::Vector3f c, float r){
	//SetMaterialByNum(rand() % NUM_MATS);
	center = c;
	radius = r;
   #ifndef CULLING
   isFlat = false;
   #endif
}
Sphere::~Sphere(){

}

