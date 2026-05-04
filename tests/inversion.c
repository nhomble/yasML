#include "unity.h"
#include "../yasML.h"
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

static void timeout_handler(int sig){
    (void)sig;
    _exit(99);
}

/* zero-pivot in upper-triangle phase triggers unsigned wrap in `j` */
void test_inversion_zero_pivot_terminates(){
    Matrix *m, *inv;
    signal(SIGALRM, timeout_handler);
    alarm(3);
    m = constructor(2, 2);
    m->numbers[0][0] = 0; m->numbers[0][1] = 1;
    m->numbers[1][0] = 1; m->numbers[1][1] = 0;
    inv = inversion(m);
    alarm(0);
    (void)inv;
    destroy_matrix(m);
    if(inv) destroy_matrix(inv);
}

/* M * inversion(M) should equal identity.
   Use [[0,1],[2,0]] (det=-2) which forces a row swap. */
void test_inversion_correct_with_swap(){
    Matrix *m, *m_copy, *inv, *prod;
    m = constructor(2, 2);
    m->numbers[0][0] = 0; m->numbers[0][1] = 2;
    m->numbers[1][0] = 1; m->numbers[1][1] = 0;
    m_copy = clonemx(m);
    inv = inversion(m);
    TEST_ASSERT_NOT_NULL(inv);
    prod = multiply(m_copy, inv);
    TEST_ASSERT_NOT_NULL(prod);
    TEST_ASSERT_FLOAT_WITHIN(1e-5f, 1.0f, (float)prod->numbers[0][0]);
    TEST_ASSERT_FLOAT_WITHIN(1e-5f, 0.0f, (float)prod->numbers[0][1]);
    TEST_ASSERT_FLOAT_WITHIN(1e-5f, 0.0f, (float)prod->numbers[1][0]);
    TEST_ASSERT_FLOAT_WITHIN(1e-5f, 1.0f, (float)prod->numbers[1][1]);
    destroy_matrix(m);
    destroy_matrix(m_copy);
    destroy_matrix(inv);
    destroy_matrix(prod);
}

void setUp(void){}
void tearDown(void){}

int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_inversion_zero_pivot_terminates);
    RUN_TEST(test_inversion_correct_with_swap);
    return UNITY_END();
}
