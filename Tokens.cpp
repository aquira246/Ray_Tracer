#include <stdio.h> 
#include <stdlib.h>
#include <assert.h>
#include <iostream> 
#include <string>

#include "Tokens.hpp"

#define MAX_STR_LENGTH 128
#define CR '\010'
#define LF '\0'

using namespace std;

/* Tokeninzing part of the parser; the parsing routines call 
 * GetToken() and UngetToken() to retrieve tokens from the input stream.
 * GetToken reads the input stream of characters stripping the comments 
 * until a complete token is produced. Then the token is placed into the
 * global variable Token, from which it can ber retrieved by the calling
 * routines. 
*/

Token_Struct Token;

/*
 * Simple method for checking for reserved words.
 * The words currently not supported are commented out.
 *
 * This could be done more efficiently with a map or hash table
 * but C++98 doesn't have pleasant intializers for those does it?
 */
static enum TokenIDs FindReserved(string str)
{
   if(!str.compare("rotate")) return T_ROTATE;
   if(!str.compare("translate")) return T_TRANSLATE;
   if(!str.compare("scale")) return T_SCALE;
   //if(!str.compare("matrix")) return T_MATRIX;
   //if(!str.compare("polygon")) return T_POLYGON;
   if(!str.compare("triangle")) return T_TRIANGLE;
   if(!str.compare("sphere")) return T_SPHERE;
   if(!str.compare("plane")) return T_PLANE;
   if(!str.compare("box")) return T_BOX;
   //if(!str.compare("cylinder")) return T_CYLINDER;
   //if(!str.compare("cone")) return T_CONE;
   //if(!str.compare("quadric")) return T_QUADRIC;
   if(!str.compare("camera")) return T_CAMERA;
   if(!str.compare("location")) return T_LOCATION;
   if(!str.compare("right")) return T_RIGHT;
   if(!str.compare("up")) return T_UP;
   if(!str.compare("look_at")) return T_LOOK_AT;
   if(!str.compare("angle")) return T_ANGLE;
   //if(!str.compare("global_settings")) return T_GLOBAL_SETTINGS;
   if(!str.compare("ambient_light")) return T_AMBIENT_LIGHT;
   if(!str.compare("light_source")) return T_LIGHT_SOURCE;
   if(!str.compare("finish")) return T_FINISH;
   if(!str.compare("pigment")) return T_PIGMENT;
   if(!str.compare("rgb")) return T_RGB;
   if(!str.compare("color")) return T_COLOR;
   if(!str.compare("rgbf")) return T_RGBF;
   if(!str.compare("reflection")) return T_REFLECTION;
   if(!str.compare("refraction")) return T_REFRACTION;
   if(!str.compare("ambient")) return T_AMBIENT;
   if(!str.compare("diffuse")) return T_DIFFUSE;
   if(!str.compare("specular")) return T_SPECULAR;
   if(!str.compare("roughness")) return T_ROUGHNESS;
   //if(!str.compare("phong")) return T_PHONG;
   //if(!str.compare("metallic")) return T_METALLIC;
   //if(!str.compare("phong_size")) return T_PHONG_SIZE;
   //if(!str.compare("interior")) return T_INTERIOR;
   if(!str.compare("ior")) return T_IOR;
   return T_NULL;
}

string TokenToString(enum TokenIDs id) {
   if(id == T_DOUBLE) return "Double";
   if(id == T_COMMA) return ",";

   if(id == T_LEFT_ANGLE) return "<";
   if(id == T_RIGHT_ANGLE) return ">";
   if(id == T_LEFT_CURLY) return "{";
   if(id == T_RIGHT_CURLY) return "}";
   if(id == T_ROTATE) return "Rotate";
   if(id == T_TRANSLATE) return "Translate";
   if(id == T_SCALE) return "Scale";
   //if(id == T_MATRIX) return "Matrix";
   //if(id == T_POLYGON) return "Polygon";
   if(id == T_PLANE) return "Plane";
   if(id == T_TRIANGLE) return "Triangle";
   if(id == T_SPHERE) return "Sphere";
   if(id == T_BOX) return "Box";
   //if(id == T_CYLINDER) return "Cylinder";
   //if(id == T_CONE) return "Cone";
   //if(id == T_QUADRIC) return "Quadratic";
   if(id == T_CAMERA) return "Camera";
   if(id == T_LOCATION) return "Location";
   if(id == T_RIGHT) return "Right";
   if(id == T_UP) return "Up";
   if(id == T_LOOK_AT) return "Look At";
   if(id == T_ANGLE) return "Angle";
   //if(id == T_GLOBAL_SETTINGS) return "Global Settings";
   if(id == T_AMBIENT_LIGHT) return "Ambient";
   if(id == T_LIGHT_SOURCE) return "Light Source";
   if(id == T_FINISH) return "Finish";
   if(id == T_PIGMENT) return "Pigment";
   if(id == T_COLOR) return "Color";
   if(id == T_RGB) return "RGB";
   if(id == T_RGBF) return "RGBF";
   if(id == T_REFLECTION) return "Reflection";
   if(id == T_REFRACTION) return "Refraction";
   if(id == T_AMBIENT) return "Ambient";
   if(id == T_DIFFUSE) return "Diffuse";
   if(id == T_SPECULAR) return "Specular";
   if(id == T_ROUGHNESS) return "Roughness";
   // if(id == T_PHONG) return "Phong";
   // if(id == T_METALLIC) return "Metallic";
   // if(id == T_PHONG_SIZE) return "Phong Size";
   // if(id == T_INTERIOR) return "Interior";
   if(id == T_IOR) return "IOR";
   if(id == T_NULL) return "NULL";
   if(id == T_EOF) return "EOF";
   if(id == T_LAST) return "Last";

   return "Unknown Token";
}

