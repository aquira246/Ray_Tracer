#include "Scene.hpp"
#include "BRDF.hpp"
#include "BVH.hpp"

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

   scene.bvh = BVH(scene.shapes);
   scene.bvh.Init(0);

   cout << "Triangles: " << scene.triangles.size() << endl;
   cout << "Spheres: " << scene.spheres.size() << endl;
   cout << "Planes: " << scene.planes.size() << endl;
   cout << "Boxes: " << scene.boxes.size() << endl;
   cout << "Lights: " << scene.lights.size() << endl;
   cout << "Camera: " << scene.cameras.size() << endl;

   return numObjects;
}

void Scene::ReflectionAndRefraction(Shape *hitShape, Vector3f &retColor, const Vector3f &dir, 
                                    const Vector3f &hitPt, Vector3f &hitNormal,  
                                    int bouncesLeft, double prevIOR, double oldIOR, double newIOR) {
    // set the values of the refraction and relfection
    double reflectVal = hitShape->finish.reflection;

    // do reflection
    double temp = -1;
    Ray reflRay = ComputeReflectionRay(hitPt, hitNormal, dir);
    Eigen::Vector3f reflectColor = ShootRayIntoScene(reflRay, temp, prevIOR, oldIOR, bouncesLeft - 1, 0);

    // if we have refraction calculate it
    if (hitShape->finish.refraction > 0) {
        // do refraction
        bool totalReflection = false;
        Ray refrRay = ComputeRefractedRay(hitPt, hitNormal, dir, oldIOR, newIOR, &totalReflection);

        if (!totalReflection) {
            //schlick's approximation
            float R0 = pow(((oldIOR - newIOR)/(oldIOR + newIOR)), 2);
            float cosX = (-dir).dot(hitNormal);
            
            // if the oldIOR > newIOR then the old schlick's approx doesn't work
            if (oldIOR > newIOR) {
                const double iorDiv = oldIOR/newIOR;
                const double sinT2 = iorDiv*iorDiv*(1 - cosX*cosX);
                cosX = sqrt(1.0 - sinT2);
            }

            float R = R0 + (1.0 - R0)*pow(1.0 - cosX, 5.0);

            // calculate refraction color
            Eigen::Vector3f refractColor = ShootRayIntoScene(refrRay, temp, oldIOR, newIOR, bouncesLeft - 1, 0);
            retColor = (1 - R)*refractColor + (R)*reflectColor;
        } else {
            // total internal reflection
            retColor = reflectColor;
        }
    }  
    else if (reflectVal > 0) {
        // if we only have reflection then calculate it
        retColor = retColor*(1 - reflectVal) + reflectVal*reflectColor;
    }
}

