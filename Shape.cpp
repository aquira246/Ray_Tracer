#include "Shape.hpp"

using namespace std;
using namespace Eigen;

Shape::Shape() {

}

Shape::~Shape(){

}

void Shape::ParseModifiers(Shape &shape) {
   Vector3f vector = Vector3f();
   
   while(1) { 
      GetToken();
      switch(Token.id) { 
         case T_SCALE:
            ParseVector(vector);
            // TODO - apply scale for shapes
            break;
         case T_ROTATE:
            ParseVector(vector);
            // TODO - apply rotate for shapes
            break;
         case T_TRANSLATE:
            ParseVector(vector);
            // TODO - apply translate for shapes with more than center
            break;
         case T_PIGMENT:
            ParsePigment(shape.color);
            break;
         case T_FINISH:
            Finish::Parse(shape.finish);
            break;   
         default:
            UngetToken();
            return;
      }
   }
}

// return vector: inxex 1: how many answers there are
// index 2: the positive output
// index 3: the negative output
Eigen::Vector3f QuadraticFormula(double A, double B, double C) {
    double discriminate = B*B - 4*A*C;

    if (discriminate < 0) {
        return Vector3f(0,0,0);
    }

    double sqrtDisc = sqrt(discriminate);

    float plusOp = (-B + sqrtDisc)/(2*A);

    if (discriminate == 0) {
        return Vector3f(1, plusOp, 0);
    }

    float minOp = (-B - sqrtDisc)/(2*A);

    return Vector3f(2, plusOp, minOp);
}
