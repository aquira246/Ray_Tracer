#include "Scene.hpp"
#include "BRDF.hpp"

using namespace Eigen;
using namespace std;

#define EPSILON .001
const double PI = 3.141592653589793;

Scene::Scene() {
	lights.clear();
	triangles.clear();
	spheres.clear();
   planes.clear();
   boxes.clear();
   BackgroundColor = Eigen::Vector3f(0,0,0);
   shader = 0;
}

Scene::~Scene() {
	lights.clear();
	triangles.clear();
	spheres.clear();
   planes.clear();
   boxes.clear();
}

int Scene::getShader() {
    return shader;
}

void Scene::setShader(int s) {
    shader = s;
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

Eigen::Vector3f Scene::ShootRayIntoScene(Ray ray, double &t) {
    Shape *hitShape = NULL;
    Shape *shadowShape = NULL;
    BRDF brdf = BRDF();

    // see if ray hits anything
    if (CheckHit(ray, hitShape, t)) {
        // calculate the point the shape was hit at
        Eigen::Vector3f hitPt = ray.position + ray.direction*t;

        // set the color to ambient
        Eigen::Vector3f baseColor = hitShape->color.head<3>();
        Eigen::Vector3f retColor = baseColor*hitShape->finish.ambient;

        double s = -1; // shadow distance

        //check to see if the hit object is in shadow for each light
        for (unsigned int i = 0; i < lights.size(); ++i)
        {
            // create the shadow ray to check if the spot is in shadow
            Eigen::Vector3f l = lights[i].location - hitPt;
            double dist = l.norm();
            l.normalize();
            Ray shadowRay = Ray(hitPt + l*EPSILON, l);  // epsilon test for shadow ray
            
            // if the spot is not in shadow because there is no object between it and the light
            if(!CheckHit(shadowRay, shadowShape, s) || s > dist) {
                Eigen::Vector3f shading;

                // pick our shader
                if (shader == 0) {
                    shading = brdf.BlinnPhong(hitShape, hitPt, l, ray.direction);
                } else if (shader == 1) {
                    shading = brdf.CookTorrance(hitShape, hitPt, l, ray.direction);
                } else if (shader == 2) {
                    shading = brdf.ToonSorta(hitShape, hitPt, l, ray.direction);
                } else {
                    cout << "BAD SHADER VALUE! " << shader << "   Default to BlinnPhong" << endl;
                    shading = brdf.BlinnPhong(hitShape, hitPt, l, ray.direction);
                }

                shading[0] *= lights[i].color[0];
                shading[1] *= lights[i].color[1];
                shading[2] *= lights[i].color[2];

                retColor += shading;
            }
        }
    
        // compute lighting
        return retColor;
    } else {
        // return the background color
        return BackgroundColor;
    }
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
