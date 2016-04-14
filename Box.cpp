#include "Box.hpp"

#define kEpsilon 1e-5 

using namespace std;

Box::Box() {
   center = Eigen::Vector3f(0,0,0);
   normal = Eigen::Vector3f(0,0,-1);
   radius = 1.0f;
   corner1 = Eigen::Vector3f(-.5, -.5, -.5);
   corner2 = Eigen::Vector3f(.5, .5, .5);
}

Box::Box(Eigen::Vector3f c1, Eigen::Vector3f c2) {
   center = Eigen::Vector3f(0,0,0);
   normal = Eigen::Vector3f(0,0,-1);
   radius = 1.0f;

   corner1 = c1;
   corner2 = c2;
}

Box::~Box(){

}

// TODO Test
void Box::Parse(Box &box) {
   box.corner1 = Eigen::Vector3f(-.5, -.5, -.5);
   box.corner2 = Eigen::Vector3f(.5, .5, .5);

   ParseLeftCurly();
   ParseVector(box.corner1);
   ParseComma();
   ParseVector(box.corner2);
   Shape::ParseModifiers(box);
   ParseRightCurly();
}

void float_swap(float &a, float &b) {
   float hold = a;
   a = b;
   b = hold;
}

// TODO test. This is untested. Also switch to unaxis aligned
// http://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
bool Box::CalculateHit(Ray ray, double &t) {
   float tmin = (corner1(0) - ray.position(0)) / ray.direction(0);
   float tmax = (corner2(0) - ray.position(0)) / ray.direction(0);

   if (tmin > tmax) float_swap(tmin, tmax);

   float tymin = (corner1(1) - ray.position(1)) / ray.direction(1);
   float tymax = (corner2(1) - ray.position(1)) / ray.direction(1);

   if (tymin > tymax) float_swap(tymin, tymax);

   if ((tmin > tymax) || (tymin > tmax))
      return false;

   if (tymin > tmin)
      tmin = tymin;

   if (tymax < tmax)
      tmax = tymax;

   float tzmin = (corner1(2) - ray.position(2)) / ray.direction(2);
   float tzmax = (corner2(2) - ray.position(2)) / ray.direction(2);

   if (tzmin > tzmax) float_swap(tzmin, tzmax);

   if ((tmin > tzmax) || (tzmin > tmax))
      return false;

   if (tzmin > tmin)
      tmin = tzmin;

   if (tzmax < tmax)
      tmax = tzmax;

   if (tmin > tmax) float_swap(tmin, tmax);

   t = tmin;
   return true;
}

// TODO 
Eigen::Vector3f Box::GetNormal(Eigen::Vector3f hitPt) {
   return Eigen::Vector3f(0,0,1);
}
