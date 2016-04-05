#pragma once
#ifndef __PARSE_H__
#define __PARSE_H__

/*
 * Base code for this parser from http://mrl.nyu.edu/~dzorin/cg05/handouts/pov-parser/index.html
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <iostream>
#include <math.h> 
#include <Eigen/Dense>
#include "Tokens.hpp"

using namespace Eigen;
using namespace std;

/* a collection of functions for syntax verification */
void ParseLeftAngle(); 
void ParseRightAngle();
double ParseDouble();
void ParseLeftCurly();
void ParseRightCurly();
void ParseComma();
void ParseVector(Vector3f &v);
void ParseRGBFColor(Eigen::Vector4f &c);
void ParseRGBColor(Eigen::Vector4f &c);
void ParseColor(Eigen::Vector4f &c);
void PrintColor(Eigen::Vector4f &c);
void ParsePigment(Eigen::Vector4f &pigment);
void PrintPigment(Eigen::Vector4f &pigment);

#endif
