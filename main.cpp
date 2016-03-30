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

#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <vector>
#include <math.h>

using namespace std;

int width = 600;
int height = 600;
Scene scene;

int main(int argc, char **argv)
{
   FILE* infile;
   scene = Scene();
 
   if(argc != 2 && argc != 4) {
      cout << "Usage: rt <input_scene.pov>" << endl;
      cout << "Usage: rt <input_scene.pov> <width> <height>" << endl;
      exit(EXIT_FAILURE);
   }

   if(argc == 4) {
      width = std::stoi(argv[2]);
      height = std::stoi(argv[3]);
   }

   infile = fopen(argv[1], "r");
   if(infile) {
      cout << Parse(infile, scene) << " objects parsed from scene file" << endl;
   }
   else {
      perror("fopen");
      exit(EXIT_FAILURE);
   }

	return 0;
}
