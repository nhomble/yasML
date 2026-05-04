#include "unity.h"
#include "../yasML.h"
#include <math.h>

void test_det_identity(){
    Matrix *m = identity(3);
    TEST_ASSERT_EQUAL_FLOAT(1.0f, (float)determinant(m));
    destroy_matrix(m);
}

void test_det_diag(){
    Matrix *m = constructor(2, 2);
    m->numbers[0][0] = 2; m->numbers[1][1] = 3;
    TEST_ASSERT_EQUAL_FLOAT(6.0f, (float)determinant(m));
    destroy_matrix(m);
}

void test_det_simple_2x2(){
    Matrix *m = constructor(2, 2);
    /* [[1,2],[3,4]] in col-major: numbers[col][row] */
    m->numbers[0][0] = 1; m->numbers[0][1] = 3;
    m->numbers[1][0] = 2; m->numbers[1][1] = 4;
    TEST_ASSERT_EQUAL_FLOAT(-2.0f, (float)determinant(m));
    destroy_matrix(m);
}

/* row-swap required: zero pivot at [0][0] */
void test_det_swap_required(){
    Matrix *m = constructor(2, 2);
    /* [[0,1],[1,0]], det = -1 */
    m->numbers[0][0] = 0; m->numbers[0][1] = 1;
    m->numbers[1][0] = 1; m->numbers[1][1] = 0;
    TEST_ASSERT_EQUAL_FLOAT(-1.0f, (float)determinant(m));
    destroy_matrix(m);
}

void test_det_singular(){
    Matrix *m = constructor(2, 2);
    /* [[1,2],[2,4]], det = 0 */
    m->numbers[0][0] = 1; m->numbers[0][1] = 2;
    m->numbers[1][0] = 2; m->numbers[1][1] = 4;
    TEST_ASSERT_EQUAL_FLOAT(0.0f, (float)determinant(m));
    destroy_matrix(m);
}

/* 3x3 needing swap at i=1: pivot becomes zero mid-elimination
   row-major: [[1,2,3],[2,4,1],[1,1,1]], det = -5 */
void test_det_3x3_mid_swap(){
    Matrix *m = constructor(3, 3);
    m->numbers[0][0] = 1; m->numbers[0][1] = 2; m->numbers[0][2] = 1;
    m->numbers[1][0] = 2; m->numbers[1][1] = 4; m->numbers[1][2] = 1;
    m->numbers[2][0] = 3; m->numbers[2][1] = 1; m->numbers[2][2] = 1;
    TEST_ASSERT_EQUAL_FLOAT(-5.0f, (float)determinant(m));
    destroy_matrix(m);
}

/* two swaps cancel sign: 3-cycle permutation, det = +1
   row-major: [[0,1,0],[0,0,1],[1,0,0]] */
void test_det_double_swap(){
    Matrix *m = constructor(3, 3);
    m->numbers[0][0] = 0; m->numbers[0][1] = 0; m->numbers[0][2] = 1;
    m->numbers[1][0] = 1; m->numbers[1][1] = 0; m->numbers[1][2] = 0;
    m->numbers[2][0] = 0; m->numbers[2][1] = 1; m->numbers[2][2] = 0;
    TEST_ASSERT_EQUAL_FLOAT(1.0f, (float)determinant(m));
    destroy_matrix(m);
}

/* singular hitting early-return: no nonzero pivot in column 0
   row-major: [[0,0],[0,1]] */
void test_det_singular_no_pivot(){
    Matrix *m = constructor(2, 2);
    m->numbers[0][0] = 0; m->numbers[0][1] = 0;
    m->numbers[1][0] = 0; m->numbers[1][1] = 1;
    TEST_ASSERT_EQUAL_FLOAT(0.0f, (float)determinant(m));
    destroy_matrix(m);
}

/* error sentinel must be distinguishable from valid -1 */
void test_det_null_is_nan(){
    TEST_ASSERT_TRUE(isnan(determinant(NULL)));
}

void test_det_non_square_is_nan(){
    Matrix *m = constructor(2, 3);
    TEST_ASSERT_TRUE(isnan(determinant(m)));
    destroy_matrix(m);
}

/* a valid determinant of -1 must NOT collide with the error sentinel */
void test_det_negative_one_valid(){
    Matrix *m = constructor(2, 2);
    /* [[0,1],[1,0]] det = -1 */
    m->numbers[0][0] = 0; m->numbers[0][1] = 1;
    m->numbers[1][0] = 1; m->numbers[1][1] = 0;
    TEST_ASSERT_FALSE(isnan(determinant(m)));
    destroy_matrix(m);
}

void setUp(void){}
void tearDown(void){}

int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_det_identity);
    RUN_TEST(test_det_diag);
    RUN_TEST(test_det_simple_2x2);
    RUN_TEST(test_det_swap_required);
    RUN_TEST(test_det_singular);
    RUN_TEST(test_det_3x3_mid_swap);
    RUN_TEST(test_det_double_swap);
    RUN_TEST(test_det_singular_no_pivot);
    RUN_TEST(test_det_null_is_nan);
    RUN_TEST(test_det_non_square_is_nan);
    RUN_TEST(test_det_negative_one_valid);
    return UNITY_END();
}
