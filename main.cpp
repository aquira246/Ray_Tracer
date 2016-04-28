#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <memory>
#include <Eigen/Dense>
#include <vector>
#include <math.h>
#include <string>

#include "Image.hpp"
#include "Ray.hpp"
#include "Scene.hpp"
#include "unit_tests.hpp"

using namespace std;

const int MAX_REFLECTIONS = 5;
int width = 640;
int height = 480;
int shader = 0;
Scene scene;

int main(int argc, char **argv)
{
   #ifdef UNIT_TEST
   
   // Run the Unit Tests
   UnitTest1();
   
   #else
   FILE* infile;
   char *fileName;
   scene = Scene();
 
   if(argc < 2 || argc > 5) {
      cout << "Usage: rt <input_scene.pov>   | default 640 by 480 with Blinn-Phong Shading" << endl;
      cout << "Usage: rt <input_scene.pov> <shader>  | default 640 by 480" << endl;
      cout << "Usage: rt <width> <height> <input_scene.pov>   | default Blinn-Phong Shading" << endl;
      cout << "Usage: rt <width> <height> <input_scene.pov> <shader>" << endl;
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
   } else {
      width = stoi(argv[1]);
      height = stoi(argv[2]);
      fileName = argv[3];
      shader = stoi(argv[4]);
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
      cout << "Width: " << width << "   Height: " << height << "   Shader: " << shaderName << endl;
      cout << Scene::Parse(infile, scene) << " objects parsed from scene file" << endl;
   } else {
      perror("fopen");
      exit(EXIT_FAILURE);
   }

   scene.setShader(shader);

   double t;

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
            Ray laserbeam = ComputeCameraRay(x, y, width, height, scene.cameras[c]);
            
            // get the color that the ray provides
            Eigen::Vector3f clr = scene.ShootRayIntoScene(laserbeam, t, 1, NULL, MAX_REFLECTIONS);

            // set the image pixel to be that color
            img->pixel(x, y, clr);
         }
      }

      // draw picture to sample.tga
      string outfile = fileName;
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
