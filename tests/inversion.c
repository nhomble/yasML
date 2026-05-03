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

void setUp(void){}
void tearDown(void){}

int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_inversion_zero_pivot_terminates);
    return UNITY_END();
}
