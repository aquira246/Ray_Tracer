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

// t = (n dot (plane point - ray position))/(n dot ray direction)
bool Plane::CalculateHit(Ray ray, double &t) {
   t = -1;

   // assuming vectors are all normalized
   // calculate the denominator
   double denom = normal.dot(ray.direction);

   // if the denominator is 0, than the ray is parallel to the plane 
   if (fabs(denom) > kEpsilon) {
      // calculate hit
      t = normal.dot((center - ray.position)) / denom;
   }


   if (t < 0) return false;

   // stop here if this is an infinite plane
   if (radius < 0) {
      return true;
   }

   // calculate the intersection point
   Eigen::Vector3f p = ray.position + ray.direction * t;

   // calculate how far away the point is from the center
   Eigen::Vector3f v = p - center;
   double d2 = v.dot(v);  // this number is squared

   // if the point is less than the radius away, than it is still a hit
   if (d2 <= radius*radius) return true;

   return false;
}
