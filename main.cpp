#ifdef __APPLE__
#include <GLUT/glut.h>
#endif
#ifdef __unix__
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <memory>

#include "Scene.hpp"
#include "Parse.hpp"
#include "Image.hpp"
#include "types.h"

#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <vector>
#include <math.h>

using namespace std;

int width = 640;
int height = 480;
Scene scene;

int main(int argc, char **argv)
{
   FILE* infile;
   scene = Scene();
 
   if(argc != 2 && argc != 4) {
      cout << "Usage: rt <input_scene.pov>" << endl;
      cout << "Usage: rt <width> <height> <input_scene.pov>" << endl;
      exit(EXIT_FAILURE);
   }

   if(argc == 4) {
      width = std::stoi(argv[1]);
      height = std::stoi(argv[2]);
      infile = fopen(argv[3], "r");
   } else {
      infile = fopen(argv[1], "r");
   }

   if(infile) {
      cout << "Width: " << width << "   Height: " << height << endl;
      cout << Parse(infile, scene) << " objects parsed from scene file" << endl;
   }
   else {
      perror("fopen");
      exit(EXIT_FAILURE);
   }

   double t;

   #ifdef UNIT_TEST
   int testPoints[] = {320, 240, 170, 120, 490, 120, 170, 360, 490, 360};

   cout << "For file " << argv[3] << endl;
   for (int i = 0; i < 5; ++i)
   {
      // calculate camera ray
      Ray laser = ComputeCameraRay(testPoints[i*2], testPoints[i*2 + 1], width, height, scene.camera);
      cout << "Ray at (" << testPoints[i*2] << ", " << testPoints[i*2 + 1] << ") has position: \n";
      cout << laser.position << endl;
      cout << " and direction " << endl; 
      cout << laser.direction << endl;

      color_t clr = scene.ShootRayIntoScene(laser, t);

      cout << "Color: r = " << clr.r << " g = " << clr.g << " b = " << clr.b << " f = " << clr.f << endl;
      cout << endl;
      cout << "distance of hit: " << t << endl;
   }

   #else
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
         color_t clr = scene.ShootRayIntoScene(laser, t);

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
