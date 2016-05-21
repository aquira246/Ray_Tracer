#include <Eigen/Dense>
#include "BoundingBox.h"

#define kEpsilon 1e-5 

using namespace std;
using namespace Eigen;

// switch a and b (wanted to use xors but meh)
void Float_Swap(float &a, float &b) {
   float hold = a;
   a = b;
   b = hold;
}

BoundingBox::BoundingBox() {
   mins = Eigen::Vector3f(-.5, -.5, -.5);
   maxs = Eigen::Vector3f(.5, .5, .5);
   contents = NULL;
}

BoundingBox::BoundingBox(Eigen::Vector3f c1, Eigen::Vector3f c2, Shape *c) {
   mins = c1;
   maxs = c2;

   if (mins(0) > maxs(0)) Float_Swap(mins(0), maxs(0));
   if (mins(1) > maxs(1)) Float_Swap(mins(1), maxs(1));
   if (mins(2) > maxs(2)) Float_Swap(mins(2), maxs(2));
   contents = c;
}

BoundingBox::~BoundingBox(){

}

bool BoundingBox::CalculateHit(const Ray &ray, double &t, double maxT, Shape *&hitShape) {
   Eigen::Vector3f eye, dir;
   dir = ray.direction;
   eye = ray.position;

   // first we get rid of parallel lines that won't hit
   if(dir(0) == 0 && (eye(0) < mins(0) || eye(0) > maxs(0)))
      return false;
   if(dir(1) == 1 && (eye(1) < mins(1) || eye(1) > maxs(0)))
      return false;
   if(dir(2) == 2 && (eye(2) < mins(2) || eye(2) > maxs(0)))
      return false;

   // calculate the mins and max
   float txmin = (mins(0) - eye(0)) / dir(0);
   float txmax = (maxs(0) - eye(0)) / dir(0);

   if (txmin > txmax) Float_Swap(txmin, txmax);

   float tmin = txmin;
   float tmax = txmax;

   float tymin = (mins(1) - eye(1)) / dir(1);
   float tymax = (maxs(1) - eye(1)) / dir(1);

   if (tymin > tymax) Float_Swap(tymin, tymax);

   // cut out early if there is for sure no hit
   if ((tmin > tymax) || (tymin > tmax))
      return false;

   if (tymin > tmin)
      tmin = tymin;

   if (tymax < tmax)
      tmax = tymax;

   float tzmin = (mins(2) - eye(2)) / dir(2);
   float tzmax = (maxs(2) - eye(2)) / dir(2);

   if (tzmin > tzmax) Float_Swap(tzmin, tzmax);

   if ((tmin > tzmax) || (tzmin > tmax))
      return false;

   if (tzmin > tmin)
         tmin = tzmin;

   if (tzmax < tmax)
      tmax = tzmax;

   if (tmin > tmax) Float_Swap(tmin, tmax);

   // don't care if the box is too far from the ray
   if (tmin > maxT)
      return false;

   // don't do any more work when this is a bounding box with no contents
   if (!contents) {
      t = tmin;
      return true;
   }

   // check if we hit the contents
   double checkingT = 0;
   bool hit = contents->CalculateHit(ray, checkingT, hitShape);

   if (hit && checkingT <= maxT && checkingT > 0) {
      t = checkingT;
      return true;
   } else {
      return false;
   }
}

BoundingBox EncaseTriangle(Triangle *toEncase) {
    Vector3f a, b, c;
    toEncase->GetTransformedPts(&a, &b, &c);

    Vector3f minPts, maxPts;
    minPts(0) = min(min(a(0), b(0)), c(0));
    minPts(1) = min(min(a(1), b(1)), c(1));
    minPts(2) = min(min(a(2), b(2)), c(2));

    maxPts(0) = max(max(a(0), b(0)), c(0));
    maxPts(1) = max(max(a(1), b(1)), c(1));
    maxPts(2) = max(max(a(2), b(2)), c(2));

    BoundingBox ret = BoundingBox(minPts, maxPts, toEncase);
    return ret;    
}

BoundingBox EncaseSphere(Sphere *toEncase) {
    Vector3f minPts = Vector3f(0,0,0), maxPts = Vector3f(0,0,0);

    float rad = toEncase->radius;
    Vector3f center = toEncase->center;

    if (toEncase->transformed) {
        Vector4f holdC = Vector4f(center(0), center(1), center(2), 1);
        Vector4f pt[8];

        pt[0] = holdC + Vector4f(rad, -rad, -rad, 0);
        pt[1] = holdC + Vector4f(rad, rad, rad, 0);
        pt[2] = holdC + Vector4f(-rad, rad, rad, 0);
        pt[3] = holdC + Vector4f(-rad, -rad, rad, 0);
        pt[4] = holdC + Vector4f(rad, -rad, rad, 0);
        pt[5] = holdC + Vector4f(rad, rad, -rad, 0);
        pt[6] = holdC + Vector4f(-rad, rad, -rad, 0);
        pt[7] = holdC + Vector4f(-rad, -rad, -rad, 0);

        for (int i = 0; i < 8; ++i)
        {
            pt[i] = toEncase->m0*pt[i];
            for (int j = 0; j < 3; ++j)
            {
                minPts(j) = min(pt[i][j], minPts(j));
                maxPts(j) = max(pt[i][j], maxPts(j)); 
            }
        }
    } else {
        minPts = center - Vector3f(rad, rad, rad);
        maxPts = center + Vector3f(rad, rad, rad);
    }

    BoundingBox ret = BoundingBox(minPts, maxPts, toEncase);
    return ret;   
}

BoundingBox EncaseBox(Box *toEncase) {
    Vector3f minPts = Vector3f(0,0,0), maxPts = Vector3f(0,0,0);
    float xmin = toEncase->xmin, ymin = toEncase->ymin, zmin = toEncase->zmin;
    float xmax = toEncase->xmax, ymax = toEncase->ymax, zmax = toEncase->zmax;

    if (toEncase->transformed) {
        Vector4f pt[8];

        pt[0] = Vector4f(xmin, ymin, zmin, 1);
        pt[1] = Vector4f(xmax, ymin, zmin, 1);
        pt[2] = Vector4f(xmax, ymax, zmin, 1);
        pt[3] = Vector4f(xmax, ymin, zmax, 1);
        pt[4] = Vector4f(xmax, ymax, zmax, 1);
        pt[5] = Vector4f(xmin, ymax, zmin, 1);
        pt[6] = Vector4f(xmin, ymax, zmax, 1);
        pt[7] = Vector4f(xmin, ymin, zmax, 1);

        for (int i = 0; i < 8; ++i)
        {
            pt[i] = toEncase->m0*pt[i];
            minPts(0) = min(pt[i][0], minPts(0)); 
            minPts(1) = min(pt[i][1], minPts(1)); 
            minPts(2) = min(pt[i][2], minPts(2));

            maxPts(0) = max(pt[i][0], maxPts(0)); 
            maxPts(1) = max(pt[i][1], maxPts(1)); 
            maxPts(2) = max(pt[i][2], maxPts(2)); 
        }
    } else {
        minPts = Vector3f(xmin, ymin, zmin);
        maxPts = Vector3f(xmax, ymax, zmax);
    }

    BoundingBox ret = BoundingBox(minPts, maxPts, toEncase);
    return ret; 
}