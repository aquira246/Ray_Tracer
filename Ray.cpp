#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Ray.hpp"

using namespace std;

Ray::Ray() {
    position = Eigen::Vector3f(0,0,0);
    direction = Eigen::Vector3f(0,0,1);
}

Ray::Ray(Eigen::Vector3f p, Eigen::Vector3f d) {
    position = p;
    direction = d;
}

Ray::~Ray() {

}

Ray ComputeCameraRay(int i, int j, int width, int height, Camera cam) {
  double us = cam.l + (cam.r - cam.l)*((double)i+.5)/(double)width;
  double vs = cam.b + (cam.t - cam.b)*((double)j+.5)/(double)height;

  Eigen::Vector3f ray_direction = cam.upAxis*vs + cam.rightAxis*us + cam.direction;
  ray_direction.normalize();

  return Ray(cam.position, ray_direction);
}
