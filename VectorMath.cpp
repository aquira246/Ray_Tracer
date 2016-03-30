#include <Eigen/Dense>
#include <math.h>
#include "VectorMath.hpp"

float angle(Eigen::Vector3f U, Eigen::Vector3f V) {
   return acos(U.dot(V) / (U.norm() * V.norm()));
}
