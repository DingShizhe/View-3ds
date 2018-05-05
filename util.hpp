#ifndef UTIL_H
#define UTIL_H

#include "main.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

#define my_length(x, y, z) (Q_rsqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)))
#define X2I(x) ((int) (WIDTH / 2 + x * WIDTH / 2))
#define Y2J(y) ((int) (HEIGHT / 2 - y * WIDTH/2))
#define I2X(i) (((float)(i - (WIDTH / 2)))  / (WIDTH / 2))
#define J2Y(j) (((float)((HEIGHT / 2) - j)) / (WIDTH / 2))

inline float Q_rsqrt( float number ) {
    long i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    y  = number;
    i  = * ( long * ) &y;                       // evil floating point bit level hacking
    i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
    y  = * ( float * ) &i;
    y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
    return 1 / y;
}

inline int in_tri(glm::vec3 &A_, glm::vec3 &B_, glm::vec3 &C_, glm::vec2 &D) {
    glm::vec2 A = glm::vec2(-A_.x / A_.z, -A_.y / A_.z);
    glm::vec2 B = glm::vec2(-B_.x / B_.z, -B_.y / B_.z);
    glm::vec2 C = glm::vec2(-C_.x / C_.z, -C_.y / C_.z);
    glm::vec2 v0 = B - A;
    glm::vec2 v1 = C - A;
    glm::vec2 v2 = D - A;

    float dot00 = dot(v0, v0);
    float dot01 = dot(v0, v1);
    float dot02 = dot(v0, v2);
    float dot11 = dot(v1, v1);
    float dot12 = dot(v1, v2);

    float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    return (u >= 0) && (v >= 0) && (u + v < 1);
}


inline float z_value(float x, float y, glm::vec3 &ABC) {
    float z = 1 / (ABC.z - ABC.x * x - ABC.y * y);
    return -my_length(x * z, y * z, z);
}

inline float f_value (float z_v) {
    float n = ((-z_v) - 3.5) / 15;
    float f_v = 50 + 400 * n;
    return f_v;
}

inline float f_value (
    float lamp_intsty,
    float face_color,
    glm::vec3 &lamp_p,
    glm::vec3 &view_p,
    glm::vec3 &face_p,
    glm::vec3 &face_norm
) {
    float ambientStrength = 0.18f;
    float ambient = ambientStrength * lamp_intsty;

    // Diffuse
    float diffuseStrength = 0.8f;
    glm::vec3 norm = glm::normalize(face_norm);
    glm::vec3 lampDir = glm::normalize(lamp_p - face_p);
    float diff = max(glm::dot(norm, lampDir), (float)0.0);
    float diffuse = diffuseStrength * diff * lamp_intsty;

    // Specular
    float specularStrength = 0.3f;
    glm::vec3 viewDir = glm::normalize(view_p - face_p);
    glm::vec3 reflectDir = glm::reflect(-lampDir, norm);
    float spec = pow(max(glm::dot(viewDir, reflectDir), (float)0.0), 2);
    float specular = specularStrength * spec * lamp_intsty;

    float f_v = (ambient + diffuse + specular) * face_color;
    return f_v;
}

template <class T>
inline void get_from_index (
    T &V0,
    T &V1,
    T &V2,
    T **V_array,
    int m_idx,
    unsigned short *Idx_array,
    int f_idx
) {
    V0 = V_array[m_idx][Idx_array[f_idx*3+0]];
    V1 = V_array[m_idx][Idx_array[f_idx*3+1]];
    V2 = V_array[m_idx][Idx_array[f_idx*3+2]];
}

inline void get_bound_box(
    glm::vec2 prjV0,
    glm::vec2 prjV1,
    glm::vec2 prjV2,
    unsigned &i_min, unsigned &i_max,
    unsigned &j_min, unsigned &j_max
) {
    float v0_x, v0_y, v1_x, v1_y, v2_x, v2_y;

    v0_x = prjV0.x; v0_y = prjV0.y;
    v1_x = prjV1.x; v1_y = prjV1.y;
    v2_x = prjV2.x; v2_y = prjV2.y;

    i_min = max(0, X2I(min(min(v0_x, v1_x), v2_x)));
    j_min = max(0, Y2J(max(max(v0_y, v1_y), v2_y)));  // y axis
    i_max = min(WIDTH, X2I(max(max(v0_x, v1_x), v2_x)));
    j_max = min(HEIGHT, Y2J(min(min(v0_y, v1_y), v2_y)));
}

#endif