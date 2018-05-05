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

inline int in_tri(glm::vec4 &A_, glm::vec4 &B_, glm::vec4 &C_, glm::vec2 &D) {
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


#endif