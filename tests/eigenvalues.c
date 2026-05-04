#include "unity.h"
#include "../yasML.h"

/* diag matrix: REF diagonal == diag entries (no swap needed) */
void test_eigenvalues_diag(){
    Matrix *m = constructor(2, 2);
    double *v;
    m->numbers[0][0] = 2; m->numbers[0][1] = 0;
    m->numbers[1][0] = 0; m->numbers[1][1] = 3;
    v = eigenvalues(m);
    TEST_ASSERT_NOT_NULL(v);
    TEST_ASSERT_EQUAL_FLOAT(2.0f, (float)v[0]);
    TEST_ASSERT_EQUAL_FLOAT(3.0f, (float)v[1]);
    free(v);
    destroy_matrix(m);
}

/* swap-required: [[0,1],[1,0]]. Post-swap REF = identity, diagonal = [1,1].
   Pre-fix the pivot search misses the swap and returns [0,0]. */
void test_eigenvalues_swap_required(){
    Matrix *m = constructor(2, 2);
    double *v;
    m->numbers[0][0] = 0; m->numbers[0][1] = 1;
    m->numbers[1][0] = 1; m->numbers[1][1] = 0;
    v = eigenvalues(m);
    TEST_ASSERT_NOT_NULL(v);
    TEST_ASSERT_EQUAL_FLOAT(1.0f, (float)v[0]);
    TEST_ASSERT_EQUAL_FLOAT(1.0f, (float)v[1]);
    free(v);
    destroy_matrix(m);
}

/* 3x3 upper-triangular: REF diag == matrix diag (no reduction needed)
   row-major: [[2,1,0],[0,3,1],[0,0,4]] */
void test_eigenvalues_3x3_upper_triangular(){
    Matrix *m = constructor(3, 3);
    double *v;
    m->numbers[0][0] = 2; m->numbers[0][1] = 0; m->numbers[0][2] = 0;
    m->numbers[1][0] = 1; m->numbers[1][1] = 3; m->numbers[1][2] = 0;
    m->numbers[2][0] = 0; m->numbers[2][1] = 1; m->numbers[2][2] = 4;
    v = eigenvalues(m);
    TEST_ASSERT_NOT_NULL(v);
    TEST_ASSERT_EQUAL_FLOAT(2.0f, (float)v[0]);
    TEST_ASSERT_EQUAL_FLOAT(3.0f, (float)v[1]);
    TEST_ASSERT_EQUAL_FLOAT(4.0f, (float)v[2]);
    free(v);
    destroy_matrix(m);
}

/* 3x3 needing lower-triangle reduction
   row-major: [[2,4,0],[1,3,0],[0,0,5]], REF diag = [2, 1, 5] */
void test_eigenvalues_3x3_needs_reduction(){
    Matrix *m = constructor(3, 3);
    double *v;
    m->numbers[0][0] = 2; m->numbers[0][1] = 1; m->numbers[0][2] = 0;
    m->numbers[1][0] = 4; m->numbers[1][1] = 3; m->numbers[1][2] = 0;
    m->numbers[2][0] = 0; m->numbers[2][1] = 0; m->numbers[2][2] = 5;
    v = eigenvalues(m);
    TEST_ASSERT_NOT_NULL(v);
    TEST_ASSERT_FLOAT_WITHIN(1e-5f, 2.0f, (float)v[0]);
    TEST_ASSERT_FLOAT_WITHIN(1e-5f, 1.0f, (float)v[1]);
    TEST_ASSERT_FLOAT_WITHIN(1e-5f, 5.0f, (float)v[2]);
    free(v);
    destroy_matrix(m);
}

/* singular: column-0 all-zero. Pivot search finds nothing. REF diag[0] = 0 */
void test_eigenvalues_singular(){
    Matrix *m = constructor(2, 2);
    double *v;
    m->numbers[0][0] = 0; m->numbers[0][1] = 0;
    m->numbers[1][0] = 1; m->numbers[1][1] = 1;
    v = eigenvalues(m);
    TEST_ASSERT_NOT_NULL(v);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, (float)v[0]);
    TEST_ASSERT_EQUAL_FLOAT(1.0f, (float)v[1]);
    free(v);
    destroy_matrix(m);
}

void test_eigenvalues_null(){
    TEST_ASSERT_NULL(eigenvalues(NULL));
}

void test_eigenvalues_non_square(){
    Matrix *m = constructor(2, 3);
    TEST_ASSERT_NULL(eigenvalues(m));
    destroy_matrix(m);
}

void setUp(void){}
void tearDown(void){}

int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_eigenvalues_diag);
    RUN_TEST(test_eigenvalues_swap_required);
    RUN_TEST(test_eigenvalues_3x3_upper_triangular);
    RUN_TEST(test_eigenvalues_3x3_needs_reduction);
    RUN_TEST(test_eigenvalues_singular);
    RUN_TEST(test_eigenvalues_null);
    RUN_TEST(test_eigenvalues_non_square);
    return UNITY_END();
}
