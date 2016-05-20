#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <memory>
#include <Eigen/Dense>
#include <vector>
#include <math.h>
#include <string>

#include "Ray.hpp"
#include "Scene.hpp"
#include "unit_tests.hpp"

#define EPSILON .0001

bool checkDifference(Eigen::Vector3f a, Eigen::Vector3f b) {
   Eigen::Vector3f diff = a - b;

   return (fabs(diff(0)) < EPSILON && fabs(diff(1)) < EPSILON && fabs(diff(2)) < EPSILON);
}

void mytest(string filename, int *tp, int n) {
   FILE* infile = fopen(filename.c_str(), "r");
   Scene scene = Scene();
   Scene::Parse(infile, scene);
   scene.setShader(0);

   double t;
   for (int i = 0; i < n; ++i)
   {
      // calculate camera ray
      Ray laser = ComputeCameraRay(tp[i*2], tp[i*2 + 1], 640, 480, scene.cameras[0]);
      
      cout << endl;
      cout << "-------\n";
      cout << "Pixel [" << tp[i*2] << ", " << tp[i*2 + 1] << "]   ";
      cout << "Ray: {" << laser.position(0) << ", " << laser.position(1) << ", " << laser.position(2) << "} -> ";
      cout << "{" << laser.direction(0) << ", " << laser.direction(1) << ", " << laser.direction(2) << "}\n";
      cout << "Iteration type: Primary" << endl;

      Eigen::Vector3f clr = scene.ShootRayIntoScene(laser, t, 1, 1, 5);
      cout << "---------------------------------------------------------" << endl;

   }
}

void UnitTest1() {
   // int width = 640;
   // int height = 480;


   cout << " ================== Beginning unit test 1 ================== " << endl;


   int n = 2;
   int testPositions[] = {399, 280, 240, 280};

   mytest("resources/valentine2.pov", testPositions, n);
}