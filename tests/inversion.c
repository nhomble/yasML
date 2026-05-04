#include "unity.h"
#include "../yasML.h"
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

static void timeout_handler(int sig){
    (void)sig;
    _exit(99);
}

/* zero-pivot in upper-triangle phase triggers unsigned wrap in `j` */
void test_inversion_zero_pivot_terminates(){
    Matrix *m, *inv;
    signal(SIGALRM, timeout_handler);
    alarm(3);
    m = constructor(2, 2);
    m->numbers[0][0] = 0; m->numbers[0][1] = 1;
    m->numbers[1][0] = 1; m->numbers[1][1] = 0;
    inv = inversion(m);
    alarm(0);
    (void)inv;
    destroy_matrix(m);
    if(inv) destroy_matrix(inv);
}

/* M * inversion(M) should equal identity.
   Use [[0,1],[2,0]] (det=-2) which forces a row swap. */
void test_inversion_correct_with_swap(){
    Matrix *m, *m_copy, *inv, *prod;
    m = constructor(2, 2);
    m->numbers[0][0] = 0; m->numbers[0][1] = 2;
    m->numbers[1][0] = 1; m->numbers[1][1] = 0;
    m_copy = clonemx(m);
    inv = inversion(m);
    TEST_ASSERT_NOT_NULL(inv);
    prod = multiply(m_copy, inv);
    TEST_ASSERT_NOT_NULL(prod);
    TEST_ASSERT_FLOAT_WITHIN(1e-5f, 1.0f, (float)prod->numbers[0][0]);
    TEST_ASSERT_FLOAT_WITHIN(1e-5f, 0.0f, (float)prod->numbers[0][1]);
    TEST_ASSERT_FLOAT_WITHIN(1e-5f, 0.0f, (float)prod->numbers[1][0]);
    TEST_ASSERT_FLOAT_WITHIN(1e-5f, 1.0f, (float)prod->numbers[1][1]);
    destroy_matrix(m);
    destroy_matrix(m_copy);
    destroy_matrix(inv);
    destroy_matrix(prod);
}

/* inversion must not mutate the caller's matrix */
void test_inversion_does_not_mutate_input(){
    Matrix *m, *original, *inv;
    m = constructor(2, 2);
    m->numbers[0][0] = 0; m->numbers[0][1] = 2;
    m->numbers[1][0] = 1; m->numbers[1][1] = 0;
    original = clonemx(m);
    inv = inversion(m);
    TEST_ASSERT_EQUAL(SUCC, equals(m, original));
    destroy_matrix(m);
    destroy_matrix(original);
    if(inv) destroy_matrix(inv);
}

/* assert M * inversion(M) == identity */
static void assert_inverse(Matrix *m){
    Matrix *m_copy = clonemx(m);
    Matrix *inv = inversion(m);
    Matrix *prod;
    int n = m->rows;
    int i, j;
    TEST_ASSERT_NOT_NULL(inv);
    prod = multiply(m_copy, inv);
    TEST_ASSERT_NOT_NULL(prod);
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            float expected = (i == j) ? 1.0f : 0.0f;
            TEST_ASSERT_FLOAT_WITHIN(1e-5f, expected, (float)prod->numbers[i][j]);
        }
    }
    destroy_matrix(m_copy);
    destroy_matrix(inv);
    destroy_matrix(prod);
}

/* 3x3 diagonal: inv = diag(1/2, 1/3, 1/5) */
void test_inversion_3x3_diag(){
    Matrix *m = constructor(3, 3);
    m->numbers[0][0] = 2; m->numbers[1][1] = 3; m->numbers[2][2] = 5;
    assert_inverse(m);
    destroy_matrix(m);
}

/* 3x3 generic invertible. row-major: [[1,2,3],[0,1,4],[5,6,0]], det = 1 */
void test_inversion_3x3_generic(){
    Matrix *m = constructor(3, 3);
    m->numbers[0][0] = 1; m->numbers[0][1] = 0; m->numbers[0][2] = 5;
    m->numbers[1][0] = 2; m->numbers[1][1] = 1; m->numbers[1][2] = 6;
    m->numbers[2][0] = 3; m->numbers[2][1] = 4; m->numbers[2][2] = 0;
    assert_inverse(m);
    destroy_matrix(m);
}

/* 4x4 diagonal */
void test_inversion_4x4_diag(){
    Matrix *m = constructor(4, 4);
    double diag[4] = {2.0, 3.0, 5.0, 7.0};
    int i;
    for(i = 0; i < 4; i++)
        m->numbers[i][i] = diag[i];
    assert_inverse(m);
    destroy_matrix(m);
}

/* 4x4 generic invertible */
void test_inversion_4x4_generic(){
    Matrix *m = constructor(4, 4);
    /* row-major:
       [[1,0,0,1],
        [0,2,0,0],
        [0,0,3,0],
        [1,0,0,4]]
       det = ? Block diag-ish. det = 2*3*(1*4 - 1*1) = 6 * 3 = 18 */
    m->numbers[0][0] = 1; m->numbers[0][1] = 0; m->numbers[0][2] = 0; m->numbers[0][3] = 1;
    m->numbers[1][0] = 0; m->numbers[1][1] = 2; m->numbers[1][2] = 0; m->numbers[1][3] = 0;
    m->numbers[2][0] = 0; m->numbers[2][1] = 0; m->numbers[2][2] = 3; m->numbers[2][3] = 0;
    m->numbers[3][0] = 1; m->numbers[3][1] = 0; m->numbers[3][2] = 0; m->numbers[3][3] = 4;
    assert_inverse(m);
    destroy_matrix(m);
}

/* 3x3 needing row swap at i=0 */
void test_inversion_3x3_swap(){
    Matrix *m = constructor(3, 3);
    /* row-major: [[0,1,0],[1,0,0],[0,0,1]] — perm */
    m->numbers[0][0] = 0; m->numbers[0][1] = 1; m->numbers[0][2] = 0;
    m->numbers[1][0] = 1; m->numbers[1][1] = 0; m->numbers[1][2] = 0;
    m->numbers[2][0] = 0; m->numbers[2][1] = 0; m->numbers[2][2] = 1;
    assert_inverse(m);
    destroy_matrix(m);
}

void setUp(void){}
void tearDown(void){}

int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_inversion_zero_pivot_terminates);
    RUN_TEST(test_inversion_correct_with_swap);
    RUN_TEST(test_inversion_does_not_mutate_input);
    RUN_TEST(test_inversion_3x3_diag);
    RUN_TEST(test_inversion_3x3_generic);
    RUN_TEST(test_inversion_4x4_diag);
    RUN_TEST(test_inversion_4x4_generic);
    RUN_TEST(test_inversion_3x3_swap);
    return UNITY_END();
}
