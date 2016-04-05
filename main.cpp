#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <memory>
#include <Eigen/Dense>
#include <vector>
#include <math.h>

#include "Image.hpp"
#include "Ray.hpp"
#include "Scene.hpp"
#include "unit_tests.hpp"

using namespace std;

int width = 640;
int height = 480;
Scene scene;

int main(int argc, char **argv)
{
   #ifdef UNIT_TEST
   
   // Run the Unit Tests
   UnitTest1();
   
   #else
   FILE* infile;
   scene = Scene();
 
   if(argc != 2 && argc != 4) {
      cout << "Usage: rt <input_scene.pov>" << endl;
      cout << "Usage: rt <width> <height> <input_scene.pov>" << endl;
      exit(EXIT_FAILURE);
   }

   if(argc == 4) {
      width = stoi(argv[1]);
      height = stoi(argv[2]);
      infile = fopen(argv[3], "r");
   } else {
      infile = fopen(argv[1], "r");
   }

   if(infile) {
      cout << "Width: " << width << "   Height: " << height << endl;
      cout << Scene::Parse(infile, scene) << " objects parsed from scene file" << endl;
   } else {
      perror("fopen");
      exit(EXIT_FAILURE);
   }

   double t;

   // create picture
   Image *img = new Image(width, height);

   // for every pixel find the color using scene
   for (int y = 0; y < height; ++y)
   {
      for (int x = 0; x < width; ++x)
      {
         // calculate camera ray
         Ray laser = ComputeCameraRay(x, y, width, height, scene.camera);
         
         // get the color that the ray provides
         Eigen::Vector3f clr = scene.ShootRayIntoScene(laser, t);

         // set the image pixel to be that color
         img->pixel(x, y, clr);
      }
   }

   // draw picture to sample.tga
   string outfile = "sample.tga";
   char *holdName = (char *)outfile.c_str();
   img->WriteTga(holdName, true); 
   #endif

	return 0;
}
