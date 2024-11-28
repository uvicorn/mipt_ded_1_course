#ifndef VEC3_H
#define VEC3_H

#include <cstddef>
#include <immintrin.h>
#include <iostream>

// #include <experimental/simd>
// namespace stdx = std::experimental;
// using Coord = stdx::native_simd<long double>;

// using Coords = stdx::native_simd<long double>;
// using Coords = stdx::fixed_size_simd<long double, 3>;
// using Coords = std::array<stdx::native_simd<long double>, 3>;

class Vec3 {
    using Coord = long double;
    public:
        union{
            Coord coords[3];
            struct {
                Coord x;
                Coord y;
                Coord z;
            };
        };
        // union {
        //     Vec3D coords_vec2;
        //     Coords coords_vec;
        //     Coords coords;
        // };
    public:
        // ctors
        // Vec3(__m512d&);
        // Vec3(__m512d&&);
        // Vec3(simd_ld4);
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
        Coord dot(const Vec3& rhs) const; // dot product
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
        friend std::ostream& operator<<(std::ostream& out, const Vec3& v);

        static Vec3 random();
        static Vec3 random_normalized();
        static Vec3 random(Coord min, Coord max);
        static Vec3 random_on_hemisphere(const Vec3& normal);
};


#endif
