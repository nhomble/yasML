#include "unity.h"
#include "../yasML.h"

void test_projection_null_matrix(){
    double v[2] = {1.0, 2.0};
    TEST_ASSERT_NULL(projection(NULL, v, 2));
}

void test_projection_null_vector(){
    Matrix *m = constructor(2, 2);
    TEST_ASSERT_NULL(projection(m, NULL, 2));
    destroy_matrix(m);
}

void setUp(void){}
void tearDown(void){}

int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_projection_null_matrix);
    RUN_TEST(test_projection_null_vector);
    return UNITY_END();
}
