#include "Plane.hpp"

#define kEpsilon 1e-5 

using namespace std;

Plane::Plane() {
   center = Eigen::Vector3f(0,0,0);
   normal = Eigen::Vector3f(0,0,-1);
   radius = 1.0f;
   #ifndef CULLING
   isFlat = true;
   #endif
}

Plane::Plane(Eigen::Vector3f c, Eigen::Vector3f n, float r) {
   center = c;
   normal = n;
   radius = r;
   #ifndef CULLING
   isFlat = true;
   #endif
}

Plane::~Plane(){

}

void Plane::Parse(Plane &plane) {
   ParseLeftCurly();

   ParseVector(plane.normal);
   ParseComma();
   plane.center = ParseDouble() * plane.normal;
   Shape::ParseModifiers(plane);
   //planes have infinite radius so lets just use -1 to represent this
   plane.radius = -1;

   ParseRightCurly();
}

// t = (n dot (plane point - ray position))/(n dot ray direction)
bool Plane::CalculateHit(Ray &ray, double &t, Eigen::Vector3f *hitNormal) {
   t = -1;

   Eigen::Vector3f dir, pos;
   if (transformed) {
      transformRay(ray, &pos, &dir);
   } else {
      dir = ray.direction;
      pos = ray.position;
   }

   // assuming vectors are all normalized
   // calculate the denominator
   double denom = normal.dot(dir);

   // if the denominator is 0, than the ray is parallel to the plane 
   if (fabs(denom) > kEpsilon) {
      // calculate hit
      t = normal.dot((center - pos)) / denom;
   }

   if (t < 0) return false;

   if (transformed) {
      transformNormal(normal, hitNormal);
   } else {
      *hitNormal = normal;
   }

   // stop here if this is an infinite plane. All of our planes are infinite
   return true;
}
