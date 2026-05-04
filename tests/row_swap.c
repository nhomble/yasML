#include "unity.h"
#include "../yasML.h"

void test_row_swap_negative_a(){
    Matrix *m = constructor(2, 2);
    TEST_ASSERT_EQUAL(FAIL, row_swap(m, -1, 0));
    destroy_matrix(m);
}

void test_row_swap_negative_b(){
    Matrix *m = constructor(2, 2);
    TEST_ASSERT_EQUAL(FAIL, row_swap(m, 0, -1));
    destroy_matrix(m);
}

void test_row_swap_valid(){
    Matrix *m = constructor(2, 2);
    m->numbers[0][0] = 1; m->numbers[0][1] = 2;
    m->numbers[1][0] = 3; m->numbers[1][1] = 4;
    TEST_ASSERT_EQUAL(SUCC, row_swap(m, 0, 1));
    TEST_ASSERT_EQUAL_FLOAT(2.0f, (float)m->numbers[0][0]);
    TEST_ASSERT_EQUAL_FLOAT(1.0f, (float)m->numbers[0][1]);
    destroy_matrix(m);
}

void setUp(void){}
void tearDown(void){}

int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_row_swap_negative_a);
    RUN_TEST(test_row_swap_negative_b);
    RUN_TEST(test_row_swap_valid);
    return UNITY_END();
}
