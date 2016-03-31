#pragma  once
#ifndef __SHAPE__
#define __SHAPE__

#include <Eigen/Dense>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream> 

#include "Ray.hpp"

// POV-ray material
struct Material
{
   Eigen::Vector3f rgb;
   float ambient, diffuse, specular, roughness, shine;
} typedef Material;


class Shape
{
    public:
        Shape();
        ~Shape();
        
        Material mat;
        // No matter the shape we generate a bounding sphere
        Eigen::Vector3f center;
        float radius;
        #ifndef CULLING
        bool isFlat; // flat objects need to have 2 faces checked
        #endif

        void SetMaterialToMat(Material newMat); 
        virtual bool CalculateHit(Ray ray, double &t) {
            return 0;
        }

    private:
};

// return vector: inxex 1: how many answers there are
// index 2: the positive output
// index 3: the negative output
Eigen::Vector3f QuadraticFormula(double A, double B, double C);

#endif
