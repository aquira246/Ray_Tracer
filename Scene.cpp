#include "Scene.hpp"
#include "Ray.hpp"

using namespace std;

Scene::Scene() {
	lights.clear();
	triangles.clear();
	spheres.clear();
    BackgroundColor.r = 0;
    BackgroundColor.g = 0;
    BackgroundColor.b = 0;
    BackgroundColor.f = 1;
}

Scene::~Scene() {
	lights.clear();
	triangles.clear();
	spheres.clear();
}

// TODO
color_t Scene::ShootRayIntoScene(Ray ray, double &t) {
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

color_t Scene::ComputeLighting(Shape *hitShape) {
    color_t ret;

    ret.r = hitShape->mat.rgb(0);
    ret.g = hitShape->mat.rgb(1);
    ret.b = hitShape->mat.rgb(2);
    ret.f = 1;

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

    // check if we hit a triangle
    for (unsigned int i = 0; i < triangles.size(); ++i)
    {
        if (triangles[i].CalculateHit(checkRay, checkingT)) {
            if (checkingT > 0 && (checkingT < t || !hit)) {
                hitShape = &(triangles[i]);
                t = checkingT;
                hit = true;
            }
        }
    }

    return hit;
}