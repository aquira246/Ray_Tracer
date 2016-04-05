#include "Scene.hpp"

using namespace Eigen;
using namespace std;

Scene::Scene() {
	lights.clear();
	triangles.clear();
	spheres.clear();
   planes.clear();
   boxes.clear();
   BackgroundColor = Eigen::Vector3f(0,0,0);
}

Scene::~Scene() {
	lights.clear();
	triangles.clear();
	spheres.clear();
   planes.clear();
   boxes.clear();
}

int Scene::Parse(FILE* infile, Scene &scene) {
   int numObjects = 0;
   InitializeToken(infile);
   GetToken();

   // For each list of objects have one that can be written to for push back
   Triangle triangle;
   Sphere sphere;
   Plane plane;
   Box box;
   Light light;

   while(Token.id != T_EOF) {
      switch(Token.id) { 
         case T_CAMERA:
            Camera::Parse(scene.camera);
            break;
         case T_TRIANGLE:
            Triangle::Parse(triangle);
            scene.triangles.push_back(triangle);
            break;
         case T_SPHERE:
            Sphere::Parse(sphere);
            scene.spheres.push_back(sphere);
            break;
         case T_PLANE:
            Plane::Parse(plane);
            scene.planes.push_back(plane);
            break;
         case T_BOX:
            Box::Parse(box);
            scene.boxes.push_back(box);
            break;
         case T_LIGHT_SOURCE:
            Light::Parse(light);
            scene.lights.push_back(light);
            break;
         default:
            Error("Unknown statement");
      }
      GetToken();
      ++numObjects;
   }

   cout << "Triangles: " << scene.triangles.size() << endl;
   cout << "Spheres: " << scene.spheres.size() << endl;
   cout << "Planes: " << scene.planes.size() << endl;
   cout << "Boxes: " << scene.boxes.size() << endl;
   cout << "Lights: " << scene.lights.size() << endl;

   scene.camera.initialize();

   return numObjects;
}

// TODO
Eigen::Vector3f Scene::ShootRayIntoScene(Ray ray, double &t) {
    Shape *hitShape = NULL;

    // see if ray hits anything
    if (CheckHit(ray, hitShape, t)) {
        //compute lighting if it does
        return ComputeLighting(hitShape);
    } else {
        // return the background color
        return BackgroundColor;
    }
}

Eigen::Vector3f Scene::ComputeLighting(Shape *hitShape) {
   Eigen::Vector3f ret;

   ret(0) = hitShape->color(0);
   ret(1) = hitShape->color(1);
   ret(2) = hitShape->color(2);

    return ret;
}

// TODO
bool Scene::CheckHit(Ray checkRay, Shape *&hitShape, double &t) {
    bool hit = false;
    double checkingT = 0;
    t = -1;

    // check if we hit a plane
    for (unsigned int i = 0; i < planes.size(); ++i)
    {
        if (planes[i].CalculateHit(checkRay, checkingT)) {
            if (checkingT > 0 && (checkingT < t || !hit)) {
                hitShape = &(planes[i]);
                t = checkingT;
                hit = true;
            }
        }
    }

    // check if we hit a sphere
    for (unsigned int i = 0; i < spheres.size(); ++i)
    {
        if (spheres[i].CalculateHit(checkRay, checkingT)) {
            if (checkingT > 0 && (checkingT < t || !hit)) {
                hitShape = &(spheres[i]);
                t = checkingT;
                hit = true;
            }
        }
    }

    return hit;
}
