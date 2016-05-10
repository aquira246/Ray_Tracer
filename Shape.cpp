#include "Shape.hpp"

using namespace std;
using namespace Eigen;

Matrix4f generateRot(float xrot, float yrot, float zrot);

Shape::Shape() {
    m0 = Matrix4f::Identity();
    transformed = false;
}

Shape::~Shape(){

}

void Shape::ParseModifiers(Shape &shape) {
   Vector3f vector = Vector3f();
   
   while(1) { 
      GetToken();
      if (Token.id == T_SCALE) {
         ParseVector(vector);
         Eigen::Matrix4f scaleMat;
         if (shape.GetShape() != PLANE_ID && vector[0] != 0 && vector[1] != 0 && vector[2] != 0){
             scaleMat << vector[0], 0,         0,         0,
                         0,         vector[1], 0,         0,
                         0,         0,         vector[2], 0,
                         0,         0,         0,         1;
             shape.m0 = scaleMat*shape.m0;
             shape.transformed = true;
         }
      } else if (Token.id == T_ROTATE) {
         ParseVector(vector);
         Eigen::Matrix4f rotMat = generateRot(vector[0], vector[1], vector[2]);
         shape.m0 = rotMat*shape.m0;
         shape.transformed = true;
      } else if (Token.id == T_TRANSLATE) {
         ParseVector(vector);
         Matrix4f transMat;
         transMat << 1, 0, 0, vector[0],
                     0, 1, 0, vector[1],
                     0, 0, 1, vector[2],
                     0, 0, 0, 1;
         shape.m0 = transMat*shape.m0;
         shape.transformed = true;
      } else if (Token.id == T_PIGMENT) {
         ParsePigment(shape.color);
      } else if (Token.id == T_FINISH) {
         Finish::Parse(shape.finish);
      } else{

         if (shape.transformed) {
            shape.inverseM0 = shape.m0.inverse();
            shape.inverseTransposeM0 = shape.inverseM0.transpose();
         } else {
            shape.inverseTransposeM0 = shape.inverseM0 = shape.m0;
         }

         UngetToken();
         return;
      }
   }
}

void Shape::transformRay(Ray &ray, Eigen::Vector3f *position, Eigen::Vector3f *direction) {
   Eigen::Vector4f oldDir;
   oldDir << ray.direction[0], ray.direction[1], ray.direction[2], 0;

   Eigen::Vector4f oldPos;
   oldPos  << ray.position[0], ray.position[1], ray.position[2], 1;

   Eigen::Vector4f transformedDir  = (inverseM0*oldDir);
   Eigen::Vector4f transformedPos = (inverseM0*oldPos);
   *direction = transformedDir.head<3>();
   *position = transformedPos.head<3>();
}

void Shape::transformNormal(Eigen::Vector3f &hitNormal, Eigen::Vector3f *transformedNormal) {
   Eigen::Vector4f transNormal;
   transNormal << hitNormal[0], hitNormal[1], hitNormal[2], 0;

   Eigen::Vector4f hold = inverseTransposeM0*transNormal;
   
   *transformedNormal = hold.head<3>();
   transformedNormal->normalize();
}

// return vector: inxex 1: how many answers there are
// index 2: the positive output
// index 3: the negative output
int QuadraticFormula(double A, double B, double C, double &plusOp, double &minOp) {
    double discriminate = B*B - 4*A*C;

   if (discriminate < 0) {
      return 0;
   }

   double sqrtDisc = sqrt(discriminate);

   plusOp = (-B + sqrtDisc)/(2*A);

   if (discriminate == 0) {
      return 1;
   }

   minOp = (-B - sqrtDisc)/(2*A);
   return 2;
}

Eigen::Matrix4f generateRot(float xrot, float yrot, float zrot) {
    Eigen::Matrix4f ret;

   if (xrot > 0.01) {
      ret << 1, 0,         0,         0,
             0, cos(xrot),-sin(xrot), 0,
             0, sin(xrot), cos(xrot), 0,
             0, 0,         0,         1;
   } else if (yrot > 0.01) {
      ret << cos(yrot), 0, sin(yrot), 0,
             0,         1, 0,         0,
             -sin(xrot),0, cos(xrot), 0,
             0,         0, 0,         1;
   } else {
      ret << cos(zrot),-sin(zrot), 0, 0,
             sin(zrot), cos(zrot), 0, 0,
             0,         0,         1, 0,
             0,         0,         0, 1;
   }

    return ret;
}