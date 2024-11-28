#ifndef VEC3_SIMD_H
#define VEC3_SIMD_H

// COPIED FROM
// https://github.com/pelletier/vector3/tree/master
//

#include <malloc.h>
#include <smmintrin.h>
#include <iostream>
#include <cstdlib>


// __m128 bits mask to target the floating point sign bit.
static const __m128 SIGNMASK = _mm_castsi128_ps(_mm_set1_epi32(0x80000000));

class __attribute__((aligned (16))) SimdVec3 {
public:
    using Coord = float;
    /// Constructors
    inline SimdVec3() : mmvalue(_mm_setzero_ps()) {}
    inline SimdVec3(float x, float y, float z) : mmvalue(_mm_set_ps(0, z, y, x)) {}
    inline SimdVec3(__m128 m) : mmvalue(m) {}

    /// Arithmetic operators with Vector3
    inline SimdVec3 operator+(const SimdVec3& b) const { return _mm_add_ps(mmvalue, b.mmvalue); }
    inline SimdVec3 operator-(const SimdVec3& b) const { return _mm_sub_ps(mmvalue, b.mmvalue); }
    inline SimdVec3 operator*(const SimdVec3& b) const { return _mm_mul_ps(mmvalue, b.mmvalue); }
    inline SimdVec3 operator/(const SimdVec3& b) const { return _mm_div_ps(mmvalue, b.mmvalue); }

    /// Assignation and arithmetic operators with Vector3
    inline SimdVec3& operator+=(const SimdVec3& b) { mmvalue = _mm_add_ps(mmvalue, b.mmvalue); return *this; }
    inline SimdVec3& operator-=(const SimdVec3& b) { mmvalue = _mm_sub_ps(mmvalue, b.mmvalue); return *this; }
    inline SimdVec3& operator*=(const SimdVec3& b) { mmvalue = _mm_mul_ps(mmvalue, b.mmvalue); return *this; }
    inline SimdVec3& operator/=(const SimdVec3& b) { mmvalue = _mm_div_ps(mmvalue, b.mmvalue); return *this; }

    /// Arithmetic operators with floats
    inline SimdVec3 operator+(float b) const { return _mm_add_ps(mmvalue, _mm_set1_ps(b)); }
    inline SimdVec3 operator-(float b) const { return _mm_sub_ps(mmvalue, _mm_set1_ps(b)); }
    inline SimdVec3 operator*(float b) const { return _mm_mul_ps(mmvalue, _mm_set1_ps(b)); }
    inline SimdVec3 operator/(float b) const { return _mm_div_ps(mmvalue, _mm_set1_ps(b)); }

    /// Assignation and arithmetic operators with float
    inline SimdVec3& operator+=(float b) { mmvalue = _mm_add_ps(mmvalue, _mm_set1_ps(b)); return *this; }
    inline SimdVec3& operator-=(float b) { mmvalue = _mm_sub_ps(mmvalue, _mm_set1_ps(b)); return *this; }
    inline SimdVec3& operator*=(float b) { mmvalue = _mm_mul_ps(mmvalue, _mm_set1_ps(b)); return *this; }
    inline SimdVec3& operator/=(float b) { mmvalue = _mm_div_ps(mmvalue, _mm_set1_ps(b)); return *this; }


    /// Equality operators
    inline bool operator==(const SimdVec3& b) const {
        return (((_mm_movemask_ps(_mm_cmpeq_ps(mmvalue, b.mmvalue))) & 0x7) == 0x7);
    }
    inline bool operator!=(const SimdVec3& b) const { return !(*this == b); }

    /// Unary minus operator
    inline SimdVec3 operator-() const { return _mm_xor_ps(mmvalue, SIGNMASK); }

    /// Subscript operator
    // Note: there is not bound checking here.
    inline const float& operator[](const int i) const {
        return i == 0 ? this->x : (i == 1 ? this->y : this->z);
    }
    inline float& operator[](const int i) {
        return i == 0 ? this->x : (i == 1 ? this->y : this->z);
    }

    /// Cross product
    inline SimdVec3 cross(const SimdVec3& b) const {
        return _mm_sub_ps(
            _mm_mul_ps(_mm_shuffle_ps(mmvalue, mmvalue, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(b.mmvalue, b.mmvalue, _MM_SHUFFLE(3, 1, 0, 2))),
            _mm_mul_ps(_mm_shuffle_ps(mmvalue, mmvalue, _MM_SHUFFLE(3, 1, 0, 2)), _mm_shuffle_ps(b.mmvalue, b.mmvalue, _MM_SHUFFLE(3, 0, 2, 1)))
            );
    }

    /// Dot product
    inline float dot(const SimdVec3& b) const { return _mm_cvtss_f32(_mm_dp_ps(mmvalue, b.mmvalue, 0x71)); }
    /// Length of the vector
    inline float norm_square() const { return _mm_cvtss_f32((_mm_dp_ps(mmvalue, mmvalue, 0x71))); }
    inline float norm() const { return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(mmvalue, mmvalue, 0x71))); }
    /// Returns the normalized vector
    inline SimdVec3 normalized() const {
        // multiplying by rsqrt does not yield an accurate enough result, so we
        // divide by sqrt instead.
        return _mm_div_ps(mmvalue, _mm_sqrt_ps(_mm_dp_ps(mmvalue, mmvalue, 0xFF)));
    }

    /// Overloaded operators that ensure alignment
    inline void* operator new[](size_t x) { return memalign(16, x); }
    inline void operator delete[](void* x) { free(x);}

    /// Textual representation
    friend std::ostream& operator<<(std::ostream& os, const SimdVec3& t);

    /// Direct access member variables.
    union
    {
        struct { float x, y, z; };
        __m128 mmvalue;
    };

    static SimdVec3 random();
    static SimdVec3 random_normalized();
    static SimdVec3 random(Coord min, Coord max);
    static SimdVec3 random_on_hemisphere(const SimdVec3& normal);
};

/**
** External operators that maps to the actual Vector3 method.
*/
inline SimdVec3 operator+(float a, const SimdVec3& b) { return b + a; }
inline SimdVec3 operator-(float a, const SimdVec3& b) { return SimdVec3(_mm_set1_ps(a)) - b; }
inline SimdVec3 operator*(float a, const SimdVec3& b) { return b * a; }
inline SimdVec3 operator/(float a, const SimdVec3& b) { return SimdVec3(_mm_set1_ps(a)) / b; }

std::ostream& operator<<(std::ostream& os, const SimdVec3& v);
#endif
