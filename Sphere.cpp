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

void Sphere::Parse(Sphere &sphere) {
   sphere.center = Vector3f(0, 0, 0);
   sphere.radius = 1.0;

   ParseLeftCurly();
   ParseVector(sphere.center);
   ParseComma();
   sphere.radius = ParseDouble();
   Shape::ParseModifiers(sphere);
   ParseRightCurly();
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
   } else {
      if (quad(1) >= 0 && quad(2) >= 0) {
         t = (double)(min(quad(1), quad(2)));
      } else {
         t = (double)(max(quad(1), quad(2)));
      }
   }

   return true;
}

Eigen::Vector3f Sphere::GetNormal(Eigen::Vector3f hitPt) {
   Eigen::Vector3f ret = hitPt - center;
   ret = ret/radius;
   return ret;
}