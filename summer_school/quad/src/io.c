#include "stdio.h"
#include "enums.h"
#include "io.h"
#include "structs.h"


#define READ_COEFS_NUM_ARGS 3
int read_coefficients(struct Coefs* coefs){
    CHECK_ZERO_ADDR(coefs)
    puts("Input your coefficients a, b, c:");
    if (scanf("%Le %Le %Le", &coefs->a, &coefs->b, &coefs->c) != READ_COEFS_NUM_ARGS){
        puts("Invalid arguments passed");
        return -1;
    }
    return 0;
}

void print_solution(const struct Roots* roots){
    switch (roots->solution_count) {
        case 2:
            printf("Two solutions: %Lf, %Lf\n", roots->x1,roots->x2);
            break;
        case 1:
            printf("One solution: %Lf\n", roots->x1);
            break;
        case 0:
            puts("No solutions!\n");
            break;
        case INF_ROOTS:
            puts("Infinity solutions!\n");
            break;
        case SOLVING_ERROR:
            puts("ERROR: while proccessing solutions");
            break;
        default:
            puts("ERROR: while proccessing solutions default case");
            break;
    }
}
