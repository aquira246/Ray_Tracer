#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "Parse.hpp"

class Light
{
   public:
      Light();
      ~Light();

      Eigen::Vector4f color;
      Eigen::Vector3f location;

      static void Parse(Light &light);

   private:
};

#endif
