#include "unity.h"
#include "../yasML.h"
#include <math.h>

static double dot(Matrix *m, int a, int b){
    int i;
    double sum = 0;
    for(i = 0; i < m->rows; i++)
        sum += m->numbers[a][i]*m->numbers[b][i];
    return sum;
}

void test_gram_schmidt_null(){
    TEST_ASSERT_NULL(gram_schmidt(NULL));
}

/* independent columns (1,0,0),(1,1,0),(1,1,1): result must be pairwise
   orthogonal and keep all three columns */
void test_gram_schmidt_orthogonalizes(){
    Matrix *m = constructor(3, 3);
    Matrix *o;
    m->numbers[0][0] = 1; m->numbers[0][1] = 1; m->numbers[0][2] = 1;
    m->numbers[1][0] = 0; m->numbers[1][1] = 1; m->numbers[1][2] = 1;
    m->numbers[2][0] = 0; m->numbers[2][1] = 0; m->numbers[2][2] = 1;
    o = gram_schmidt(m);
    TEST_ASSERT_NOT_NULL(o);
    TEST_ASSERT_EQUAL_INT(3, o->columns);
    TEST_ASSERT_FLOAT_WITHIN(1e-9f, 0.0f, (float)dot(o, 0, 1));
    TEST_ASSERT_FLOAT_WITHIN(1e-9f, 0.0f, (float)dot(o, 0, 2));
    TEST_ASSERT_FLOAT_WITHIN(1e-9f, 0.0f, (float)dot(o, 1, 2));
    destroy_matrix(m);
    destroy_matrix(o);
}

/* a matrix containing a zero column has no Gram-Schmidt basis */
void test_gram_schmidt_zero_column_is_null(){
    Matrix *m = constructor(2, 2);
    m->numbers[0][0] = 0; m->numbers[0][1] = 0;
    m->numbers[1][0] = 1; m->numbers[1][1] = 1;
    TEST_ASSERT_NULL(gram_schmidt(m));
    destroy_matrix(m);
}

/* col1 = 2*col0: nonzero but linearly dependent. Previously this
   produced a zero orthogonal vector and NaN downstream; now the
   dependent column is dropped instead of kept as a degenerate zero */
void test_gram_schmidt_drops_dependent_column(){
    Matrix *m = constructor(2, 2);
    Matrix *o;
    m->numbers[0][0] = 1; m->numbers[0][1] = 2;
    m->numbers[1][0] = 2; m->numbers[1][1] = 4;
    o = gram_schmidt(m);
    TEST_ASSERT_NOT_NULL(o);
    TEST_ASSERT_EQUAL_INT(1, o->columns);
    destroy_matrix(m);
    destroy_matrix(o);
}

void test_orthonormal_basis_null(){
    TEST_ASSERT_NULL(orthonormal_basis(NULL));
}

void test_orthonormal_basis_unit_length(){
    Matrix *m = constructor(3, 3);
    Matrix *n;
    int i;
    m->numbers[0][0] = 1; m->numbers[0][1] = 1; m->numbers[0][2] = 1;
    m->numbers[1][0] = 0; m->numbers[1][1] = 1; m->numbers[1][2] = 1;
    m->numbers[2][0] = 0; m->numbers[2][1] = 0; m->numbers[2][2] = 1;
    n = orthonormal_basis(m);
    TEST_ASSERT_NOT_NULL(n);
    for(i = 0; i < n->columns; i++)
        TEST_ASSERT_FLOAT_WITHIN(1e-9f, 1.0f, (float)sqrt(dot(n, i, i)));
    destroy_matrix(m);
    destroy_matrix(n);
}

/* must not deref a NULL gram_schmidt result, and must not divide by
   zero on the dependent column that gram_schmidt now drops */
void test_orthonormal_basis_dependent_columns_no_nan(){
    Matrix *m = constructor(2, 2);
    Matrix *n;
    m->numbers[0][0] = 1; m->numbers[0][1] = 2;
    m->numbers[1][0] = 2; m->numbers[1][1] = 4;
    n = orthonormal_basis(m);
    TEST_ASSERT_NOT_NULL(n);
    TEST_ASSERT_EQUAL_INT(1, n->columns);
    TEST_ASSERT_FALSE(isnan(n->numbers[0][0]));
    TEST_ASSERT_FALSE(isnan(n->numbers[0][1]));
    destroy_matrix(m);
    destroy_matrix(n);
}

void test_orthonormal_basis_zero_column_is_null(){
    Matrix *m = constructor(2, 2);
    m->numbers[0][0] = 0; m->numbers[0][1] = 0;
    m->numbers[1][0] = 1; m->numbers[1][1] = 1;
    TEST_ASSERT_NULL(orthonormal_basis(m));
    destroy_matrix(m);
}

void setUp(void){}
void tearDown(void){}

int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_gram_schmidt_null);
    RUN_TEST(test_gram_schmidt_orthogonalizes);
    RUN_TEST(test_gram_schmidt_zero_column_is_null);
    RUN_TEST(test_gram_schmidt_drops_dependent_column);
    RUN_TEST(test_orthonormal_basis_null);
    RUN_TEST(test_orthonormal_basis_unit_length);
    RUN_TEST(test_orthonormal_basis_dependent_columns_no_nan);
    RUN_TEST(test_orthonormal_basis_zero_column_is_null);
    return UNITY_END();
}
