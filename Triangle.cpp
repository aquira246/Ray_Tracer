#include "Triangle.hpp"

using namespace std;

#define kEpsilon 1e-5

Triangle::Triangle() {
   //SetMaterialByNum(rand() % NUM_MATS);
   a = Eigen::Vector3f();
   b = Eigen::Vector3f();
   c = Eigen::Vector3f();
   Initialize();
}

Triangle::Triangle(Eigen::Vector3f pta, Eigen::Vector3f ptb, Eigen::Vector3f ptc) {
   //SetMaterialByNum(rand() % NUM_MATS);
   a = pta;
   b = ptb;
   c = ptc;
   Initialize();
}

Triangle::~Triangle(){

}

/*
 * Calculate normal and bounding sphere.
 */
void Triangle::Initialize() {
   // compute plane's normal
   Eigen::Vector3f ab = b - a;
   Eigen::Vector3f ac = c - a;

   // no need to normalize
   normal = ab.cross(ac);
   areaSqr = normal.norm();
   // the not offsetted center of the circumsphere
   center = normal.cross(ab) * ac.norm() + ac.cross(normal) * ab.norm();
   // radius ofthe circumsphere
   radius = center.norm();
   // offset the center properly in the world
   center += a;
   normal.normalize();

   #ifndef CULLING
   isFlat = true;
   #endif
}

void Triangle::Parse(Triangle &triangle) {
   ParseLeftCurly();

   ParseVector(triangle.a);
   ParseComma();
   ParseVector(triangle.b);
   ParseComma();
   ParseVector(triangle.c);
   Shape::ParseModifiers(triangle);

   ParseRightCurly();
   
   triangle.Initialize();
}

// help from 
// http://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection
// same method as what we did in class but cleaner!
bool Triangle::CalculateHit(const Ray &ray, double &t, Shape *&hitShape, Eigen::Vector3f *hitNormal) {
   double u, v;
   Eigen::Vector3f ab = b - a;
   Eigen::Vector3f ac = c - a;

   Eigen::Vector3f dir, pos;
   if (transformed) {
      transformRay(ray, &pos, &dir);
   } else {
      dir = ray.direction;
      pos = ray.position;
   }

   // get the vector perpendicular to D and pt a to pt c
   Eigen::Vector3f pvec = dir.cross(ac);

   // get the angle between the pvec and pt a to pt b
   // this is the angle between the ray and triangle
   float det = ab.dot(pvec);

   // if the ray and triangle are pretty parallel, return false
   // this is the case when the determinent is really close to 0
   if (fabs(det) < kEpsilon)
      return false;

   float inverseDet = 1.0/det;

   Eigen::Vector3f tvec = pos - a;

   u = tvec.dot(pvec)*inverseDet;

   // 0 < u < 1 also u + v == 1
   if (u < 0 || u > 1) 
      return false;

   Eigen::Vector3f qvec = tvec.cross(ab);
   v = dir.dot(qvec)*inverseDet;

   // 0 < v < 1 also u + v == 1 
   if (v < 0 || u + v > 1)
      return false;

   t = ac.dot(qvec)*inverseDet;

   if (transformed) {
      transformNormal(normal, hitNormal);
   } else {
      *hitNormal = normal;
   }

   hitShape = this;

   return true;
}
