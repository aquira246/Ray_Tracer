#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Ray.hpp"

#define EPSILON .001

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

// help from https://www.cs.unc.edu/~rademach/xroads-RT/RTarticle.html
Ray ComputeReflectionRay(Eigen::Vector3f hitPt, Eigen::Vector3f surfaceNormal, Eigen::Vector3f rayDirection) {
   // angle between normal an reflected ray
   double c1 = -surfaceNormal.dot(rayDirection);

   // calcuate the reflected ray
   Eigen::Vector3f reflected = rayDirection + (2*surfaceNormal*c1);

   // epsilon shift the starting point
   return Ray(hitPt + reflected*EPSILON, reflected);
}

// help from https://www.cs.unc.edu/~rademach/xroads-RT/RTarticle.html
Ray ComputeRefractedRay(Eigen::Vector3f hitPt, Eigen::Vector3f surfaceNormal, Eigen::Vector3f rayDirection, 
                        double ior1, double ior2, bool *totalReflection) {

   double iorDiv = ior1/ior2;
   Eigen::Vector3f N = surfaceNormal;

   // might need? not sure yet
   if (N.dot(rayDirection) > 0) {
      N = -N;
   }


   // angle between normal an reflected ray
   double c1 = -N.dot(rayDirection);

   // get the radicand and see if it is imaginary
   double radicand = 1 - iorDiv*iorDiv*(1 - c1*c1); 

   if (radicand < 0) {
      // total internal reflection, no refraction
      *totalReflection = true;
      return Ray();
   }

   *totalReflection = false;

   // c2 is the angle between the reversed normal and the refracted ray
   double c2 = sqrt(radicand);

   // calculate the refracted ray
   Eigen::Vector3f refracted = (iorDiv*rayDirection) + (iorDiv*c1 - c2)*N ;

   // this is the in class version. The one I'm using seems more efficient since it does less math with the vector3f
   // Eigen::Vector3f refracted = iorDiv*(rayDirection + N*c1) - c2*N;

   // epsilon shift the starting point
   return Ray(hitPt + refracted*EPSILON, refracted);
}