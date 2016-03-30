#ifndef __TYPES_H__
#define __TYPES_H__

#include <vector>
#include "Triangle.hpp"
#include "Sphere.hpp"

/* Color struct */
typedef struct color_struct {
   double r;
   double g;
   double b;
   double f; // "filter" or "alpha"
} color_t;

typedef struct Camera {
   Eigen::Vector3f position;
   Eigen::Vector3f look_at;
   Eigen::Vector3f right;
   Eigen::Vector3f up;
} Camera;

typedef struct Light {
   color_t color;
   Eigen::Vector3f location;
} Light;

typedef struct DirLight {
   color_t color;
   Eigen::Vector3f direction;
} DirLight;

#endif
