#ifndef MATRIX3_HPP
#define MATRIX3_HPP

#include "vec3_simd.hpp"
#include <tuple>

using Coord = SimdVec3::Coord;
using Vec3  = SimdVec3;

template <typename T = Coord>
class Matrix {
  public:
    T** elements;
    size_t rows_cnt;
    size_t cols_cnt;
};

template <typename T = Coord>
class SquareMatrix : public Matrix<T>{
  public:
    virtual T det() const;
    virtual SquareMatrix<T>& operator*(const SquareMatrix<T>& B) const;
};

FORCE_INLINE std::tuple<Coord, Coord, Coord> get_3x3_sub_dets(Coord** elements){
    Coord A =  elements[1][1]*elements[2][2] - elements[1][2]*elements[2][1];
    Coord B = -elements[1][0]*elements[2][2] + elements[1][2]*elements[2][0];
    Coord C =  elements[1][0]*elements[2][1] - elements[1][1]*elements[2][0];
    return std::make_tuple(A,B,C);
}

class  __attribute__((aligned (16))) Matrix3 : public SquareMatrix<float> {
  public:
    float det() const override{
        // TODO проверить на overflow
        const auto [A,B,C] = get_3x3_sub_dets(elements);
        __m128 sub_dets = _mm_set_ps(0, C, B, A);
        __m128 coefs = _mm_set_ps(0, elements[0][2], elements[0][1], elements[0][0]);

        return _mm_cvtss_f32(_mm_dp_ps(sub_dets, coefs, 0x71));
    }
    virtual Matrix3& operator*=(const Matrix3& B) const{
        const Matrix3& A = *this;
    }

};

#include <x86intrin.h>
#include <stdio.h>
#include <stdlib.h>

#define N 2048

float *matrix_a;
float *matrix_b;
float result[N][N];

void chunked_mm(int chunk, int n_chunks) {
    __m256 va, vb, vc;
    for (size_t i = chunk*(N/n_chunks); i < (chunk+1)*(N/n_chunks); i++) {
        for (size_t j = 0; j < N; j++) {
            float buffer[8] = {};
            vc = _mm256_loadu_ps(buffer);
            for (int k = 0; k < N; k += 8) {
                // load
                va = _mm256_loadu_ps(matrix_a+(i*N)+k); // matrix_a[i][k]
                vb = _mm256_loadu_ps(matrix_b+(j*N)+k); // matrix_b[j][k]

                // fused multiply and add
                vc = _mm256_fmadd_ps(va, vb, vc);
            }
            //vc = _mm256_hadd_ps(vc, vc);
            _mm256_storeu_ps(buffer, vc);
            result[i][j] = buffer[0] + buffer[1] + buffer[2] + buffer[3] + buffer[4] + buffer[5] + buffer[6] + buffer[7];
            //result[i][j] = buffer[0] + buffer[2] + buffer[4] + buffer[6];
        }
    }
}

int main(int argc, char **argv) {
    // initialize matrix_a and matrix_b
    matrix_a = malloc(N*N*sizeof(float));
    matrix_b = malloc(N*N*sizeof(float));

    for (int i = 0; i < N*N; i++) {
        *(matrix_a+i) = 0.1f;
        *(matrix_b+i) = 0.2f;
    }
    // initialize result matrix
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            result[i][j] = 0.0f;
        }
    }

    #pragma omp parallel for
    for (int i = 0; i < 4; i++) {
        chunked_mm(i, 4);
    }
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            //printf("%f ", result[i][j]);
            printf("%x ", *(unsigned int*)&result[i][j]);
        }
        printf("\n");
    }
    
    return 0;
}


#endif
