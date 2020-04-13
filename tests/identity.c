#include "unity.h"
#include "../yasML.h"

void test_identity_equality(){
    Matrix *m1;
    Matrix *m2;
    m1 = identity(2);
    m2 = identity(2);

    TEST_ASSERT_EQUAL(1, equals(m1, m2));

    free(m1);
    free(m2);
}

void test_multiply_identity() {
    Matrix *m1, *m2, *m3;
    m1 = identity(2);
    m2 = identity(2);

    m3 = multiply(m1, m2);

    TEST_ASSERT_EQUAL(1, equals(m1, m3));

    free(m1);
    free(m2);
    free(m3);
}

void setUp(void){}
void tearDown(void){}

int main(void){
    UNITY_BEGIN();

    RUN_TEST(test_identity_equality);
    RUN_TEST(test_multiply_identity);

    return UNITY_END();
}
