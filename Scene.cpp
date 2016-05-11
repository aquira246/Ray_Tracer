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
            // cout << "Sphere\n";
            // cout << sphere->m0 << endl;
            // cout << "inverse: " << endl;
            // cout << sphere->inverseM0 << endl;
            // cout << "transpose: " << endl;
            // cout << sphere->inverseTransposeM0 << endl;
            // cout << "is transformed " << sphere->transformed << endl << endl;
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

Eigen::Vector3f Scene::ShootRayIntoScene(Ray ray, double &t, double prevIOR, double curIOR, int bouncesLeft) {
    Shape *hitShape = NULL;
    Shape *shadowShape = NULL;
    Eigen::Vector3f hitNormal;

    // see if ray hits anything
    if (CheckHit(ray, hitShape, t, hitNormal)) {

        #ifdef UNIT_TEST
        cout << "T = " << t << endl;
        #endif

        // calculate the point the shape was hit at
        Eigen::Vector3f hitPt = ray.position + ray.direction*t;

        // determine if we are inside or outside the object, and set the IORs accordingly
        bool isInside = (-ray.direction).dot(hitNormal) < 0;
        double oldIOR, newIOR;

        if (isInside) {
            // if we are inside, we are leaving the current IOR and going to the previous one
            oldIOR = curIOR;
            newIOR = prevIOR;
            hitNormal = -hitNormal; // also reverse normal
        } else {
            // if we are entering we leaving the current IOR and entering the new one (putting it on top)
            oldIOR = curIOR;
            newIOR = hitShape->finish.ior;
        }

        // set the color to ambient
        Eigen::Vector3f baseColor = hitShape->color.head<3>();
        Eigen::Vector3f retColor = baseColor*hitShape->finish.ambient;

        //check to see if the hit object is in shadow for each light
        for (unsigned int i = 0; i < lights.size(); ++i)
        {    
            // create the shadow ray to check if the spot is in shadow
            Eigen::Vector3f l = lights[i].location - hitPt;
            double lightDistance = l.norm();
            l.normalize();
            Ray shadowRay = Ray(hitPt + l*EPSILON, l);  // epsilon test for shadow ray
            
            // if the spot is not in shadow because there is no object between it and the light
            double s = -1;
            Eigen::Vector3f tempNormal;
            bool inShadow = (CheckHit(shadowRay, shadowShape, s, tempNormal) || s > lightDistance);

            if(!inShadow) {
                
                BRDF brdf = BRDF();
                Eigen::Vector3f lightCol = Eigen::Vector3f(lights[i].color[0], lights[i].color[1], lights[i].color[2]);

                // pick our shader
                if (shader == 0) {
                    retColor += brdf.BlinnPhong(hitShape, hitNormal, l, ray.direction, lightCol);
                } else if (shader == 1) {
                    retColor += brdf.CookTorrance(hitShape, hitNormal, l, ray.direction, lightCol, oldIOR, newIOR);
                } else if (shader == 2) {
                    retColor += brdf.ToonSorta(hitShape, hitNormal, l, ray.direction, lightCol, &retColor);
                } else {
                    cout << "BAD SHADER VALUE! " << shader << "   Default to BlinnPhong" << endl;
                    retColor += brdf.BlinnPhong(hitShape, hitNormal, l, lightCol, ray.direction);
                }
            } else {
                #ifdef UNIT_TEST
                cout << "In shadow" << endl;

                Eigen::Vector3f amb = baseColor*hitShape->finish.ambient;
                cout << "Ambient: (" << amb[0] << ", " << amb[1] << ", " << amb[2] << ")" << endl;
                #endif
            }
        }

        // set the values of the refraction and relfection
        double reflectVal = hitShape->finish.reflection;
        double refractVal = hitShape->color[3];

        // initialize colors
        Eigen::Vector3f reflectColor = Eigen::Vector3f(0,0,0);
        Eigen::Vector3f refractColor = Eigen::Vector3f(0,0,0);

        // if we can still bounce around for reflection and refraction
        if (bouncesLeft > 0) {
            // if we have refraction calculate it
            if (hitShape->finish.refraction > 0) {
                // do refraction
                bool totalReflection = false;
                Ray refrRay = ComputeRefractedRay(hitPt, hitNormal, ray.direction, oldIOR, newIOR, &totalReflection);

                #ifdef UNIT_TEST
                cout << "-------\nIteration type: Refraction" << endl;
                cout << "Ray: {" << refrRay.position[0] << ", " << refrRay.position[1] << ", " << refrRay.position[2] << "} -> {";
                cout << refrRay.direction[0] << ", " << refrRay.direction[1] << ", " << refrRay.direction[2] << "}\n";
                #endif

                float R0 = pow(((oldIOR - newIOR)/(oldIOR + newIOR)), 2);
                float R = R0 + (1.0 - R0)*pow(1.0 - hitNormal.dot(-ray.direction), 5.0);   //schlick's approximation

                double temp = -1;
                double refractT = -1;
                if (!totalReflection) {
                    refractColor = ShootRayIntoScene(refrRay, refractT, oldIOR, newIOR, bouncesLeft - 1);
                } else {
                    R = 0;
                    #ifdef UNIT_TEST
                    cout << "Total Internal Reflection" << endl;
                    #endif
                }

                Ray reflRay = ComputeReflectionRay(hitPt, hitNormal, ray.direction);
                reflectColor = ShootRayIntoScene(reflRay, temp, prevIOR, curIOR, bouncesLeft - 1);

                /* beers law */
                Eigen::Vector3f atten = .15f*baseColor* -fabs(refractT);
                Eigen::Vector3f intensity;
                intensity[0] = expf(atten[0]);
                intensity[1] = expf(atten[1]);
                intensity[2] = expf(atten[2]);

                refractColor[0] = refractColor[0]*intensity[0];
                refractColor[1] = refractColor[1]*intensity[1];
                refractColor[2] = refractColor[2]*intensity[2];

                retColor = R*refractColor + (1 - R)*reflectColor;

                /* previously */
//                retColor = (1 - refractVal - reflectVal)*retColor + refractVal*refractColor + reflectColor*reflectColor;

            }  
            // if we have reflection calculate it
            else if (reflectVal > 0) {
                // do reflection
                Ray reflRay = ComputeReflectionRay(hitPt, hitNormal, ray.direction);
                double temp = -1;

                #ifdef UNIT_TEST
                cout << "-------\nIteration type: Reflection" << endl;
                cout << "Ray: {" << reflRay.position[0] << ", " << reflRay.position[1] << ", " << reflRay.position[2] << "} -> {";
                cout << reflRay.direction[0] << ", " << reflRay.direction[1] << ", " << reflRay.direction[2] << "}\n";
                #endif

                reflectColor = ShootRayIntoScene(reflRay, temp, prevIOR, curIOR, bouncesLeft - 1);
                retColor = retColor*(1 - reflectVal) + reflectVal*reflectColor;

            }
        }

        retColor[0] = fmin(retColor[0], 1.0f);
        retColor[1] = fmin(retColor[1], 1.0f);
        retColor[2] = fmin(retColor[2], 1.0f);

        // compute lighting
        return retColor;
    } else {
        #if UNIT_TEST
        cout << "No Intersection!" << endl;
        #endif

        // return the background color
        return BackgroundColor;
    }
}

bool Scene::CheckHit(Ray &checkRay, Shape *&hitShape, double &t, Eigen::Vector3f &hitNormal) {
    bool hit = false;
    double checkingT = 0;
    t = -1;
    Eigen::Vector3f tempNormal;

    // check if we hit a shape
    for (unsigned int i = 0; i < shapes.size(); ++i)
    {
        if ((*shapes[i]).CalculateHit(checkRay, checkingT, &tempNormal)) {
            if (checkingT > 0 && (checkingT < t || !hit)) {
                hitShape = shapes[i];
                t = checkingT;
                hit = true;
                hitNormal = tempNormal;
            }
        }
    }

    return hit;
}
