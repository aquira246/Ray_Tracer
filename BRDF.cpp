#include "BRDF.hpp"

using namespace Eigen;
using namespace std;

const double PI = 3.141592653589793;

Eigen::Vector3f BRDF::BlinnPhong(Shape *hitShape, Eigen::Vector3f &hitPt, Eigen::Vector3f &l, Eigen::Vector3f &d) {
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
    Eigen::Vector3f ret = dif + spec;

    return ret;
}

// http://gamedev.stackexchange.com/questions/48804/cook-torrance-model-implementation-black-specular-light
// https://en.wikipedia.org/wiki/Specular_highlight#Ward_anisotropic_distribution
// https://en.wikipedia.org/wiki/Schlick's_approximation
// http://simonstechblog.blogspot.com/2011/12/microfacet-brdf.html
Eigen::Vector3f BRDF::CookTorrance(Shape *hitShape, Eigen::Vector3f &hitPt, Eigen::Vector3f &l, Eigen::Vector3f &d) {
    Eigen::Vector3f baseColor = hitShape->color.head<3>();

    l.normalize();
    d.normalize();

    Eigen::Vector3f v = -d;
    Eigen::Vector3f h = l + v;
    h.normalize();
    Eigen::Vector3f n = hitShape->GetNormal(hitPt);
    float m = hitShape->finish.roughness;

    v.normalize();
    n.normalize();

    float NdotV= n.dot(v);
    float NdotH= n.dot(h);
    float NdotL= n.dot(l);
    float VdotH= v.dot(h);

    // Calculate G
    double G1 = (2.0*NdotH*NdotV)/VdotH;
    double G2 = (2.0*NdotH*NdotL)/VdotH;
    float Geom = fmin(1.0, fmin(G2, G1));

    // Calculate Beckman Distribution (D)
    double alpha = acos(NdotH);
    double tanAlpha = tan(alpha);
    double numr = (tanAlpha*tanAlpha)/(m*m);
    numr = exp(-numr);
    double denom = PI*m*m*pow(cos(alpha), 4);
    float Rough= numr/denom;

    // // Calculate F
    double r1 = 1; // index of refraction for the medium one (like Air,  which is 1)
    double r2 = hitShape->finish.ior; // index of refraction for the medium two (like lead, which is 2)
    float R0 = pow(((r1 - r2)/(r1 + r2)), 2);
    float Fresnel= R0 + (1.0 - R0)*pow(1.0 - NdotV, 5.0);   //schlick's approximation

    double ct = (Rough*Fresnel*Geom)/(4.0*NdotV*NdotL);
    Eigen::Vector3f spec = (baseColor*hitShape->finish.specular)*ct;

    if (spec.norm() > 0)
        cout << "Spec (" << spec[0] << ", " << spec[1] << ", " << spec[2] << ")" << endl;

    if (isnan(Rough) || isnan(Geom) || isnan(Fresnel)) {
        cout << "The following are Nan: ";
        if (isnan(Rough))
            cout << "Rough ";
        if (isnan(Geom))
            cout << "Geom ";
        if (isnan(Fresnel))
            cout << "Fresnel";

        cout << endl;
    }

    // calculate diffuse
    Eigen::Vector3f dif = baseColor*hitShape->finish.diffuse*max(0.0f, NdotL);
    Eigen::Vector3f ret = dif + spec;

    return ret;
}

Eigen::Vector3f BRDF::ToonSorta(Shape *hitShape, Eigen::Vector3f &hitPt, Eigen::Vector3f &l, Eigen::Vector3f &d) {
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