Eigen::Vector3f Scene::ShootRayIntoScene(const Ray &ray, double &t, double prevIOR, double curIOR, int bouncesLeft, int GIBounces) {
    // const hitShape
    Shape *hitShape = NULL;
    Eigen::Vector3f hitNormal;

    // see if ray hits anything
    if (CheckHit(ray, hitShape, t, hitNormal)) {

        #ifdef UNIT_TEST
        cout << "T = " << t << endl;
        #endif

        // calculate the point the shape was hit at
        Eigen::Vector3f hitPt = ray.position + ray.direction*t;

        // determine if we are inside or outside the object, and set the IORs accordingly
        double oldIOR = curIOR, newIOR;

        if ((-ray.direction).dot(hitNormal) < 0) {
            // if we are inside, we are leaving the current IOR and going to the previous one
            newIOR = prevIOR;
            hitNormal = -hitNormal; // also reverse normal
        } else {
            // if we are entering we leaving the current IOR and entering the new one (putting it on top)
            newIOR = hitShape->finish.ior;
        }

//////////////////////////////////// SET THE AMBIENT COLOR //////////////////////////////////////////////////////
        Eigen::Vector3f retColor(0,0,0);

        // if we can go deeper into global illumination and we are not doing refraction (which ignores this anyway)
        if (GIBounces > 0 && hitShape->finish.refraction == 0) {
            // setup the initial GIray
            Ray giRay = Ray(hitPt, Vector3f(0,0,0));

            // calculate the number of samples we are taking. On the second layer cut the samples in half
            int sample_size = GIBounces > 1 ? GI_SAMPLE_SIZE : GI_SMALL_SAMPLE_SIZE;

            double holdt = 0;

            // for each sample, we shoot out a ray to calculate the color and add it to retColor
            for (int i = 0; i < sample_size; ++i)
            {
                giRay.direction = ComputeGIRay(hitNormal);
                giRay.position = hitPt + giRay.direction*EPSILON;
                retColor += ShootRayIntoScene(giRay, holdt, prevIOR, curIOR, bouncesLeft, GIBounces - 1);
            }

            // divide retColor by the sample size to average it
            retColor /= sample_size;
        } else {
            retColor = hitShape->color.head(3)*hitShape->finish.ambient;
        }

/////////////////////////////////////ADD DIRECT LIGHTING ////////////////////////////////////////////
        //check to see if the hit object is in shadow for each light
        for (unsigned int i = 0; i < lights.size(); ++i)
        {    
            // create the shadow ray to check if the spot is in shadow
            Eigen::Vector3f l = lights[i].location - hitPt;
            double lightDistance = l.norm();
            l.normalize();
            Ray shadowRay = Ray(hitPt + l*EPSILON, l);  // epsilon test for shadow ray
            
            // if the spot is not in shadow because there is no object between it and the light
            float softShadowPercentage = 0;
            // if (GIBounces == 0) {
                if (!ShadowHit(shadowRay, lightDistance))
                    softShadowPercentage = 1;
            // } else {
            //     softShadowPercentage = SoftShadowValue(shadowRay, lightDistance);
            // }

            if(softShadowPercentage > EPSILON) {
                Eigen::Vector3f lightCol = lights[i].color.head<3>();

                // pick our shader
                if (shader == 0) {
                    retColor += BlinnPhong(hitShape, hitNormal, l, ray.direction, lightCol)*softShadowPercentage;
                } else if (shader == 1) {
                    retColor += CookTorrance(hitShape, hitNormal, l, ray.direction, lightCol, oldIOR, newIOR)*softShadowPercentage;
                } else if (shader == 2) {
                    retColor += ToonSorta(hitShape, hitNormal, l, ray.direction, lightCol, &retColor)*softShadowPercentage;
                } else {
                    cout << "BAD SHADER VALUE! " << shader << "   Default to BlinnPhong" << endl;
                    retColor += BlinnPhong(hitShape, hitNormal, l, lightCol, ray.direction)*softShadowPercentage;
                }
            } 
        }

////////////////////////////////////////ADD REFLECTION AND REFRACTION ///////////////////////////////////////////////
        // if we can still bounce around for reflection and refraction
        if (bouncesLeft > 0)
            ReflectionAndRefraction(hitShape, retColor, ray.direction, hitPt, hitNormal, bouncesLeft, prevIOR, oldIOR, newIOR);

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

float Scene::SoftShadowValue(const Ray &checkRay, double lightDistance) {
    Eigen::Vector3f shadowPos = checkRay.position;
    Eigen::Vector3f shadowDir = checkRay.direction;
    float hits = 0;

    // setup the initial GIray
    Ray shadowRay = Ray(shadowPos, shadowDir);

    // for each sample, we shoot out a ray to calculate the color and add it to retColor
    for (int i = 0; i < 32; ++i)
    {
        shadowRay.direction = ComputeGIRay(shadowDir);
        shadowRay.position = shadowPos + shadowRay.direction;
        if (ShadowHit(shadowRay, lightDistance)) {
            ++hits;
        }
    }

    // divide retColor by the sample size to average it
    hits /= 32.0f;
    return hits;
}

bool Scene::ShadowHit(const Ray &checkRay, double lightDistance) {
    double checkingT = 0;
    Shape *shadowShape;

    // check if our BVH hit
    if (bvh.checkShadowHit(checkRay, lightDistance)) {
            return true;
    }

    // check if we hit a plane
    for (unsigned int i = 0; i < planes.size(); ++i)
    {
        if (planes[i].CalculateHit(checkRay, checkingT, shadowShape)) {
            if (checkingT > 0 && checkingT < lightDistance) {
                return true;
            }
        }
    }

    return false;
}

bool Scene::CheckHit(const Ray &checkRay, Shape *&hitShape, double &t, Eigen::Vector3f &hitNormal) {
    bool hit = false;
    double checkingT = 0;
    t = -1;
    Shape *testShape;

    // check if our BVH hit
    if (bvh.checkHit(checkRay, checkingT, testShape)) {
        t = checkingT;
        hit = true;
        hitShape = testShape;
    }

    // check if we hit a plane
    for (unsigned int i = 0; i < planes.size(); ++i)
    {
        if (planes[i].CalculateHit(checkRay, checkingT, testShape)) {
            if (checkingT > 0 && (checkingT < t || !hit)) {
                t = checkingT;
                hit = true;
                hitShape = testShape;
            }
        }
    }

    if (hit) {
        hitShape->GetNormal(checkRay, &hitNormal, t);

        #ifdef UNIT_TEST
        Eigen::Vector3f hitPt = checkRay.position + checkRay.direction*t;
        cout << "Hit Object t: " << t;
        cout << "    Intersection point: {" << hitPt(0) << ", " << hitPt(1) << ", " << hitPt(2) << "}\n";
        cout << "Normal: {" << hitNormal(0) << ", " << hitNormal(1) << ", " << hitNormal(2) << "}\n";
        #endif
    }

    return hit;
}
