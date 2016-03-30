#include "Plane.hpp"

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
