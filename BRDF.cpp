#include "BRDF.hpp"

using namespace Eigen;
using namespace std;

const double PI = 3.141592653589793;

Eigen::Vector3f BlinnSpec(const Shape *hitShape, const Eigen::Vector3f &n, const Eigen::Vector3f &l, 
                                    const Eigen::Vector3f &d, const Eigen::Vector3f &lightCol) {
    // prepare what is need for blinn-phong
    Eigen::Vector3f h = l + -d;
    h.normalize();

    // make specular
    double hdn = h.dot(n);
    double shine = (1.0f/hitShape->finish.roughness);
    double spec = hitShape->finish.specular*pow(hdn, shine);

    Eigen::Vector3f ret = hitShape->color.head<3>()*spec;

    ret[0] *= lightCol[0];
    ret[1] *= lightCol[1];
    ret[2] *= lightCol[2];

    return ret;
}

Eigen::Vector3f BlinnPhong(const Shape *hitShape, const Eigen::Vector3f &n, const Eigen::Vector3f &l, 
                                    const Eigen::Vector3f &d, const Eigen::Vector3f &lightCol) {
    // prepare what is need for blinn-phong
    Eigen::Vector3f h = (l + -d).normalized();

    // make diffuse
    double dif = hitShape->finish.diffuse*max(0.0f, n.dot(l));
    
    // make specular: specular value * pow(h dot n, shine)
    double spec = hitShape->finish.specular*pow(h.dot(n), (1.0f/hitShape->finish.roughness));

    #ifdef UNIT_TEST
    Eigen::Vector3f specular_color = hitShape->color.head<3>()*spec;

    Eigen::Vector3f diffuse_color = hitShape->color.head<3>()*dif;
    
    Eigen::Vector3f ambient_color = hitShape->color.head<3>()*hitShape->finish.ambient;

    cout << "Ambient: " << "(" << ambient_color(0) << ", " << ambient_color(1) << ", " << ambient_color(2) << ")\n";
    cout << "Diffuse: " << "(" << diffuse_color(0) << ", " << diffuse_color(1) << ", " << diffuse_color(2) << ")\n";
    cout << "Specular: " << "(" << specular_color(0) << ", " << specular_color(1) << ", " << specular_color(2) << ")\n";
    #endif

    Eigen::Vector3f ret = hitShape->color.head<3>()*(dif + spec);
    ret[0] *= lightCol[0];
    ret[1] *= lightCol[1];
    ret[2] *= lightCol[2];


    return ret;
}


// http://graphicrants.blogspot.com/2013/08/specular-brdf-reference.html
Eigen::Vector3f CookTorrance(const Shape *hitShape, const Eigen::Vector3f &n, const Eigen::Vector3f &l, 
                                        const Eigen::Vector3f &d, const Eigen::Vector3f &lightCol, 
                                        double curIOR, double newIOR) {
    Eigen::Vector3f v = -d;
    Eigen::Vector3f h = (l + v).normalized();
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
    double r1 = curIOR; // index of refraction for the medium one (like Air,  which is 1)
    double r2 = newIOR; // index of refraction for the medium two (like lead, which is 2)
    float R0 = pow(((r1 - r2)/(r1 + r2)), 2);
    float Fresnel= R0 + (1.0 - R0)*pow(1.0 - VdotH, 5.0);   //schlick's approximation

    double ct = (Rough*Fresnel*Geom)/(4.0*NdotV*NdotL);
    double spec = (hitShape->finish.specular)*ct;

    // calculate diffuse
    double dif = hitShape->finish.diffuse*max(0.0f, NdotL);

    Eigen::Vector3f ret = hitShape->color.head<3>()*(dif + spec);
    ret[0] *= lightCol[0];
    ret[1] *= lightCol[1];
    ret[2] *= lightCol[2];

    return ret;
}

Eigen::Vector3f ToonSorta(const Shape *hitShape, const Eigen::Vector3f &n, const Eigen::Vector3f &l, 
                                const Eigen::Vector3f &d, const Eigen::Vector3f &lightCol, 
                                Eigen::Vector3f *retColor) {
    Eigen::Vector3f baseColor = hitShape->color.head<3>();

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

    ret[0] *= lightCol[0];
    ret[1] *= lightCol[1];
    ret[2] *= lightCol[2];

    return ret;
}