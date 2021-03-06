#include "Plane.hpp"

#define kEpsilon 1e-5 

using namespace std;

Plane::Plane() {
   normal = Eigen::Vector3f(0,0,-1);
   center = Eigen::Vector3f(0,0,-1);
}

Plane::Plane(Eigen::Vector3f c, Eigen::Vector3f n, float r) {
   normal = n;
   center = c;
}

Plane::~Plane(){

}

void Plane::Init() {
   if (transformed) {
      transformNormal(normal, &transformedNormal);
   } else {
      transformedNormal = normal;
   }
}

void Plane::Parse(Plane &plane) {
   ParseLeftCurly();

   ParseVector(plane.normal);
   ParseComma();
   plane.center = ParseDouble() * plane.normal;
   Shape::ParseModifiers(plane);

   ParseRightCurly();
   plane.Init();
}

void Plane::GetNormal(const Ray &ray, Eigen::Vector3f *hitNormal, double t) {
   *hitNormal = transformedNormal;
}

// t = (n dot (plane point - ray position))/(n dot ray direction)
bool Plane::CalculateHit(const Ray &ray, double &t, Shape *&hitShape) {
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

   hitShape = this;

   // stop here if this is an infinite plane. All of our planes are infinite
   return true;
}
