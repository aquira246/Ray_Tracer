#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Parse.hpp"

class Camera
{
   public:
      Camera();
      ~Camera();

      Eigen::Vector3f position;
      Eigen::Vector3f look_at;
      Eigen::Vector3f right;
      Eigen::Vector3f up;

      static void Parse(Camera &camera);

   private:
};

#endif
