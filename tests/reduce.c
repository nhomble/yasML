#include "unity.h"
#include "../yasML.h"

void test_reduce_a_equals_rows(){
    Matrix *m = constructor(2, 2);
    TEST_ASSERT_EQUAL(FAIL, reduce(m, 2, 0, 1.0));
    destroy_matrix(m);
}

void test_reduce_b_equals_rows(){
    Matrix *m = constructor(2, 2);
    TEST_ASSERT_EQUAL(FAIL, reduce(m, 0, 2, 1.0));
    destroy_matrix(m);
}

void test_reduce_negative_a(){
    Matrix *m = constructor(2, 2);
    TEST_ASSERT_EQUAL(FAIL, reduce(m, -1, 0, 1.0));
    destroy_matrix(m);
}

void test_reduce_negative_b(){
    Matrix *m = constructor(2, 2);
    TEST_ASSERT_EQUAL(FAIL, reduce(m, 0, -1, 1.0));
    destroy_matrix(m);
}

void test_reduce_valid(){
    Matrix *m = constructor(2, 2);
    m->numbers[0][0] = 1; m->numbers[1][0] = 2;
    m->numbers[0][1] = 3; m->numbers[1][1] = 4;
    TEST_ASSERT_EQUAL(SUCC, reduce(m, 0, 1, 3.0));
    TEST_ASSERT_EQUAL_FLOAT(0.0f, (float)m->numbers[0][1]);
    destroy_matrix(m);
}

void setUp(void){}
void tearDown(void){}

int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_reduce_a_equals_rows);
    RUN_TEST(test_reduce_b_equals_rows);
    RUN_TEST(test_reduce_negative_a);
    RUN_TEST(test_reduce_negative_b);
    RUN_TEST(test_reduce_valid);
    return UNITY_END();
}
