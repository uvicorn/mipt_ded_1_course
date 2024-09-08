#include "solver.h"
#include "enums.h"
#include "io.h"
#include "structs.h"
#include "solver.h"

enum TEST_RESULT{
    FAILED,
    COMPLETED, 
};

#define ASSERT_PRINT(x) if (!(x)) TEST_RES &= FAILED;
#define ASSERT_PRINT(x,case_num) if (!(x)){TEST_RES&=FAILED; printf("FAILED ON %d",case_num);}
#define RUN_TEST_CASE(eq) { \
    if(test_name())printf("COMPLETED"); \
    else printf("FAILED %s on %d testcase", #eq); \
}

#define RUN(test_name) if(test_name())printf("COMPLETED %s", #test_name); else printf("FAILED %s", #test_name);


enum TEST_RESULT test_match_equation_type(){
    int TEST_RES = COMPLETED;
    struct Coefs coefs = {234121, 231321132, 123123};
    ASSERT_PRINT(match_equation_type(&coefs) == QUAD,1)

    struct Coefs coefs2 = {0, 231321132, 123123};
    ASSERT_PRINT(match_equation_type(&coefs2) == LINEAR,2)

    struct Coefs coefs3 = {0, 0, 123123};
    ASSERT_PRINT(match_equation_type(&coefs3) == CONST,3)

    struct Coefs coefs4 = {0, 0, 0};
    ASSERT_PRINT(match_equation_type(&coefs4) == INFINITY_SOL_EQUATION,4)

    return TEST_RES;
}


enum TEST_RESULT test_solve_quadratic_equation(){
    int TEST_RES = COMPLETED;
    struct Roots roots;
    struct Coefs coefs= {1, -2, 1};
    solve_quadratic_equation(&coefs, &roots);
    ASSERT_PRINT(roots.solution_count == 1 && roots.x1  ==  1 && roots.x2  ==  1,1)
    
    struct Roots roots1;
    struct Coefs coefs1= {1, 0, 1};
    solve_quadratic_equation(&coefs1, &roots1);
    ASSERT_PRINT(roots1.solution_count == 0,2)
    
    struct Roots roots2;
    struct Coefs coefs2= {1, 2, -15};
    solve_quadratic_equation(&coefs2, &roots2);
    ASSERT_PRINT(roots2.solution_count == 2 && ((roots2.x1 == 3 && roots2.x2 == -5) || (roots2.x1 == -5 && roots2.x2 == 3)),3)
    return COMPLETED;
}

// заменить на массивы

enum TEST_RESULT test_solve_linear_equation(){
    int TEST_RES = COMPLETED;
    long double x = 0;
    int nRoots = solve_linear_equation(15, 45, &x);
    ASSERT_PRINT(x == -3 && nRoots == 1,1);
    
    nRoots = solve_linear_equation(15, 0, &x);
    ASSERT_PRINT(x == 0 && nRoots == 1,2);
    
    nRoots = solve_linear_equation(0, 0, &x);
    ASSERT_PRINT(nRoots == INF_ROOTS,3);
    
    nRoots = solve_linear_equation(0, 123, &x);
    ASSERT_PRINT(nRoots == 0,4);
    return COMPLETED;
}

int main(){
    RUN(test_match_equation_type)
    RUN(test_solve_quadratic_equation)
    RUN(test_solve_linear_equation)
    return 0;
}
