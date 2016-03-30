/*
 * Base code for this parser from http://mrl.nyu.edu/~dzorin/cg05/handouts/pov-parser/index.html
 */

#ifndef __PARSE_H__
#define __PARSE_H__

#include <stdio.h>
#include "Scene.hpp"
 
int Parse(FILE* infile, Scene &scene);

#endif /* __PARSE_H__ */
