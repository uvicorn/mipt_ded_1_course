#ifndef VEC3_H
#define VEC3_H

#include <cstddef>
#include <immintrin.h>
#include <iostream>

// TODO: проверить хуйню
// using Coord = long double __attribute__((vector_size(16)));
using Coord = long double;
// typedef long double Coord __attribute__ ((vector_size (16)));

class Vec3 {
    public:
        Coord coords[3];
    public:
        // ctors
        Vec3(__m512d&);
        Vec3(__m512d&&);
        Vec3(Coord, Coord, Coord);
        Vec3() : coords{0,0,0} {}
        // add
        Vec3& operator+=(const Vec3& other);
        Vec3 operator+(const Vec3& b) const;
        // sub
        Vec3& operator-=(const Vec3& other);
        Vec3 operator-(const Vec3& b) const;
        Vec3 operator-() const;

        // mul
        // Vec3& operator*=(const Vec3& other); 
        Coord operator*(const Vec3& rhs) const; // dot product
        // Vec3 operator*(const Vec3& b) const;

        //scalar mul
        Vec3& operator*=(Coord lambda);
        Vec3 operator*(Coord lambda) const;
        friend Vec3 operator*(Coord lambda, const Vec3& rhs);

        //scalar div
        Vec3& operator/=(Coord lambda);
        Vec3 operator/(Coord lambda) const;
        //vector ops
        Vec3 cross(const Vec3& rhs) const;
        // Coord dot(const Vec3& rhs) const;

        // get coords
        Coord operator[](size_t index) const;
        Coord norm() const;
        Coord norm_square() const;
        Vec3 normalized() const;

        // output
        Coord x() const;
        Coord y() const;
        Coord z() const;
        friend std::ostream& operator<<(std::ostream& out, const Vec3& v);
};


inline Coord Vec3::x() const { return coords[0]; }
inline Coord Vec3::y() const { return coords[1]; }
inline Coord Vec3::z() const { return coords[2]; }
// inline vec3 operator/(const vec3& v, double t) {
//     return (1/t) * v;
// }
// 
// 
// inline vec3 unit_vector(const vec3& v) {
//     return v / v.length();
// }
#endif
