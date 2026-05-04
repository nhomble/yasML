#include "unity.h"
#include "../yasML.h"

/* exact-equal matrices */
void test_equals_exact(){
    Matrix *a = constructor(2, 2);
    Matrix *b = constructor(2, 2);
    a->numbers[0][0] = 1; a->numbers[0][1] = 2;
    a->numbers[1][0] = 3; a->numbers[1][1] = 4;
    b->numbers[0][0] = 1; b->numbers[0][1] = 2;
    b->numbers[1][0] = 3; b->numbers[1][1] = 4;
    TEST_ASSERT_EQUAL(SUCC, equals(a, b));
    destroy_matrix(a);
    destroy_matrix(b);
}

/* matrices that differ → FAIL */
void test_equals_different(){
    Matrix *a = constructor(1, 1);
    Matrix *b = constructor(1, 1);
    a->numbers[0][0] = 1.0;
    b->numbers[0][0] = 2.0;
    TEST_ASSERT_EQUAL(FAIL, equals(a, b));
    destroy_matrix(a);
    destroy_matrix(b);
}

/* FP noise: 0.1 + 0.2 != 0.3 exactly, but should compare equal */
void test_equals_fp_noise(){
    Matrix *a = constructor(1, 1);
    Matrix *b = constructor(1, 1);
    a->numbers[0][0] = 0.1 + 0.2;
    b->numbers[0][0] = 0.3;
    TEST_ASSERT_EQUAL(SUCC, equals(a, b));
    destroy_matrix(a);
    destroy_matrix(b);
}

/* dimension mismatch → FAIL */
void test_equals_dim_mismatch(){
    Matrix *a = constructor(2, 2);
    Matrix *b = constructor(2, 3);
    TEST_ASSERT_EQUAL(FAIL, equals(a, b));
    destroy_matrix(a);
    destroy_matrix(b);
}

/* NULL → FAIL */
void test_equals_null(){
    Matrix *a = constructor(2, 2);
    TEST_ASSERT_EQUAL(FAIL, equals(NULL, a));
    TEST_ASSERT_EQUAL(FAIL, equals(a, NULL));
    TEST_ASSERT_EQUAL(FAIL, equals(NULL, NULL));
    destroy_matrix(a);
}

void setUp(void){}
void tearDown(void){}

int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_equals_exact);
    RUN_TEST(test_equals_different);
    RUN_TEST(test_equals_fp_noise);
    RUN_TEST(test_equals_dim_mismatch);
    RUN_TEST(test_equals_null);
    return UNITY_END();
}
