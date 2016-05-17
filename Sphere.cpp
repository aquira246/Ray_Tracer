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

void Sphere::GetNormal(const Ray &ray, Eigen::Vector3f *hitNormal, double t) {
   Eigen::Vector3f dir, eye;

   // transform the ray into the object's world
   if (transformed) {
      transformRay(ray, &eye, &dir);
   } else {
      dir = ray.direction;
      eye = ray.position;
   }

   // calculate the normal
   Eigen::Vector3f hitPt = eye + dir*t;
   Eigen::Vector3f norm = (hitPt - center);
   norm.normalize();

   // transform the normal back 
   if (transformed) {
      // sets the hitnormal in the transformNormal function
      transformNormal(norm, hitNormal);
   } else {
      *hitNormal = norm;
   }
}

bool Sphere::CalculateHit(const Ray &ray, double &t, Shape *&hitShape) {
   Eigen::Vector3f dir, dist, eye;

   if (transformed) {
      transformRay(ray, &eye, &dir);
   } else {
      dir = ray.direction;
      eye = ray.position;
   }

   dist = eye - center;


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
   
   hitShape = this;

   return true;
}
