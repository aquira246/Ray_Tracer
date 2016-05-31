#include "Box.hpp"

#define kEpsilon 1e-5 

using namespace std;

Box::Box() {
   corner1 = Eigen::Vector3f(-.5, -.5, -.5);
   corner2 = Eigen::Vector3f(.5, .5, .5);
}

Box::Box(Eigen::Vector3f c1, Eigen::Vector3f c2) {
   corner1 = c1;
   corner2 = c2;
   transformed = false;

   xmin = corner1(0);
   xmax = corner2(0);
   
   ymin = corner1(1);
   ymax = corner2(1);

   zmin = corner1(2);
   zmax = corner2(2);

   if (xmin > xmax) swap(xmin, xmax);
   if (ymin > ymax) swap(ymin, ymax);
   if (zmin > zmax) swap(zmin, zmax);
}

Box::~Box(){

}

void Box::Init() {
   xmin = corner1(0);
   xmax = corner2(0);

   ymin = corner1(1);
   ymax = corner2(1);

   zmin = corner1(2);
   zmax = corner2(2);

   if (xmin > xmax) swap(xmin, xmax);
   if (ymin > ymax) swap(ymin, ymax);
   if (zmin > zmax) swap(zmin, zmax);

   //transform normals if appropriate
   if (transformed) {
      transformNormal(Eigen::Vector3f(-1, 0, 0), &leftNormal);
      transformNormal(Eigen::Vector3f(1, 0, 0), &rightNormal);
      transformNormal(Eigen::Vector3f(0, -1, 0), &botNormal);
      transformNormal(Eigen::Vector3f(0, 1, 0), &topNormal);
      transformNormal(Eigen::Vector3f(0, 0, -1), &backNormal);
      transformNormal(Eigen::Vector3f(0, 0, 1), &frontNormal);
   }
   else {
      leftNormal = Eigen::Vector3f(-1, 0, 0);
      rightNormal = Eigen::Vector3f(1, 0, 0);
      botNormal = Eigen::Vector3f(0, -1, 0);
      topNormal = Eigen::Vector3f(0, 1, 0);
      backNormal = Eigen::Vector3f(0, 0, -1);
      frontNormal = Eigen::Vector3f(0, 0, 1);
   }
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

   box.Init();
}

void Box::GetNormal(const Ray &ray, Eigen::Vector3f *hitNormal, double t) {
   Eigen::Vector3f eye, dir;

   if (transformed) {
      transformRay(ray, &eye, &dir);
   } else {
      dir = ray.direction;
      eye = ray.position;
   }

   // calculate normal
   Eigen::Vector3f hitPt = eye + dir*t;

   if (hitPt(0) <= xmin + kEpsilon)
      *hitNormal = leftNormal;
   else if (hitPt(0) >= xmax - kEpsilon)
      *hitNormal =  rightNormal;
   
   else if (hitPt(1) <= ymin + kEpsilon)
      *hitNormal =  botNormal;
   else if (hitPt(1) >= ymax - kEpsilon)
      *hitNormal =  topNormal;
   
   else if (hitPt(2) <= zmin + kEpsilon)
      *hitNormal = backNormal;
   else if (hitPt(2) >= zmax - kEpsilon)
      *hitNormal = frontNormal;
   else
      // throw exception
      cout << "ERROR GETTING BOX NORMAL!" << endl;
}

bool Box::CalculateHit(const Ray &ray, double &t, Shape *&hitShape) {
   t = -1;

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

   if (txmin > txmax) swap(txmin, txmax);

   float tmin = txmin;
   float tmax = txmax;

   float tymin = (corner1(1) - eye(1)) / dir(1);
   float tymax = (corner2(1) - eye(1)) / dir(1);

   if (tymin > tymax) swap(tymin, tymax);

   // cut out early if there is for sure no hit
   if ((tmin > tymax) || (tymin > tmax))
      return false;

   if (tymin > tmin)
      tmin = tymin;

   if (tymax < tmax)
      tmax = tymax;

   float tzmin = (corner1(2) - eye(2)) / dir(2);
   float tzmax = (corner2(2) - eye(2)) / dir(2);

   if (tzmin > tzmax) swap(tzmin, tzmax);

   if ((tmin > tzmax) || (tzmin > tmax))
      return false;

   if (tzmin > tmin)
         tmin = tzmin;

   if (tzmax < tmax)
      tmax = tzmax;

   if (tmin > tmax) swap(tmin, tmax);

   // don't care if the box is behind the ray
   if (tmin < 0 && tmax < 0)
      return false;

   // calculate the hit info
   t = tmin < 0 ? tmax : tmin;
   hitShape = this;

   return true;
}

