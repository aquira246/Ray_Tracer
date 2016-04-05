#include "Finish.hpp"

Finish::Finish() {

}

Finish::~Finish() {

}

void Finish::Parse(Finish &finish) {
   ParseLeftCurly();

   finish.ambient = 0.1;
   finish.diffuse = 0.6;
   finish.specular = 0.0;
   finish.roughness = 0.05;
   finish.reflection = 0.0;
   finish.refraction = 0.0;
   finish.ior = 1.0;

   while(1) { 
      GetToken();
      switch(Token.id) {
      case T_AMBIENT:
         finish.ambient = ParseDouble();
         assert(finish.ambient > 0 && finish.ambient < 1);
         break;
      case T_DIFFUSE:
         finish.diffuse = ParseDouble();
         // Diffuse lighting is unbounded
         break;
      case T_SPECULAR:
         finish.specular = ParseDouble();
         assert(finish.specular >= 0 && finish.specular <= 1);
         break;
      case T_ROUGHNESS:
         finish.roughness = ParseDouble();
         assert(finish.roughness > 0 && finish.roughness <= 1);
         break;
      case T_REFLECTION:
         finish.reflection = ParseDouble();
         assert(finish.reflection == 0 && finish.reflection == 1);
         break;
      case T_REFRACTION:
         finish.refraction = ParseDouble();
         assert(finish.refraction >= 0 && finish.refraction <= 1);
         break;
      case T_IOR:
         finish.ior = ParseDouble();
         // Index of refraction is unbounded
         break;
      case T_RIGHT_CURLY:
         return;
      default:
         Error("Error parsing finish: unexpected token");
      }   
   }
}

void Finish::PrintFinish(Finish &finish) {
   printf("\tfinish { ambient %.3g diffuse %.3g specular %.3g roughness %.3g reflection %.3g refraction %.3g ior %.3g }\n",
         finish.ambient, finish.diffuse, 
         finish.specular, finish.roughness, 
         finish.reflection, finish.refraction, finish.ior);
}
