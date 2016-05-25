#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Ray.hpp"

#define EPSILON .001
#define PI 3.1415

using namespace std;

float RandomFloat(float a, float b) {
   /* initialize random seed: */
   srand (time(NULL));
   
   float random = ((float) rand()) / (float) RAND_MAX;
   float diff = b - a;
   float r = random * diff;
   return a + r;
}

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

Ray ComputeCameraRay(int i, int j, int width, int height, const Camera &cam) {
   double us = cam.l + (cam.r - cam.l)*((double)i+.5)/(double)width;
   double vs = cam.b + (cam.t - cam.b)*((double)j+.5)/(double)height;

   Eigen::Vector3f ray_direction = cam.upAxis*vs + cam.rightAxis*us + cam.direction;
   ray_direction.normalize();

   return Ray(cam.position, ray_direction);
}

void ComputeCameraRay_AntiAliasing(int i, int j, int width, int height, const Camera &cam, std::vector<Ray> &rays) {
   int ctr = 0;
   for (int imod = 0; imod < 3; imod++)
   {
      for (int jmod = 0; jmod < 3; jmod++)
      {
         float randI = RandomFloat(0, .33) + imod*.33;
         float randJ = RandomFloat(0, .33) + jmod*.33;
         double us = cam.l + (cam.r - cam.l)*((double)i+randI)/(double)width;
         double vs = cam.b + (cam.t - cam.b)*((double)j+randJ)/(double)height;

         Eigen::Vector3f ray_direction = cam.upAxis*vs + cam.rightAxis*us + cam.direction;
         ray_direction.normalize();
         rays[ctr++] = Ray(cam.position, ray_direction);
      }
   }
}

// help from https://www.cs.unc.edu/~rademach/xroads-RT/RTarticle.html
Ray ComputeReflectionRay(const Eigen::Vector3f &hitPt, const Eigen::Vector3f &surfaceNormal, const Eigen::Vector3f &rayDirection) {
   // angle between normal an reflected ray
   double c1 = -surfaceNormal.dot(rayDirection);

   // calcuate the reflected ray
   Eigen::Vector3f reflected = rayDirection + (2*surfaceNormal*c1);

   // epsilon shift the starting point
   return Ray(hitPt + reflected*EPSILON, reflected);
}

// help from https://www.cs.unc.edu/~rademach/xroads-RT/RTarticle.html
Ray ComputeRefractedRay(const Eigen::Vector3f &hitPt, const Eigen::Vector3f &surfaceNormal, const Eigen::Vector3f &rayDirection, 
                        double ior1, double ior2, bool *totalReflection) {

   double iorDiv = ior1/ior2;

   // angle between normal an reflected ray
   double c1 = -surfaceNormal.dot(rayDirection);

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
   Eigen::Vector3f refracted = (iorDiv*rayDirection) + (iorDiv*c1 - c2)*surfaceNormal ;

   // this is the in class version. The one I'm using seems more efficient since it does less math with the vector3f
   // Eigen::Vector3f refracted = iorDiv*(rayDirection + surfaceNormal*c1) - c2*surfaceNormal;

   // epsilon shift the starting point
   return Ray(hitPt + refracted*EPSILON, refracted);
}

Eigen::Vector3f CosineSampleHemisphere()
{
   float u1 = RandomFloat(0, 1);
   float u2 = RandomFloat(0, 1);

   const float r = sqrt(u1);
   const float theta = 2 * PI * u2;

   const float x = r * cos(theta);
   const float y = r * sin(theta);

   return Eigen::Vector3f(x, y, sqrt(max(0.0f, 1 - u1)));
}

// u1 and u2 must be positive!!!
Eigen::Vector3f CosineSampleHemisphere(float u1, float u2)
{
   // u1 = fabs(u1);
   // u2 = fabs(u2);
   const float r = sqrt(u1);
   const float theta = 2 * PI * u2;

   const float x = r * cos(theta);
   const float y = r * sin(theta);

   return Eigen::Vector3f(x, y, sqrt(max(0.0f, 1 - u1)));
}