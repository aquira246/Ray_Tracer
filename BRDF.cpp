#include "BRDF.hpp"

using namespace Eigen;
using namespace std;

const double PI = 3.141592653589793;

Eigen::Vector3f BRDF::BlinnPhong(Shape *hitShape, Eigen::Vector3f &hitPt, Eigen::Vector3f &l, Eigen::Vector3f &d) {
    // prepare what is need for blinn-phong
    Eigen::Vector3f h = l + -d;
    h.normalize();
    Eigen::Vector3f n = hitShape->GetNormal(hitPt);


    // make diffuse
    double ndl = max(0.0f, n.dot(l));
    double dif = hitShape->finish.diffuse*ndl;
    
    // make specular
    double hdn = h.dot(n);
    double shine = (1.0f/hitShape->finish.roughness);
    double spec = hitShape->finish.specular*pow(hdn, shine);
    Eigen::Vector3f ret = hitShape->color.head<3>()*(dif + spec);

    return ret;
}


// http://graphicrants.blogspot.com/2013/08/specular-brdf-reference.html
Eigen::Vector3f BRDF::CookTorrance(Shape *hitShape, Eigen::Vector3f &hitPt, Eigen::Vector3f &l, Eigen::Vector3f &d) {
    Eigen::Vector3f v = -d;
    Eigen::Vector3f h = (l + v).normalized();
    Eigen::Vector3f n = (hitShape->GetNormal(hitPt)).normalized();
    float m = hitShape->finish.roughness;
    v.normalize();

    float NdotV= n.dot(v);
    float NdotH= n.dot(h);
    float NdotL= n.dot(l);
    float VdotH= v.dot(h);

    // Calculate Cook-Torrance (G)
    double G1 = (2.0*NdotH*NdotV)/VdotH;
    double G2 = (2.0*NdotH*NdotL)/VdotH;
    float Geom = fmin(1.0, fmin(G2, G1));

    // Calculate GGX (Trowbridge-Reitz) Distribution (D)
    double alpha = m*m;
    double alphaSqr = alpha*alpha;
    double denom = PI*pow(((NdotH*NdotH)*(alphaSqr - 1) + 1), 2);
    float Rough= alphaSqr/denom;

    // Calculate Beckman Distribution (D)
    // double alpha = m*m;
    // double alphaSqr = alpha*alpha;
    // double numr = (NdotH*NdotH - 1)/(alphaSqr*NdotH*NdotH);
    // numr = exp(numr);
    // double denom = PI*alphaSqr*pow(NdotH, 4);
    // float Rough= numr/denom;

    // // Calculate F
    double r1 = 1; // index of refraction for the medium one (like Air,  which is 1)
    double r2 = hitShape->finish.ior; // index of refraction for the medium two (like lead, which is 2)
    float R0 = pow(((r1 - r2)/(r1 + r2)), 2);
    float Fresnel= R0 + (1.0 - R0)*pow(1.0 - VdotH, 5.0);   //schlick's approximation

    double ct = (Rough*Fresnel*Geom)/(4.0*NdotV*NdotL);
    double spec = (hitShape->finish.specular)*ct;

    // calculate diffuse
    double dif = hitShape->finish.diffuse*max(0.0f, NdotL);
    Eigen::Vector3f ret = hitShape->color.head<3>()*(dif + spec);

    return ret;
}

Eigen::Vector3f BRDF::ToonSorta(Shape *hitShape, Eigen::Vector3f &hitPt, Eigen::Vector3f &l, Eigen::Vector3f &d, Eigen::Vector3f *retColor) {
    Eigen::Vector3f baseColor = hitShape->color.head<3>();
    Eigen::Vector3f n = hitShape->GetNormal(hitPt);

    if (-.1 < d.dot(n)) {
        // we are at an edge and should color it black for outlining
        *retColor = Eigen::Vector3f(0,0,0);
        return Eigen::Vector3f(0,0,0);
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

    return ret;
}