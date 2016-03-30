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
