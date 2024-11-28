#include "vec3.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include "utils/rand.hpp"


// ctor
// Vec3::Vec3(__m512d& vec){
//     _mm512_storeu_pd(this->coords, vec);
// }
using Coord = Vec3::Coord;

Vec3::Vec3(Coord x, Coord y, Coord z){
    this->coords[0] = x;
    this->coords[1] = y;
    this->coords[2] = z;
}

// add sub 
Vec3& Vec3::operator+=(const Vec3& other){
    // this->coords_vec += other.coords_vec;

    // this->coords_vec2 += other.coords_vec2;
    // this->coords_vec2[0] += other.coords_vec2[0];
    // this->coords_vec2[1] += other.coords_vec2[1];
    // this->coords_vec2[2] += other.coords_vec2[2];

    this->coords[0] += other.coords[0];
    this->coords[1] += other.coords[1];
    this->coords[2] += other.coords[2];
    return *this;

    // __m512d vec_a = _mm512_loadu_pd(this->coords);
    // __m512d vec_b = _mm512_loadu_pd(other.coords);

    // __m512d vec_sum = _mm512_add_pd(vec_a, vec_b);
    // _mm512_storeu_pd(this->coords, vec_sum);
    // return *this;
}

Vec3& Vec3::operator-=(const Vec3& other){
    this->coords[0] -= other.coords[0];
    this->coords[1] -= other.coords[1];
    this->coords[2] -= other.coords[2];
    return *this;

    // __m512d vec_a = _mm512_loadu_pd(this->coords);
    // __m512d vec_b = _mm512_loadu_pd(other.coords);

    // __m512d vec_diff = _mm512_sub_pd(vec_a, vec_b);
    // _mm512_storeu_pd(this->coords, vec_diff);
    // return *this;
}



Vec3 Vec3::operator+(const Vec3& b) const{
    Vec3 copy = *this;
    return copy += b;
}

Vec3 Vec3::operator-(const Vec3& b) const{
    Vec3 copy = *this;
    return copy -= b;
}

// unary minus
Vec3 Vec3::operator-() const {
    return *this * -1;
    // __m512d new_coords = _mm512_loadu_pd(this->coords);
    // new_coords = _mm512_sub_pd(_mm512_set1_pd(0.0), new_coords);

    // return Vec3(new_coords);
}

// mul
// Vec3& Vec3::operator*=(const Vec3& other){
//     *this = Vec3(
//         coords[0] * other.coords[0],
//         coords[1] * other.coords[1],
//         coords[2] * other.coords[2]
//     );
//     return *this;
// }

// Vec3 Vec3::operator*(const Vec3& other) const{
//     Vec3 copy = *this;
//     return copy *= other;
// }

// scaral mul
Vec3& Vec3::operator*=(Coord lambda){
    *this = Vec3(
        coords[0] * lambda,
        coords[1] * lambda,
        coords[2] * lambda
    );
    return *this;
}

Vec3 Vec3::operator*(Coord lambda) const{
    Vec3 copy = *this;
    return copy *= lambda;
}

Vec3 operator*(Coord lambda, const Vec3& rhs){
    return rhs * lambda;
}

// scalar division
Vec3& Vec3::operator/=(Coord lambda){
    *this = Vec3(
        coords[0] / lambda,
        coords[1] / lambda,
        coords[2] / lambda
    );
    return *this;
}

Vec3 Vec3::operator/(Coord lambda) const{
    Vec3 copy = *this;
    return copy /= lambda;
}


// vector ops

Vec3 Vec3::cross(const Vec3& rhs) const{
    return Vec3(this->coords[1] * rhs.coords[2] - this->coords[2] * rhs.coords[1],
                this->coords[2] * rhs.coords[0] - this->coords[0] * rhs.coords[2],
                this->coords[0] * rhs.coords[1] - this->coords[1] * rhs.coords[0]);
}

// Coord Vec3::dot(const Vec3& rhs) const{
//     return (
//         + coords[0] * rhs.coords[0]
//         + coords[1] * rhs.coords[1]
//         + coords[2] * rhs.coords[2]
//     );
// }

Coord Vec3::dot(const Vec3& rhs) const {
    return (
        + coords[0] * rhs.coords[0]
        + coords[1] * rhs.coords[1]
        + coords[2] * rhs.coords[2]
    );
}

// get coords
Coord Vec3::operator[](size_t index) const{
    assert(index < 3 && index >= 0);
    return this->coords[index];
}

// norm ops
Coord Vec3::norm() const {
    return std::sqrt(coords[0]*coords[0] + coords[1]*coords[1] + coords[2]*coords[2]);
}
Coord Vec3::norm_square() const {
    return (coords[0]*coords[0] + coords[1]*coords[1] + coords[2]*coords[2]);
}

Vec3 Vec3::normalized() const {
    return *this / this->norm();
}

// output

std::ostream& operator<<(std::ostream& out, const Vec3& v) {
    return out << v.coords[0] << ' ' << v.coords[1] << ' ' << v.coords[2];
}

// random

Vec3 Vec3::random(){
    return Vec3(random_coord(), random_coord(), random_coord());
}

Vec3 Vec3::random_normalized(){
    while (true) {
        auto p = Vec3::random(-1,1);
        auto lensq = p.norm_square();
        if (1e-160 < lensq && lensq <= 1)
            return p / std::sqrt(lensq);
    }
    // return Vec3::random(-1, 1).normalized();
}

Vec3 Vec3::random(Coord min, Coord max){
    return Vec3(random_coord(min,max), random_coord(min,max), random_coord(min,max));
}

Vec3 Vec3::random_on_hemisphere(const Vec3& normal){
    Vec3 on_unit_sphere = Vec3::random_normalized();
    if (on_unit_sphere.dot(normal) > 0.0)
        return on_unit_sphere;
    return -on_unit_sphere;
    // return (on_unit_sphere * normal > 0.0) ?
    //     on_unit_sphere : -on_unit_sphere; // TODO: тут может быть проблема
}
