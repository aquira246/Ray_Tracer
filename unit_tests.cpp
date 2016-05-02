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

void UnitTest1_Helper1() {
   string filename = "resources/simple.pov";
   int width = 640, height = 480;
   FILE* infile = fopen(filename.c_str(), "r");
   Scene scene = Scene();
   Scene::Parse(infile, scene);

   int testPoints[] = {170, 120, 490, 120, 320, 240, 170, 360, 490, 360};
   int testPositions[] = {0,0,14,0,0,14,0,0,14,0,0,14,0,0,14};
   double testDirections[] = {-.289308, -0.231253, -0.928883,
                              0.325871, -0.228397, -0.917411,
                              0.00104165, 0.00104165, -0.999999,
                              -.289178, .233084, -0.928466,
                              0.325728, .230207, -0.917009
                              };

   bool success = true;
   for (int i = 0; i < 5; ++i)
   {
      // calculate camera ray
      Ray laser = ComputeCameraRay(testPoints[i*2], testPoints[i*2 + 1], width, height, scene.cameras[0]);

      int tp1 = testPositions[i*3];
      int tp2 = testPositions[i*3+1];
      int tp3 = testPositions[i*3+2];
      double td1 = testDirections[i*3];
      double td2 = testDirections[i*3+1];
      double td3 = testDirections[i*3+2];

      if (!checkDifference(laser.position, Eigen::Vector3f(tp1, tp2, tp3)) ||
            !checkDifference(laser.direction, Eigen::Vector3f(td1, td2, td3))) {
         cout << "Ray at (" << testPoints[i*2] << ", " << testPoints[i*2 + 1] << ") has position: \n";
         cout << "(" << laser.position(0) << ", " << laser.position(1) << ", " << laser.position(2) << ")\n";
         cout << " and direction " << endl; 
         cout << "(" << laser.direction(0) << ", " << laser.direction(1) << ", " << laser.direction(2) << ")\n\n";

         cout << "Should be position:\n";
         cout << "(" << tp1 << ", " << tp2 << ", " << tp3 << ")\n";
         cout << " and direction " << endl; 
         cout << "(" << td1 << ", " << td2 << ", " << td3 << ")\n";

         success = false;
      }
   }

   if (success)
      cout << "Successful test\n" << endl;
   else
      cout << "Unit Test 1 Failed\n" << endl;
}

void UnitTest1_Helper2(string filename, int width, int height, int *tp, double *tr, Eigen::Vector3f *tc, int n) {
   FILE* infile = fopen(filename.c_str(), "r");
   Scene scene = Scene();
   Scene::Parse(infile, scene);
   bool success = true;
   scene.setShader(0);

   double t;
   for (int i = 0; i < n; ++i)
   {
      // calculate camera ray
      Ray laser = ComputeCameraRay(tp[i*2], tp[i*2 + 1], width, height, scene.cameras[0]);
      Eigen::Vector3f clr = scene.ShootRayIntoScene(laser, t, 1, 1, 5);
      if (!(tc[i](0) <= 1 && tc[i](1) <= 1 && tc[i](2) <= 1)) {
         clr *= 255;
         clr(0) = ceil(clr(0));
         clr(1) = ceil(clr(1));
         clr(2) = ceil(clr(2));
      }

      if (!checkDifference(clr, tc[i]) || fabs(t - tr[i]) > EPSILON) {
         cout << endl;
         cout << "Ray at (" << tp[i*2] << ", " << tp[i*2 + 1] << ")\n";
         cout << "(" << laser.position(0) << ", " << laser.position(1) << ", " << laser.position(2) << ")\n";
         cout << " and direction " << endl; 
         cout << "(" << laser.direction(0) << ", " << laser.direction(1) << ", " << laser.direction(2) << ")\n\n";

         cout << "Color: r = " << clr(0) << " g = " << clr(1) << " b = " << clr(2) << endl;
         cout << "Should be: r = " << tc[i](0) << " g = " << tc[i](1) << " b = " << tc[i](2) << endl;
         cout << "distance of hit: " << t << endl;
         cout << "should have been: " << tr[i] << endl;
         success = false;
      }
   }



   if (success)
      cout << "Successful test\n" << endl;
   else
      cout << "Unit Test 1 Failed\n" << endl;
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
      cout << "Ray at [" << tp[i*2] << ", " << tp[i*2 + 1] << "]\n";
      cout << "{" << laser.position(0) << ", " << laser.position(1) << ", " << laser.position(2) << "} -> ";
      cout << "{" << laser.direction(0) << ", " << laser.direction(1) << ", " << laser.direction(2) << "}\n";
      cout << "---------------------------------------------------------" << endl;
      cout << "Iteration type: Primary" << endl;

      Eigen::Vector3f clr = scene.ShootRayIntoScene(laser, t, 1, 1, 5);
   }
}

void UnitTest1() {
   // int width = 640;
   // int height = 480;


   cout << " ================== Beginning unit test 1 ================== " << endl;


   int n = 4; //3;
   int testPositions[] = {100, 100, 265, 200, 320, 145, 429, 267};

   cout << "------------------- REFLECTION -------------------------" << endl;

   mytest("resources/simple_reflect3.pov", testPositions, n);

   cout << "------------------- REFRACTION -------------------------" << endl;

   n = 4; //3;
   int testPositions2[] = {295, 265, 315, 185, 315, 320, 370, 270};
   mytest("resources/refract2.pov", testPositions2, n);
}