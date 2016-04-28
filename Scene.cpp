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
   Triangle *triangle;
   Sphere *sphere;
   Plane *plane;
   Box *box;
   Light light;
   Camera cam;

   while(Token.id != T_EOF) {
      switch(Token.id) { 
         case T_CAMERA:
            Camera::Parse(cam);
            cam.initialize();
            scene.cameras.push_back(cam);
            break;
         case T_TRIANGLE:
            triangle = new Triangle();
            Triangle::Parse(*triangle);
            scene.shapes.push_back(triangle);
            scene.triangles.push_back(*triangle);
            break;
         case T_SPHERE:
            sphere = new Sphere();
            Sphere::Parse(*sphere);
            scene.shapes.push_back(sphere);
            scene.spheres.push_back(*sphere);
            break;
         case T_PLANE:
            plane = new Plane();
            Plane::Parse(*plane);
            scene.shapes.push_back(plane);
            scene.planes.push_back(*plane);
            break;
         case T_BOX:
            box = new Box();
            Box::Parse(*box);
            scene.shapes.push_back(box);
            scene.boxes.push_back(*box);
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
   cout << "Camera: " << scene.cameras.size() << endl;

   return numObjects;
}

Eigen::Vector3f Scene::ShootRayIntoScene(Ray ray, double &t, double ior, Shape *curShape, int bouncesLeft) {
    Shape *hitShape = NULL;
    Shape *shadowShape = NULL;

    // see if ray hits anything
    if (CheckHit(ray, hitShape, t)) {
        // calculate the point the shape was hit at
        Eigen::Vector3f hitPt = ray.position + ray.direction*t;

        // set the color to ambient
        Eigen::Vector3f baseColor = hitShape->color.head<3>();
        Eigen::Vector3f retColor = baseColor*hitShape->finish.ambient;
        double reflectVal = hitShape->finish.reflection;
        double refractVal = hitShape->finish.refraction;

        double s = -1; // shadow distance

        //check to see if the hit object is in shadow for each light
        for (unsigned int i = 0; i < lights.size(); ++i)
        {
            // create the shadow ray to check if the spot is in shadow
            Eigen::Vector3f l = lights[i].location - hitPt;
            double lightDistance = l.norm();
            l.normalize();
            Ray shadowRay = Ray(hitPt + l*EPSILON, l);  // epsilon test for shadow ray

            #ifdef UNIT_TEST
            cout << "\nShadow feeler: " << "(" << shadowRay.direction(0) << ", " 
                                        << shadowRay.direction(1) << ", " 
                                        << shadowRay.direction(2) << ")\n";
            #endif
            
            Eigen::Vector3f shading = Eigen::Vector3f(0,0,0);
            Eigen::Vector3f reflectColor = Eigen::Vector3f(0,0,0);
            Eigen::Vector3f refractColor = Eigen::Vector3f(0,0,0);

            // if we can still bounce around for reflection and refraction
            if (bouncesLeft > 0) {
                Eigen::Vector3f hitNormal = hitShape->GetNormal(hitPt);

                // if we have refraction calculate it
                if (refractVal > 0) {
                    // do refraction
                    Ray refrRay;
                    bool totalReflection = false;

                    if (curShape == NULL)
                        refrRay = ComputeRefractedRay(hitPt, hitNormal, ray.direction, ior, 
                                                        hitShape->finish.ior, &totalReflection);
                    else if(curShape != hitShape)
                        refrRay = ComputeRefractedRay(hitPt, hitNormal, ray.direction, curShape->finish.ior, 
                                                        hitShape->finish.ior, &totalReflection);
                    else
                        refrRay = ComputeRefractedRay(hitPt, hitNormal, ray.direction, hitShape->finish.ior, 
                                                        ior, &totalReflection);

                    double temp = 0;

                    if (!totalReflection) {
                        if (hitShape == curShape)
                            refractColor = ShootRayIntoScene(refrRay, temp, ior, NULL, bouncesLeft - 1);
                        else   
                            refractColor = ShootRayIntoScene(refrRay, temp, ior, hitShape, bouncesLeft - 1);
                    } else {
                        reflectVal += refractVal;
                        refractVal = 0;
                    }
                }

                // if we have reflection calculate it
                if (reflectVal > 0) {
                    // do reflection
                    Ray reflRay = ComputeReflectionRay(hitPt, hitNormal, ray.direction);
                    double temp = 0;

                    if (hitShape == curShape)
                        reflectColor = ShootRayIntoScene(reflRay, temp, ior, NULL, bouncesLeft - 1);
                    else   
                        reflectColor = ShootRayIntoScene(reflRay, temp, ior, hitShape, bouncesLeft - 1);
                }
            }

            // if the spot is not in shadow because there is no object between it and the light
            s = -1;
            bool inShadow = (CheckHit(shadowRay, shadowShape, s) || s > lightDistance);

            if(!inShadow) {
                
                BRDF brdf = BRDF();
                Eigen::Vector3f lightCol = Eigen::Vector3f(lights[i].color[0], lights[i].color[1], lights[i].color[2]);

                // pick our shader
                if (shader == 0) {
                    shading = brdf.BlinnPhong(hitShape, hitPt, l, ray.direction, lightCol);
                } else if (shader == 1) {
                    if (curShape == NULL)
                        shading = brdf.CookTorrance(hitShape, hitPt, l, ray.direction, lightCol, ior, hitShape->finish.ior);
                    else if(curShape != hitShape)
                        shading = brdf.CookTorrance(hitShape, hitPt, l, ray.direction, lightCol, curShape->finish.ior, hitShape->finish.ior);
                    else
                        shading = brdf.CookTorrance(hitShape, hitPt, l, ray.direction, lightCol, hitShape->finish.ior, ior);
                } else if (shader == 2) {
                    shading = brdf.ToonSorta(hitShape, hitPt, l, ray.direction, lightCol, &retColor);
                } else {
                    cout << "BAD SHADER VALUE! " << shader << "   Default to BlinnPhong" << endl;
                    shading = brdf.BlinnPhong(hitShape, hitPt, l, lightCol, ray.direction);
                }
            }

            retColor += (1 - (reflectVal + refractVal))*shading
                        + reflectVal*reflectColor
                        + refractVal*refractColor;
        }
    
        retColor[0] = fmin(retColor[0], 1.0f);
        retColor[1] = fmin(retColor[1], 1.0f);
        retColor[2] = fmin(retColor[2], 1.0f);

        // compute lighting
        return retColor;
    } else {
        // return the background color
        return BackgroundColor;
    }
}

bool Scene::CheckHit(Ray checkRay, Shape *&hitShape, double &t) {
    bool hit = false;
    double checkingT = 0;
    t = -1;

    // check if we hit a shape
    for (unsigned int i = 0; i < shapes.size(); ++i)
    {
        if ((*shapes[i]).CalculateHit(checkRay, checkingT)) {
            if (checkingT > 0 && (checkingT < t || !hit)) {
                hitShape = shapes[i];
                t = checkingT;
                hit = true;
            }
        }
    }

    return hit;
}
