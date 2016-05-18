#include "BVH.hpp"
#include "Sphere.hpp"
#include "Triangle.hpp"

#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

Box EncaseTriangle(Triangle *toEncase) {
    Vector3f a, b, c;
    toEncase->GetTransformedPts(&a, &b, &c);

    Vector3f minPts, maxPts;
    minPts(0) = min(min(a(0), b(0)), c(0));
    minPts(1) = min(min(a(1), b(1)), c(1));
    minPts(2) = min(min(a(2), b(2)), c(2));

    maxPts(0) = max(max(a(0), b(0)), c(0));
    maxPts(1) = max(max(a(1), b(1)), c(1));
    maxPts(2) = max(max(a(2), b(2)), c(2));

    Box ret = Box(minPts, maxPts, true);

    return ret;    
}

// Box EncaseSphere(Sphere *toEncase) {
//     float halfRad = toEncase->radius/2.0;
//     toEncase->center
// }

// Box EncaseShape(Shape *toEncase) {
//     Vector3f min, max;

//     return NULL;
// }