#include "unity.h"
#include "../yasML.h"
#include <signal.h>
#include <unistd.h>

static void timeout_handler(int sig){
    (void)sig;
    _exit(99);
}

static void assert_identity(Matrix *r, int n){
    int i, j;
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            float expected = (i == j) ? 1.0f : 0.0f;
            TEST_ASSERT_FLOAT_WITHIN(1e-5f, expected, (float)r->numbers[i][j]);
        }
    }
}

/* 2x2 diag → identity */
void test_solved_aug_2x2_diag(){
    Matrix *m = constructor(2, 2);
    Matrix *r;
    m->numbers[0][0] = 2; m->numbers[0][1] = 0;
    m->numbers[1][0] = 0; m->numbers[1][1] = 3;
    r = solved_aug_matrix(m);
    TEST_ASSERT_NOT_NULL(r);
    assert_identity(r, 2);
    destroy_matrix(m);
    destroy_matrix(r);
}

/* 3x3 upper-triangular → identity
   row-major: [[2,1,0],[0,3,1],[0,0,4]] */
void test_solved_aug_3x3_upper_triangular(){
    Matrix *m = constructor(3, 3);
    Matrix *r;
    m->numbers[0][0] = 2; m->numbers[0][1] = 0; m->numbers[0][2] = 0;
    m->numbers[1][0] = 1; m->numbers[1][1] = 3; m->numbers[1][2] = 0;
    m->numbers[2][0] = 0; m->numbers[2][1] = 1; m->numbers[2][2] = 4;
    r = solved_aug_matrix(m);
    TEST_ASSERT_NOT_NULL(r);
    assert_identity(r, 3);
    destroy_matrix(m);
    destroy_matrix(r);
}

/* swap-required at pivot 0: [[0,1],[1,0]] → identity
   pre-fix this hung (missed swap → upper-phase loop never finds nonzero pivot) */
void test_solved_aug_2x2_swap(){
    Matrix *m = constructor(2, 2);
    Matrix *r;
    signal(SIGALRM, timeout_handler);
    alarm(3);
    m->numbers[0][0] = 0; m->numbers[0][1] = 1;
    m->numbers[1][0] = 1; m->numbers[1][1] = 0;
    r = solved_aug_matrix(m);
    alarm(0);
    TEST_ASSERT_NOT_NULL(r);
    assert_identity(r, 2);
    destroy_matrix(m);
    destroy_matrix(r);
}

/* singular triggers zero pivot in upper-triangle phase → unsigned wrap */
void test_solved_aug_singular_terminates(){
    Matrix *m, *r;
    signal(SIGALRM, timeout_handler);
    alarm(3);
    m = constructor(2, 2);
    /* row-major: [[1,0],[1,0]] — rank 1, singular */
    m->numbers[0][0] = 1; m->numbers[0][1] = 1;
    m->numbers[1][0] = 0; m->numbers[1][1] = 0;
    r = solved_aug_matrix(m);
    alarm(0);
    (void)r;
    destroy_matrix(m);
    if(r) destroy_matrix(r);
}

/* 4x4 diag → identity */
void test_solved_aug_4x4_diag(){
    Matrix *m = constructor(4, 4);
    Matrix *r;
    int i, j;
    double diag[4] = {2.0, 3.0, 5.0, 7.0};
    for(i = 0; i < 4; i++)
        for(j = 0; j < 4; j++)
            m->numbers[i][j] = (i == j) ? diag[i] : 0.0;
    r = solved_aug_matrix(m);
    TEST_ASSERT_NOT_NULL(r);
    assert_identity(r, 4);
    destroy_matrix(m);
    destroy_matrix(r);
}

void setUp(void){}
void tearDown(void){}

int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_solved_aug_2x2_diag);
    RUN_TEST(test_solved_aug_3x3_upper_triangular);
    RUN_TEST(test_solved_aug_2x2_swap);
    RUN_TEST(test_solved_aug_singular_terminates);
    RUN_TEST(test_solved_aug_4x4_diag);
    return UNITY_END();
}