void Error(string str) {
  cout << "Line " << Token.lineNumber << ": " << str << endl;
  exit(EXIT_FAILURE);
}

/* should be called before GetToken() */
void InitializeToken(FILE* infile) { 
  Token.unget_flag = 0;
  Token.id = T_NULL;
  Token.infile = infile;
  Token.lineNumber = 1;
}


static void SkipSpaces() { 
   int c;
   while(1) { 
      c = getc(Token.infile);
      if(c == '\n') Token.lineNumber++;
      if(c == EOF) return;
      if(c ==  '/') {
         // we use slash only as a part of the 
         // comment begin sequence; if something other than another 
         // slash follows it, it is an error
         if( getc(Token.infile) == '/') {
            // skip everything till the end of the line
            while( c != '\n' && c != '\r' && c != EOF ) {
               c = getc(Token.infile);
            }
            Token.lineNumber++;
         } else Error("Missing second slash in comment");
      }
      if(!isspace(c))
      break;
   } 
   ungetc(c, Token.infile);
}

void ReadDouble() { 
   int res;
   res = fscanf(Token.infile, "%le", &Token.double_value);
   if(res == 1) Token.id = T_DOUBLE; 
   else Error("Could not read a number");
}

static void ReadName() { 
   char str[MAX_STR_LENGTH];
   int str_index;
   int c;

   str_index = 0;
   while (1) {
      c = getc(Token.infile);
      if (c == EOF) {
         Error("Could not read a name");
      }
      if (isalpha(c) || isdigit(c) || c == '_') {
      // if the name is too long, ignore extra characters
         if( str_index < MAX_STR_LENGTH - 1) {
            str[str_index++] = c;
         }
      }
      else {
         ungetc(c, Token.infile);
         break;
      }
   }

   str[str_index++] = '\0';
   Token.id = FindReserved(str);
   if(Token.id == T_NULL) { 
      fprintf(stderr, "%s: ", str); 
      Error("Unknown reserved word");
   }
}

/* 
 * Sets the global struct Token to the
 * next input token, if there is one. 
 * if there is no legal token in the input, 
 * returns 0, otherwise returns 1.
 */
void GetToken() { 
   int c;
   if(Token.unget_flag) { 
      Token.unget_flag = FALSE;
      return;
   }
  
   SkipSpaces();
   c = getc(Token.infile);
  
   if(c == EOF) { 
      Token.id = T_EOF;
      return;
   }
  
   if(isalpha(c)) { 
      ungetc(c, Token.infile);
      ReadName();
   } else if(isdigit(c) || c == '.' || c == '-' || c == '+' ) { 
      ungetc(c, Token.infile); 
      ReadDouble();
   } else { 
      switch(c) { 
         case ',':   
            Token.id = T_COMMA;
            break;    
         case '{': 
            Token.id = T_LEFT_CURLY;
            break;
         case '}': 
            Token.id = T_RIGHT_CURLY;
            break;
         case '<': 
            Token.id = T_LEFT_ANGLE;
            break;
         case '>': 
            Token.id = T_RIGHT_ANGLE;
            break;
         default:
            Error("Unknown token");
      }
   }
}

/* 
 * Returns a token to the data stream.
 * Assumes that GetToken() was called at least once. 
 * Cannot be called two times without a GetToken() between the calls.
 */
void UngetToken() { 
  assert(!Token.unget_flag);
  Token.unget_flag = TRUE;
}

