#include "Light.hpp"

Light::Light() {

}

Light::~Light() {

}

void Light::Parse(Light &light) {
   ParseLeftCurly();

   ParseVector(light.location);
   GetToken();
   if(Token.id != T_COLOR) {
      Error("Error parsing light source: missing color");
   }
   ParseColor(light.color);

   ParseRightCurly();

}
