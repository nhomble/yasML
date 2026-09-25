#include "unity.h"
#include "../yasML.h"

/* 2x2 matrix with a zero column should be detected */
void test_zero_vector_2x2(){
    Matrix *m = constructor(2, 2);
    m->numbers[0][0] = 0;
    m->numbers[0][1] = 0;
    m->numbers[1][0] = 1;
    m->numbers[1][1] = 2;
    TEST_ASSERT_EQUAL(SUCC, zero_vector(m));
    destroy_matrix(m);
}

/* 4x4 matrix with a zero column should be detected */
void test_zero_vector_4x4(){
    Matrix *m = constructor(4, 4);
    int i, j;
    for(i = 0; i < 4; i++)
        for(j = 0; j < 4; j++)
            m->numbers[i][j] = (i == 2) ? 0 : 1;
    TEST_ASSERT_EQUAL(SUCC, zero_vector(m));
    destroy_matrix(m);
}

/* matrix with no zero column should not be detected */
void test_zero_vector_none(){
    Matrix *m = constructor(2, 2);
    m->numbers[0][0] = 1;
    m->numbers[0][1] = 2;
    m->numbers[1][0] = 3;
    m->numbers[1][1] = 4;
    TEST_ASSERT_EQUAL(0, zero_vector(m));
    destroy_matrix(m);
}

/* near-zero column (FP noise) should be treated as zero */
void test_zero_vector_fp_noise(){
    Matrix *m = constructor(2, 2);
    m->numbers[0][0] = 1e-15;
    m->numbers[0][1] = 1e-15;
    m->numbers[1][0] = 1;
    m->numbers[1][1] = 2;
    TEST_ASSERT_EQUAL(SUCC, zero_vector(m));
    destroy_matrix(m);
}

/* FAIL is -1, which is truthy in C: zero_vector() must return a plain
   0 for "no zero column", or this idiom always takes the if-branch */
void test_zero_vector_if_idiom(){
    Matrix *m = constructor(2, 2);
    m->numbers[0][0] = 1;
    m->numbers[0][1] = 2;
    m->numbers[1][0] = 3;
    m->numbers[1][1] = 4;
    if(zero_vector(m))
        TEST_FAIL_MESSAGE("if(zero_vector(m)) took the true branch with no zero column");
    destroy_matrix(m);
}

void setUp(void){}
void tearDown(void){}

int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_zero_vector_2x2);
    RUN_TEST(test_zero_vector_4x4);
    RUN_TEST(test_zero_vector_none);
    RUN_TEST(test_zero_vector_fp_noise);
    RUN_TEST(test_zero_vector_if_idiom);
    return UNITY_END();
}
