#include "Sphere.hpp"

using namespace std;

Sphere::Sphere() {
	center = Eigen::Vector3f(0,0,0);
	radius = 1.0f;
   #ifndef CULLING
   isFlat = false;
   #endif
}
Sphere::Sphere(Eigen::Vector3f c) {
	center = c;
	radius = 1.0f;
   #ifndef CULLING
   isFlat = false;
   #endif
}
Sphere::Sphere(float r){
	center = Eigen::Vector3f(0,0,0);
	radius = r;
   #ifndef CULLING
   isFlat = false;
   #endif
}
Sphere::Sphere(Eigen::Vector3f c, float r){
	center = c;
	radius = r;
   #ifndef CULLING
   isFlat = false;
   #endif
}
Sphere::~Sphere(){

}

// TODO
bool Sphere::CalculateHit(Ray ray, double &t) {
   Eigen::Vector3f dir = ray.direction;
   Eigen::Vector3f dist = ray.position - center;

   double A = dir.dot(dir);
   double B = (2*dir).dot(dist);

   // double C = center.dot(center) + ray.position.dot(ray.position) + -2*ray.position.dot(center) - radius*radius;
   double C = dist.dot(dist) - radius*radius;

   Eigen::Vector3f quad = QuadraticFormula(A, B, C);

   if (quad(0) == 0) {
      //SHOULD BE AN ERROR
      return false;
   }

   if (quad(0) == 1) {
      t = (double)quad(1);
   } else if (fabs(quad(1)) <= fabs(quad(2))) {
      t = (double)quad(1);
   } else {
      t = (double)quad(2);
   }

   return true;
}

