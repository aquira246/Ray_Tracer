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

bool Sphere::CalculateHit(Ray &ray, double &t, Eigen::Vector3f *hitNormal) {
   Eigen::Vector3f dir, dist;

   if (transformed) {
      transformRay(ray, &dist, &dir);
      dist = dist - center;
   } else {
      dir = ray.direction;
      dist = ray.position - center;
   }

   double A = dir.dot(dir);
   double B = (2*dir).dot(dist);

   // double C = center.dot(center) + ray.position.dot(ray.position) + -2*ray.position.dot(center) - radius*radius;
   double C = dist.dot(dist) - radius*radius;
   double plusOp, minOp;

   int valids = QuadraticFormula(A, B, C, plusOp, minOp);

   if (valids == 0) {
      //SHOULD BE AN ERROR
      return false;
   }

   if (valids == 1) {
      t = (double)plusOp;
   } else {
      if (plusOp >= 0 && minOp >= 0) {
         t = (double)(min(plusOp, minOp));
      } else {
         t = (double)(max(plusOp, minOp));
      }
   }

   Eigen::Vector3f hitPt = ray.position + ray.direction*t;
   Eigen::Vector3f norm = (hitPt - center)/radius;

   if (transformed) {
      transformNormal(norm, hitNormal);
      Eigen::Vector3f n = *hitNormal;
      cout << "Normal: " << norm(0) << ", " << norm(1) << ", " << norm(2) 
            << "    xform: " << n[0] << ", " << n[1] << ", " << n[2] << "\n";
   } else {
      *hitNormal = norm;
   }
   
   return true;
}
