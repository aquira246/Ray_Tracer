#include "Camera.hpp"

Camera::Camera() {

}

Camera::~Camera() {

}

void Camera::Parse(Camera &camera) {
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

void Camera::initialize() {
   t = up.norm()/2;
   b = -up.norm()/2;
   l = -right.norm()/2;
   r = right.norm()/2;

  direction = look_at - position;
  direction.normalize();
  
  upAxis = -direction.cross(right);
  upAxis.normalize();

  rightAxis = direction.cross(up);
  rightAxis.normalize();
}
