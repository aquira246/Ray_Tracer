#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <memory>
#include <Eigen/Dense>
#include <vector>
#include <math.h>
#include <string>
#include <stack>

#include "Image.hpp"
#include "Ray.hpp"
#include "Scene.hpp"
#include "unit_tests.hpp"
#include "LoadingBar.h"

using namespace std;

const int MAX_REFLECTIONS = 5;
int GI_BOUNCES = -1;
int width = 640;
int height = 480;
int shader = 0;
Scene scene;
bool useAA = true;

int main(int argc, char **argv)
{
   /* initialize random seed: */
   srand (time(NULL));
   
   #ifdef UNIT_TEST
   
   // Run the Unit Tests
   UnitTest1();
   
   #else
   FILE* infile;
   char *fileName;
   scene = Scene();
 
   if(argc < 2 || argc > 7) {
      cout << "Usage: raytrace <input_scene.pov>   | default AA is on, 640 by 480 with Blinn-Phong Shading" << endl;
      cout << "Usage: raytrace <input_scene.pov> <shader>  | default AA is on, 640 by 480, and Blinn-Phong" << endl;
      cout << "Usage: raytrace <width> <height> <input_scene.pov>   | default AA is on, Blinn-Phong Shading" << endl;
      cout << "Usage: raytrace <width> <height> <input_scene.pov> <shader>   | default Blinn-Phong Shading" << endl;
      cout << "Usage: raytrace <width> <height> <input_scene.pov> <shader> <GlobalIllumination>";
      cout << "| AntiAliasing is off" << endl;
      cout << "Usage: raytrace <width> <height> <input_scene.pov> <shader> <GlobalIllumination> <AntiAliasing>" << endl;
      exit(EXIT_FAILURE);
   }

   if(argc == 2) {
      fileName = argv[1];
   } else if (argc == 3) {
      fileName = argv[1];
      shader = stoi(argv[2]);
   } else if (argc == 4) {
      width = stoi(argv[1]);
      height = stoi(argv[2]);
      fileName = argv[3];
   } else if (argc == 5) {
      width = stoi(argv[1]);
      height = stoi(argv[2]);
      fileName = argv[3];
      shader = stoi(argv[4]);
   } else if (argc == 6) {
      width = stoi(argv[1]);
      height = stoi(argv[2]);
      fileName = argv[3];
      shader = stoi(argv[4]);
      GI_BOUNCES = stoi(argv[5]);
      if (GI_BOUNCES != 0) {
         useAA = false;
         GI_BOUNCES = min(3, GI_BOUNCES);
         GI_BOUNCES = max(0, GI_BOUNCES);
      }
   } else {
      width = stoi(argv[1]);
      height = stoi(argv[2]);
      fileName = argv[3];
      shader = stoi(argv[4]);
      if (stoi(argv[6]) == 0)
         useAA = false;
      if (stoi(argv[5]) > 0)
         GI_BOUNCES = 2;
   }

   if (GI_BOUNCES <= 0) {
      GI_BOUNCES = -1;
   }

   infile = fopen(fileName, "r");

   string shaderName = "";

   if (shader == 0) {
      shaderName = "Blinn-Phong";
   } else if (shader == 1) {
      shaderName = "Cook-Torrence";
   } else if (shader == 2) {
      shaderName = "Toon-ish";
   } else {
      cout << "INVALID NUMBER! Defaulting to Blinn-Phong" << endl;
      shaderName = "Blinn-Phong";
      shader = 0;
   }

   if(infile) {
      cout << "Width: " << width << "   Height: " << height << "   Shader: " << shaderName << "   AA: ";
      if (useAA)
         cout << "on";
      else
         cout << "off";
      
      cout << "   GlobalIllumination: " << GI_BOUNCES << endl;
      cout << Scene::Parse(infile, scene) << " objects parsed from scene file" << endl;
   } else {
      perror("fopen");
      exit(EXIT_FAILURE);
   }

   scene.setShader(shader);

   std::vector<Ray> laserbeams;
   if (useAA)
      laserbeams.resize(9);
   else
      laserbeams.resize(1);

   double t;
   Eigen::Vector3f clr;

   cout << endl;
   for (unsigned int c = 0; c < scene.cameras.size(); ++c)
   {
      // create picture
      Image *img = new Image(width, height);

      // for every pixel find the color using scene
      for (int y = 0; y < height; ++y)
      {
         for (int x = 0; x < width; ++x)
         {
            // calculate camera ray
            if (useAA) {
               ComputeCameraRay_AntiAliasing(x, y, width, height, scene.cameras[c], laserbeams);
            } else {
               laserbeams[0] = ComputeCameraRay(x, y, width, height, scene.cameras[c]);
            }
            
            clr << 0, 0, 0;

            for (unsigned int i = 0; i < laserbeams.size(); ++i)
            {
               // get the color that the ray provides
               clr += scene.ShootRayIntoScene(laserbeams[i], t, 1, 1, MAX_REFLECTIONS, GI_BOUNCES);
            }

            clr /= (float)laserbeams.size();

            if (shader == 2) {
               clr = clr*7;
               clr[0] = round(clr[0]);
               clr[1] = round(clr[1]);
               clr[2] = round(clr[2]);

               clr = clr/7;

               clr[0] = fmin(clr[0], 1.0f);
               clr[1] = fmin(clr[1], 1.0f);
               clr[2] = fmin(clr[2], 1.0f);
           }

            // set the image pixel to be that color
            img->pixel(x, y, clr);
            
            // cout << "Ray Tracing: ";
            // cout << y << ", " << x << "                    \r";
         }

         cout << "Ray Tracing: ";
         printBar(y, height, 40);
      }
      cout << endl;

      // draw picture to sample.tga
      string outfile = fileName;
      outfile = outfile.substr(0, outfile.length() - 4);
      if (c == 0) {
         outfile = outfile + ".tga";
      } else {
         outfile = outfile + to_string(c) + ".tga";
      }

      cout << "Writing to: " << outfile << endl;
      char *holdName = (char *)outfile.c_str();
      img->WriteTga(holdName, true); 
   }
   #endif

	return 0;
}
