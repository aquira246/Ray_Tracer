/*
 * Base code for this parser from http://mrl.nyu.edu/~dzorin/cg05/handouts/pov-parser/index.html
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <iostream>
#include <math.h> 
#include <vector> 
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include "Tokens.hpp"
#include "types.h"
#include "Triangle.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"

#include "Parse.hpp"

using namespace Eigen;
using namespace std;

/* Functions in this file implement parsing operations for 
each object type. The main function to be called is Parse().
Each parsing function assigns values retrieved from the file 
to a set of local variables that can be used to create 
objects. Parsing stops at the first error and the program 
exits through function Error() defined in tokens.c, after
printing the current line number and an error message. 
(A real parser would have better error recovery, but this 
simple approach greatly simplifies the code and is sufficient
for our purposes).
*/

struct Finish { 
   double ambient;
   double diffuse;
   double specular;
   double roughness;
   double phong;
   double phong_size;
   double reflection;
   int metallic;
} typedef Finish;

struct ModifierStruct { 
   color_t pigment;
   struct Finish finish;
   double interior;
} typedef ModifierStruct;

/* a collection of functions for syntax verification */
void ParseLeftAngle() { 
  GetToken();
  if(Token.id != T_LEFT_ANGLE ) Error("Expected <");
}

void ParseRightAngle() { 
  GetToken();
  if(Token.id != T_RIGHT_ANGLE ) Error("Expected >");  
}

double ParseDouble() { 
  GetToken();
  if(Token.id != T_DOUBLE ) Error("Expected a number");
  return Token.double_value;
}

void ParseLeftCurly() { 
  GetToken();
  if(Token.id != T_LEFT_CURLY ) Error("Expected {");
}

void ParseRightCurly() { 
  GetToken();
  if(Token.id != T_RIGHT_CURLY ) Error("Expected }");
}

void ParseComma() { 
  GetToken();
  if(Token.id != T_COMMA ) Error("Expected ,");
}

void ParseVector(Vector3f &v) { 
  ParseLeftAngle();
  v[0] = ParseDouble();
  ParseComma();
  v[1] = ParseDouble();
  ParseComma();
  v[2] = ParseDouble();
  ParseRightAngle();
}

void ParseRGBFColor(color_t &c) {
  ParseLeftAngle();
  c.r = ParseDouble();
  ParseComma();
  c.g = ParseDouble();
  ParseComma();
  c.b = ParseDouble();
  ParseComma();
  c.f = ParseDouble();
  ParseRightAngle();
}

void ParseRGBColor(color_t &c) { 
  ParseLeftAngle();
  c.r = ParseDouble();
  ParseComma();
  c.g = ParseDouble();
  ParseComma();
  c.b = ParseDouble();
  c.f = 0.0;
  ParseRightAngle();
}

void ParseColor(color_t &c) { 
  GetToken();
  if(Token.id == T_RGB) 
    ParseRGBColor(c);
  else if ( Token.id == T_RGBF )
    ParseRGBFColor(c);
  else Error("Expected rgb or rgbf");
}

void PrintColor(color_t &c) { 
  printf("rgbf <%.3g,%.3g,%.3g,%.3g>", c.r, c.g, c.b, c.f);
}

void ParsePigment(color_t &pigment) { 
  ParseLeftCurly();
  while(1) { 
    GetToken();
    if(Token.id == T_COLOR)
      ParseColor(pigment);
    else if(Token.id == T_RIGHT_CURLY) return;
    else Error("error parsing pigment: unexpected token");
  }    
}

void PrintPigment(color_t &pigment) {
  printf("\tpigment { color ");
  PrintColor(pigment);
  printf("}");
}

void ParseFinish(Finish &finish) { 
  ParseLeftCurly();
  while(1) { 
    GetToken();
    switch(Token.id) {
    case T_AMBIENT:
      finish.ambient = ParseDouble();
      break;
    case T_DIFFUSE:
      finish.diffuse = ParseDouble();
      break;
    case T_SPECULAR:
      finish.specular= ParseDouble();
      break;
    case T_ROUGHNESS:
      finish.roughness= ParseDouble();
      break;
    case T_PHONG:
      finish.phong = ParseDouble();
      break;
    case T_PHONG_SIZE:
      finish.phong_size = ParseDouble();
      break;
    case T_REFLECTION:
      finish.reflection = ParseDouble();
      break;
    case T_METALLIC:
      finish.metallic = (ParseDouble() != 0.0 ? 1: 0);
      break;
    case T_RIGHT_CURLY: return;
    default: Error("Error parsing finish: unexpected token");
    }   
  } 
}

void PrintFinish(Finish &finish) { 
  printf("\tfinish { ambient %.3g diffuse %.3g phong %.3g phong_size %.3g reflection %.3g metallic %d }\n", 
         finish.ambient, finish.diffuse, 
         finish.phong, finish.phong_size, 
         finish.reflection, finish.metallic);
}

void ParseInterior(double interior) { 
  ParseLeftCurly();
  while(1) {
    GetToken();
    if( Token.id == T_RIGHT_CURLY) return;
    else if (Token.id == T_IOR) { 
      interior = ParseDouble();
    } else Error("Error parsing interior: unexpected token\n");
  }
}


