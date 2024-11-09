#include "BigInt.hpp"


// karatsub algo

void karatsuba(int *a, int *b, int *c, int n) {
    if (n <= 64) {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                c[i + j] += a[i] * b[j];
    } else {
        int k = n / 2;
        int l[k], r[k], t[n] = {0};
        for (int i = 0; i < k; i++) {
            l[i] = a[i] + a[k + i];
            r[i] = b[i] + b[k + i];
        }
        karatsuba(l, r, t, k); // считает t
        karatsuba(a, b, c, k); // считает p1
        karatsuba(a + k, b + k, c + n, k); // считает p2
        int *t1 = t, *t2 = t + k;
        int *s1 = c, *s2 = c + k, *s3 = c + 2 * k, *s4 = c + 3 * k;
        for (int i = 0; i < k; i++) {
            int c1 = s2[i] + t1[i] - s1[i] - s3[i];
            int c2 = s3[i] + t2[i] - s2[i] - s4[i];
            c[k + i] = c1;
            c[n + i] = c2;
        }
    }
}


// MULTIPLICATION
BigInt operator*(const BigInt& a, const BigInt& b){
    
}


// POWER

BigInt operator^(const BigInt& a, UInt power){
    BigInt result = a;
    BigInt mul = a;
    while (power){
        if (power & 1){
            result += mul;
        }
        mul *= UInt(2);
        power >>= 1;
    }
}
