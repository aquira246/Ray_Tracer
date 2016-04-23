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

// http://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/ray-triangle-intersection-geometric-solution
bool Triangle::CalculateHit(Ray ray, double &t) {
   double u, v;
   t = 0;
   // first check for circumsphere hit
   Eigen::Vector3f dist = ray.position - center;

   double A = ray.direction.dot(ray.direction);
   double B = (2*ray.direction).dot(dist);
   double C = dist.dot(dist) - radius*radius;

   Eigen::Vector3f quad = QuadraticFormula(A, B, C);
   float result;

   if (quad(0) == 0) {
      //SHOULD BE AN ERROR
      result = 0;
   }

   if (quad(0) == 1) {
      result = quad(1);
   }

   if (fabs(quad(1)) <= fabs(quad(2))) {
      result = quad(1);
   } else {
      result = quad(2);
   }

   // failure to even hit the circumsphere
   if (result < 0) {
      return false;
   }

   Eigen::Vector3f ab = b - a;
   Eigen::Vector3f ac = c - a;
   Eigen::Vector3f pvec = ray.direction.cross(ac);
   double det = ab.dot(pvec);

   #ifdef CULLING
   // if the determinant is negative the triangle is backfacing
   // if the determinant is close to 0, the ray misses the triangle
   if (det < kEpsilon) return false;
   #else
   // ray and triangle are parallel if det is close to 0
   if (fabs(det) < kEpsilon) return false;
   #endif
   double invDet = 1 / det;

   Eigen::Vector3f tvec = ray.position - a;
   u = tvec.dot(pvec) * invDet;
   if (u < 0 || u > 1) return false;

   Eigen::Vector3f qvec = tvec.cross(ab);
   v = ray.direction.dot(qvec) * invDet;
   if (v < 0 || u + v > 1) return false;

   t = ac.dot(qvec) * invDet;

   return true;
}

Eigen::Vector3f Triangle::GetNormal(Eigen::Vector3f hitPt) {
   return normal;
}