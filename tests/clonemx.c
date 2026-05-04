#include "unity.h"
#include "../yasML.h"

void test_clonemx_null(){
    TEST_ASSERT_NULL(clonemx(NULL));
}

void test_clonemx_copy(){
    Matrix *m = constructor(2, 2);
    Matrix *c;
    m->numbers[0][0] = 1; m->numbers[0][1] = 2;
    m->numbers[1][0] = 3; m->numbers[1][1] = 4;
    c = clonemx(m);
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_EQUAL(SUCC, equals(m, c));
    /* mutating original should not affect copy */
    m->numbers[0][0] = 99;
    TEST_ASSERT_EQUAL_FLOAT(1.0f, (float)c->numbers[0][0]);
    destroy_matrix(m);
    destroy_matrix(c);
}

void setUp(void){}
void tearDown(void){}

int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_clonemx_null);
    RUN_TEST(test_clonemx_copy);
    return UNITY_END();
}
