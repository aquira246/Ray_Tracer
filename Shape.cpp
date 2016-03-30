#include "Shape.hpp"

using namespace std;

Shape::Shape() {
    //SetMaterialByNum(rand() % NUM_MATS);
    //center = Eigen::Vector3f(0,0,0);
}

Shape::~Shape(){

}

void Shape::SetMaterialToMat(Material newMat) {
   mat = newMat;
}

// return vector: inxex 1: how many answers there are
// index 2: the positive output
// index 3: the negative output
Eigen::Vector3f QuadraticFormula(double A, double B, double C) {
    double discriminate = B*B - 4*A*C;

    if (discriminate < 0) {
        return Eigen::Vector3f(0,0,0);
    }

    double sqrtDisc = sqrt(discriminate);

    float plusOp = (-B + sqrtDisc)/(2*A);

    if (discriminate == 0) {
        return Eigen::Vector3f(1, plusOp, 0);
    }

    float minOp = (-B - sqrtDisc)/(2*A);

    return Eigen::Vector3f(2, plusOp, minOp);
}