void InitModifiers(ModifierStruct &modifiers) { 
  modifiers.pigment.r = 0;
  modifiers.pigment.g = 0;
  modifiers.pigment.b = 0;
  modifiers.pigment.f = 0;

  modifiers.finish.ambient    = 0.1;
  modifiers.finish.diffuse    = 0.6;
  modifiers.finish.phong      = 0.0;
  modifiers.finish.phong_size = 40.0;
  modifiers.finish.reflection = 0;
  modifiers.finish.roughness  = 0.05;
  modifiers.finish.specular   = 0.0;

  modifiers.interior = 1.0; 
}

void PrintModifiers(ModifierStruct &modifiers) {
  PrintPigment(modifiers.pigment);
  printf("\n"); 
  PrintFinish(modifiers.finish);
  printf("\tinterior { ior %.3g }\n", modifiers.interior);
}

void ParseModifiers(Material &mat) {
   ModifierStruct modifiers;
   InitModifiers(modifiers);

   while(1) { 
      GetToken();
      switch(Token.id) { 
      case T_SCALE:
      case T_ROTATE:
      case T_TRANSLATE:
      case T_PIGMENT:
         ParsePigment(modifiers.pigment);
         break;
      case T_FINISH:
         ParseFinish(modifiers.finish);
         break;
      case T_INTERIOR:
         ParseInterior(modifiers.interior);
         break;      
      default: UngetToken();
         mat.rgb = Eigen::Vector3f(modifiers.pigment.r,
                             modifiers.pigment.g,
                             modifiers.pigment.b);
         mat.ambient = modifiers.finish.ambient;
         mat.diffuse = modifiers.finish.diffuse;
         mat.specular = modifiers.finish.specular;
         mat.roughness = modifiers.finish.roughness;
         mat.shine = 1 / modifiers.finish.roughness;
         return;
      }
   }

}

void ParseCamera(Camera &camera) { 
   double angle;
   int done = FALSE;

   ParseLeftCurly();

   // parse camera parameters
   while(!done) {     
      GetToken();
      switch(Token.id) { 
      case T_LOCATION:
         ParseVector(camera.position);
         break;
      case T_RIGHT:
         ParseVector(camera.right);
         break;
      case T_UP:
         ParseVector(camera.up);
         break;
      case T_LOOK_AT:
         ParseVector(camera.look_at);
         break;
      case T_ANGLE:
         angle = M_PI * ParseDouble() / 180.0;
         break;
      default:
         done = TRUE;
         UngetToken();
         break;
      }
   }

   ParseRightCurly(); 
}

void ParseSphere(vector<Sphere> &spheres) { 
   Vector3f center; 
   double radius; 
   Material mat;

   center = Vector3f(0, 0, 0);
   radius = 1.0;

   ParseLeftCurly();
   ParseVector(center);
   ParseComma();
   radius = ParseDouble();
   ParseModifiers(mat);
   ParseRightCurly();

   spheres.push_back(Sphere(center, radius));
   spheres.back().SetMaterialToMat(mat);
}

void ParseTriangle(vector<Triangle> &triangles) { 
   Vector3f vert1, vert2, vert3;
   Material mat;

   ParseLeftCurly();

   ParseVector(vert1);
   ParseComma();
   ParseVector(vert2);
   ParseComma();
   ParseVector(vert3);
   ParseModifiers(mat);

   ParseRightCurly();

   triangles.push_back(Triangle(vert1, vert2, vert3));
   triangles.back().SetMaterialToMat(mat);
}

void ParsePlane(vector<Plane> &planes) { 
   Eigen::Vector3f normal;
   float dist;
   Material mat;

   ParseLeftCurly();

   ParseVector(normal);
   ParseComma();
   dist = (float) ParseDouble();
   ParseModifiers(mat);

   ParseRightCurly();

   planes.push_back(Plane(Eigen::Vector3f(0,dist,0), normal, -1));
   planes.back().SetMaterialToMat(mat);
}

void ParseLightSource(vector<Light> &lights) {
   Light light;
   ParseLeftCurly();

   ParseVector(light.location);
   GetToken();
   if(Token.id != T_COLOR) Error("Error parsing light source: missing color");
   ParseColor(light.color);

   ParseRightCurly();

   lights.push_back(light);
} 

void ParseGlobalSettings() { 
   color_t ambient;
   ParseLeftCurly();
   while(1) { 
     GetToken();
     if(Token.id == T_AMBIENT_LIGHT) {
       ParseLeftCurly();
       GetToken();
       if(Token.id != T_COLOR) 
         Error("Error parsing light source: missing color");
       ParseColor(ambient);
       ParseRightCurly();
     } else if(Token.id == T_RIGHT_CURLY) { 
       break;
     } else         
       Error("error parsing default settings: unexpected token");
   }
}

/* main parsing function calling functions to parse each object;  */
int Parse(FILE* infile, Scene &scene) {
   int numObjects = 0;
   InitializeToken(infile);
   GetToken();
   while(Token.id != T_EOF) { 
      switch(Token.id) { 
         case T_CAMERA:
            ParseCamera(scene.camera);
            break;
         case T_TRIANGLE:
            ParseTriangle(scene.triangles);
            break;
         case T_SPHERE:
            ParseSphere(scene.spheres);
            break;
         case T_PLANE:
            ParsePlane(scene.planes);
            break;
         case T_LIGHT_SOURCE:
            ParseLightSource(scene.lights);
            break;
         case T_GLOBAL_SETTINGS:
            ParseGlobalSettings();
            break;
         default: Error("Unknown statement");
      }
      GetToken();
      ++numObjects;
   }
   return numObjects;
}
