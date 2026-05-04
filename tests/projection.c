#include "unity.h"
#include "../yasML.h"
#include <math.h>

void test_projection_null_matrix(){
    double v[2] = {1.0, 2.0};
    TEST_ASSERT_NULL(projection(NULL, v, 2));
}

void test_projection_null_vector(){
    Matrix *m = constructor(2, 2);
    TEST_ASSERT_NULL(projection(m, NULL, 2));
    destroy_matrix(m);
}

/* zero column triggers div-by-zero in projection factor */
void test_projection_zero_column(){
    Matrix *m = constructor(2, 2);
    double v[2] = {1.0, 1.0};
    double *r;
    /* col 0 = (0,0), col 1 = (1,0) */
    m->numbers[0][0] = 0; m->numbers[0][1] = 0;
    m->numbers[1][0] = 1; m->numbers[1][1] = 0;
    r = projection(m, v, 2);
    TEST_ASSERT_NOT_NULL(r);
    TEST_ASSERT_TRUE(isfinite(r[0]));
    TEST_ASSERT_TRUE(isfinite(r[1]));
    /* projection onto col 1 = (v . col1)/(col1 . col1) * col1 = 1/1 * (1,0) = (1,0) */
    TEST_ASSERT_EQUAL_FLOAT(1.0f, (float)r[0]);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, (float)r[1]);
    free(r);
    destroy_matrix(m);
}

void setUp(void){}
void tearDown(void){}

int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_projection_null_matrix);
    RUN_TEST(test_projection_null_vector);
    RUN_TEST(test_projection_zero_column);
    return UNITY_END();
}
