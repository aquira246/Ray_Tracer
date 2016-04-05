/*
 * Base code for this parser from http://mrl.nyu.edu/~dzorin/cg05/handouts/pov-parser/index.html
 */
#include <string>
#include <iostream>
#include "Parse.hpp"

using namespace Eigen;
using namespace std;

/* a collection of functions for syntax verification */
void ParseLeftAngle() { 
   GetToken();
   if(Token.id != T_LEFT_ANGLE ) {
    string msg;
    msg = "Expected <   Found " + TokenToString(Token.id);
    Error(msg);
  }
}

void ParseRightAngle() { 
   GetToken();
   if(Token.id != T_RIGHT_ANGLE ) {
    string msg;
    msg = "Expected >   Found " + TokenToString(Token.id);
    Error(msg);
  }
}

double ParseDouble() { 
   GetToken();
   if(Token.id != T_DOUBLE ) {
    string msg;
    msg = "Expected a number   Found " + TokenToString(Token.id);
    Error(msg);
  }
   return Token.double_value;
}

void ParseLeftCurly() { 
   GetToken();
   if(Token.id != T_LEFT_CURLY ) {
    string msg;
    msg = "Expected {   Found " + TokenToString(Token.id);
    Error(msg);
  }
}

void ParseRightCurly() { 
   GetToken();
   if(Token.id != T_RIGHT_CURLY ) {
    string msg;
    msg = "Expected }   Found " + TokenToString(Token.id);
    Error(msg);
  }
}

void ParseComma() { 
   GetToken();
   if(Token.id != T_COMMA ) {
    string msg;
    msg = "Expected ,   Found " + TokenToString(Token.id);
    Error(msg);
  }
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

void ParseRGBFColor(Eigen::Vector4f &c) {
  ParseLeftAngle();
  c[0] = ParseDouble();
  ParseComma();
  c[1] = ParseDouble();
  ParseComma();
  c[2] = ParseDouble();
  ParseComma();
  c[3] = ParseDouble();
  ParseRightAngle();
}

void ParseRGBColor(Eigen::Vector4f &c) { 
   ParseLeftAngle();
   c[0] = ParseDouble();
   ParseComma();
   c[1] = ParseDouble();
   ParseComma();
   c[2] = ParseDouble();
   c[3] = 1.0;
   ParseRightAngle();
}

void ParseColor(Eigen::Vector4f &c) { 
   GetToken();
   if(Token.id == T_RGB) {
      ParseRGBColor(c);
   }
   else if(Token.id == T_RGBF) {
      ParseRGBFColor(c);
   }
   else {
      Error("Expected rgb or rgbf");
   }
}

void PrintColor(Eigen::Vector4f &c) { 
  printf("rgbf <%.3g,%.3g,%.3g,%.3g>", c[0], c[1], c[2], c[3]);
}

void ParsePigment(Eigen::Vector4f &pigment) { 
   ParseLeftCurly();
   while(1) { 
      GetToken();
      if(Token.id == T_COLOR) {
         ParseColor(pigment);
      }
      else if(Token.id == T_RIGHT_CURLY) {
         return;
      }
      else {
         Error("error parsing pigment: unexpected token");
      }
   }    
}

void PrintPigment(Eigen::Vector4f &pigment) {
   printf("\tpigment { color ");
   PrintColor(pigment);
   printf("}");
}