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

void MyTests() {
   FILE* infile = fopen("resources/simple.pov", "r");
   Scene scene = Scene();
   Scene::Parse(infile, scene);

   double t;
   Ray laser = Ray(Eigen::Vector3f(0,0,12), Eigen::Vector3f(0,0,-1));
   scene.ShootRayIntoScene(laser, t);
   cout << "t is " << t << endl;
}

void UnitTest1_Helper(string filename, int width, int height) {
   FILE* infile = fopen(filename.c_str(), "r");
   Scene scene = Scene();
   cout << Scene::Parse(infile, scene) << " objects parsed from scene file: " << filename << endl;

   int testPoints[] = {320, 240, 170, 120, 490, 120, 170, 360, 490, 360};

   double t;
   for (int i = 0; i < 5; ++i)
   {
      // calculate camera ray
      Ray laser = ComputeCameraRay(testPoints[i*2], testPoints[i*2 + 1], width, height, scene.camera);
      cout << "Ray at (" << testPoints[i*2] << ", " << testPoints[i*2 + 1] << ") has position: \n";
      cout << laser.position << endl;
      cout << " and direction " << endl; 
      cout << laser.direction << endl;

      Eigen::Vector3f clr = scene.ShootRayIntoScene(laser, t);

      cout << "Color: r = " << clr(0) << " g = " << clr(1) << " b = " << clr(2) << endl;
      cout << endl;
      cout << "distance of hit: " << t << endl;
   }
}

void UnitTest1() {
   int width = 640;
   int height = 480;

   MyTests();

   // cout << " ================== Beginning unit test 1 ================== " << endl;
   // UnitTest1_Helper("resources/simple.pov", width, height);
   // UnitTest1_Helper("resources/planes.pov", width, height);
   // UnitTest1_Helper("resources/spheres.pov", width, height);
}