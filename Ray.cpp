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
    float normalized_i, normalized_j;
    double aspectRatio = (double) width / height;

   if(aspectRatio > 1) {
      normalized_i = ((i/(float)width) - 0.5) * aspectRatio;
      normalized_j = (j/(float)height) - 0.5;
   }
   else {
      normalized_i = (i/(float)width) - 0.5;
      normalized_j = ((j/(float)height) - 0.5) / aspectRatio;
   }

   Eigen::Vector3f imagePoint = normalized_i * cam.right + 
                                normalized_j * cam.up +
                                cam.position + cam.look_at;

   #ifdef DEBUG
   if (isnan(imagePoint(0)) || isnan(imagePoint(1)) || isnan(imagePoint(2)) ) {
      cout << "ERROR imagePoint has nans" << endl;
   }
   #endif

   Eigen::Vector3f ray_direction = imagePoint - cam.position;

   #ifdef DEBUG
   if (isnan(ray_direction(0)) || isnan(ray_direction(1)) || isnan(ray_direction(2)) ) {
      cout << "ERROR ray_direction has nans" << endl;
   }
   Eigen::Vector3f hold = ray_direction;
   #endif

   // Can't normalize if the ray direction is 0,0,0
   if (ray_direction(0) != 0 || ray_direction(1) != 0 || ray_direction(2) != 0)
      ray_direction.normalize();

   #ifdef DEBUG
   if (isnan(ray_direction(0)) || isnan(ray_direction(1)) || isnan(ray_direction(2)) ) {
      cout << "ERROR ray_direction has nans after normalize" << endl;
      cout << hold << endl;
   }
   #endif

   return Ray(cam.position, ray_direction);
}
