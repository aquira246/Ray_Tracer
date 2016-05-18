#include "Box.hpp"

#define kEpsilon 1e-5 

using namespace std;

// switch a and b (wanted to use xors but meh)
void float_swap(float &a, float &b) {
   float hold = a;
   a = b;
   b = hold;
}

Box::Box() {
   corner1 = Eigen::Vector3f(-.5, -.5, -.5);
   corner2 = Eigen::Vector3f(.5, .5, .5);
   isBounding = false;
}

Box::Box(Eigen::Vector3f c1, Eigen::Vector3f c2, bool bounding) {
   corner1 = c1;
   corner2 = c2;
   isBounding = bounding;
   transformed = false;

   xmin = corner1(0);
   xmax = corner2(0);
   
   ymin = corner1(1);
   ymax = corner2(1);

   zmin = corner1(2);
   zmax = corner2(2);

   if (xmin > xmax) float_swap(xmin, xmax);
   if (ymin > ymax) float_swap(ymin, ymax);
   if (zmin > zmax) float_swap(zmin, zmax);
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

   box.xmin = box.corner1(0);
   box.xmax = box.corner2(0);

   box.ymin = box.corner1(1);
   box.ymax = box.corner2(1);

   box.zmin = box.corner1(2);
   box.zmax = box.corner2(2);

   if (box.xmin > box.xmax) float_swap(box.xmin, box.xmax);
   if (box.ymin > box.ymax) float_swap(box.ymin, box.ymax);
   if (box.zmin > box.zmax) float_swap(box.zmin, box.zmax);
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

      // calculate normal
      Eigen::Vector3f hitPt = eye + dir*t;
      Eigen::Vector3f norm = Eigen::Vector3f(0,0,0);

      if (hitPt(0) <= xmin + kEpsilon)
         norm(0) = -1;
      else if (hitPt(0) >= xmax - kEpsilon)
         norm(0) = 1;
      
      else if (hitPt(1) <= ymin + kEpsilon)
         norm(1) = -1;
      else if (hitPt(1) >= ymax - kEpsilon)
         norm(1) = 1;
      
      else if (hitPt(2) <= zmin + kEpsilon)
         norm(2) = -1;
      else if (hitPt(2) >= zmax - kEpsilon)
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

   // first we get rid of parallel lines that won't hit
   if(dir(0) == 0 && (eye(0) < xmin || eye(0) > xmax))
      return false;
   if(dir(1) == 1 && (eye(1) < ymin || eye(1) > ymax))
      return false;
   if(dir(2) == 2 && (eye(2) < zmin || eye(2) > zmax))
      return false;

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

   if (tzmin > tmin)
         tmin = tzmin;

   if (tzmax < tmax)
      tmax = tzmax;

   if (tmin > tmax) float_swap(tmin, tmax);

   // don't care if the box is behind the ray
   if (tmin < 0)
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
      t = tmin;
      hitShape = this;

      return true;
   }
}

