#include "Box.hpp"

#define kEpsilon 1e-5 

using namespace std;

Box::Box() {
   center = Eigen::Vector3f(0,0,0);
   normal = Eigen::Vector3f(0,0,-1);
   radius = 1.0f;
   corner1 = Eigen::Vector3f(-.5, -.5, -.5);
   corner2 = Eigen::Vector3f(.5, .5, .5);
   isBounding = false;
}

Box::Box(Eigen::Vector3f c1, Eigen::Vector3f c2, bool bounding) {
   center = Eigen::Vector3f(0,0,0);
   normal = Eigen::Vector3f(0,0,-1);
   radius = 1.0f;

   corner1 = c1;
   corner2 = c2;
   isBounding = bounding;
   transformed = false;
}

Box::~Box(){

}

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

// switch a and b (wanted to use xors but meh)
void float_swap(float &a, float &b) {
   float hold = a;
   a = b;
   b = hold;
}

void Box::GetNormal(const Ray &ray, Eigen::Vector3f *hitNormal, double t) {
   if (!isBounding) {
      Eigen::Vector3f eye, dir;

      if (transformed) {
         transformRay(ray, &eye, &dir);
      } else {
         dir = ray.direction;
         eye = ray.position;
      }

      // find the smallest/largest points per axis
      float txmin = corner1(0);
      float txmax = corner2(0);
      
      float tymin = corner1(1);
      float tymax = corner2(1);

      float tzmin = corner1(2);
      float tzmax = corner2(2);

      if (txmin > txmax) float_swap(txmin, txmax);
      if (tymin > tymax) float_swap(tymin, tymax);
      if (tzmin > tzmax) float_swap(tzmin, tzmax);

      // calculate normal
      Eigen::Vector3f hitPt = eye + dir*t;
      Eigen::Vector3f norm;

      if (hitPt(0) <= txmin + kEpsilon)
         norm(0) = -1;
      else if (hitPt(0) >= txmax - kEpsilon)
         norm(0) = 1;
      
      if (hitPt(1) <= tymin + kEpsilon)
         norm(1) = -1;
      else if (hitPt(1) >= tymax - kEpsilon)
         norm(1) = 1;
      
      if (hitPt(2) <= tzmin + kEpsilon)
         norm(2) = -1;
      else if (hitPt(2) >= tzmax - kEpsilon)
         norm(2) = 1;

      norm.normalize();

      //transform normal
      if (transformed) {
         // sets the hitnormal in the transformNormal function
         transformNormal(norm, hitNormal);
      } else {
         *hitNormal = norm;
      }
   }
}

bool Box::CalculateHit(const Ray &ray, double &t, Shape *&hitShape) {
   t = -1;

   // don't do any work when this is a bounding box with no contents
   if (isBounding && contents.size() == 0)
      return false;

   Eigen::Vector3f eye, dir;

   // transform the ray to the object's space
   if (transformed) {
      transformRay(ray, &eye, &dir);
   } else {
      dir = ray.direction;
      eye = ray.position;
   }

   // calculate the mins and max
   float txmin = (corner1(0) - eye(0)) / dir(0);
   float txmax = (corner2(0) - eye(0)) / dir(0);

   if (txmin > txmax) float_swap(txmin, txmax);

   float tmin = txmin;
   float tmax = txmax;

   float tymin = (corner1(1) - eye(1)) / dir(1);
   float tymax = (corner2(1) - eye(1)) / dir(1);

   if (tymin > tymax) float_swap(tymin, tymax);

   // cut out early if there is for sure no hit
   if ((tmin > tymax) || (tymin > tmax))
      return false;

   if (tymin > tmin)
      tmin = tymin;

   if (tymax < tmax)
      tmax = tymax;

   float tzmin = (corner1(2) - eye(2)) / dir(2);
   float tzmax = (corner2(2) - eye(2)) / dir(2);

   if (tzmin > tzmax) float_swap(tzmin, tzmax);

   if ((tmin > tzmax) || (tzmin > tmax))
      return false;

   // if we are a bounding box, we run the checks on that
   if (isBounding) {
      bool hit = false;
      double checkingT = 0;
      Eigen::Vector3f tempNormal;
      Shape *testShape;

      // check if we hit a shape
      for (unsigned int i = 0; i < contents.size(); ++i)
      {
         if (contents[i]->CalculateHit(ray, checkingT, testShape)) {
            if (checkingT > 0 && (checkingT < t || !hit)) {
               t = checkingT;
               hit = true;
               hitShape = testShape;
            }
         }
      }

      return hit;
   } else {
      // otherwise calculate the hit info
      if (tzmin > tmin)
         tmin = tzmin;

      if (tzmax < tmax)
         tmax = tzmax;

      if (tmin > tmax) float_swap(tmin, tmax);

      t = tmin;

      hitShape = this;

      return true;
   }
}

