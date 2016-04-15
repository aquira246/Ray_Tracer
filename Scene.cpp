#include "Scene.hpp"

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
                    shading = BlinnPhong(hitShape, hitPt, l, ray.direction);
                } else if (shader == 1) {
                    shading = ComputeLighting(hitShape, hitPt, l, ray.direction);
                } else if (shader == 2) {
                    shading = ToonSorta(hitShape, hitPt, l, ray.direction);
                } else {
                    cout << "BAD SHADER VALUE! " << shader << "   Default to BlinnPhong" << endl;
                    shading = BlinnPhong(hitShape, hitPt, l, ray.direction);
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

Eigen::Vector3f Scene::BlinnPhong(Shape *hitShape, Eigen::Vector3f &hitPt, Eigen::Vector3f &l, Eigen::Vector3f &d) {
    Eigen::Vector3f baseColor = hitShape->color.head<3>();

    // prepare what is need for blinn-phong
    Eigen::Vector3f h = l + -d;
    h.normalize();
    Eigen::Vector3f n = hitShape->GetNormal(hitPt);


    // make diffuse
    double ndl = max(0.0f, n.dot(l));
    Eigen::Vector3f dif = baseColor*hitShape->finish.diffuse*ndl;
    
    // make specular
    double hdn = h.dot(n);
    double shine = (1.0f/hitShape->finish.roughness);
    Eigen::Vector3f spec = (baseColor*hitShape->finish.specular)*pow(hdn, shine);

    // add in light color
    Eigen::Vector3f ret = dif + spec;

    return ret;
}

// http://gamedev.stackexchange.com/questions/48804/cook-torrance-model-implementation-black-specular-light
// https://en.wikipedia.org/wiki/Specular_highlight#Ward_anisotropic_distribution
// https://en.wikipedia.org/wiki/Schlick's_approximation
Eigen::Vector3f Scene::ComputeLighting(Shape *hitShape, Eigen::Vector3f &hitPt, Eigen::Vector3f &l, Eigen::Vector3f &d) {
    Eigen::Vector3f baseColor = hitShape->color.head<3>();

    Eigen::Vector3f v = -d;
    Eigen::Vector3f h = l + v;
    h.normalize();
    Eigen::Vector3f n = hitShape->GetNormal(hitPt);
    float m = hitShape->finish.roughness;

    float NdotV= n.dot(v);
    float NdotH= n.dot(h);
    float NdotL= n.dot(l);
    float VdotH= v.dot(h);

    // Calculate G
    double G1 = (2.0*NdotH*NdotV)/VdotH;
    double G2 = (2.0*NdotH*NdotL)/VdotH;
    double Geom= min(1.0, min(G2, G1));

    // Calculate D
    double alpha = acos(NdotH);
    double cosSqrAlpha = pow(cos(alpha), 2);
    double numr = (1 - cosSqrAlpha)/(cosSqrAlpha*m*m);
    numr = exp(-numr);
    double denom = PI*m*m*pow(cos(alpha), 4);
    double Rough= numr/denom;

    // Calculate F
    double r1 = 1; // index of refraction for the medium one (like Air,  which is 1)
    double r2 = 2.01; // index of refraction for the medium two (like lead, which is 2)
    float R0 = pow(((r1 - r2)/(r1 + r2)), 2);
    double Fresnel= R0 + (1.0 - R0)*pow(1.0 - VdotH, 5.0);   //schlick's approximation

    double ct = (Rough*Fresnel*Geom)/(4.0*NdotV*NdotL);
    Eigen::Vector3f spec = (baseColor*hitShape->finish.specular)*ct;

    // if ((baseColor*hitShape->finish.specular).norm() > 0)
    //     cout << "ct: " << ct << "   (" << spec[0] << ", " << spec[1] << ", " << spec[2] << ")" << endl;

    // calculate diffuse
    Eigen::Vector3f dif = baseColor*hitShape->finish.diffuse*max(0.0f, NdotL);

    // add in light color
    Eigen::Vector3f ret = dif + spec;

    return ret;
}

Eigen::Vector3f Scene::ToonSorta(Shape *hitShape, Eigen::Vector3f &hitPt, Eigen::Vector3f &l, Eigen::Vector3f &d) {
    Eigen::Vector3f baseColor = hitShape->color.head<3>();
    bool isEdge = true; // 0 if on edge, and 1 if not
    Eigen::Vector3f n = hitShape->GetNormal(hitPt);

    if (d.dot(n) < -.1) {
        isEdge = false;
    }

    // prepare what is need for blinn-phong
    Eigen::Vector3f h = l + -d;
    h.normalize();

    // make diffuse
    double ndl = max(0.0f, n.dot(l));
    Eigen::Vector3f dif = baseColor*hitShape->finish.diffuse*ndl;
    
    // make specular
    double hdn = h.dot(n);
    double shine = (1.0f/hitShape->finish.roughness);
    Eigen::Vector3f spec = (baseColor*hitShape->finish.specular)*pow(hdn, shine);

    // add in light color
    Eigen::Vector3f ret = dif + spec;

    ret = ret*7;
    ret[0] = ceil(ret[0]);
    ret[1] = ceil(ret[1]);
    ret[2] = ceil(ret[2]);

    ret = ret/7;

    if(isEdge) {
        ret = Eigen::Vector3f(-1, -1, -1);
    }

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
