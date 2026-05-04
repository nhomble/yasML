#include "unity.h"
#include "../yasML.h"
#include <stdio.h>

#define INPUT_PATH "/tmp/yasml_manual_entry_input.txt"

static void write_input(const char *contents){
    FILE *f = fopen(INPUT_PATH, "w");
    fputs(contents, f);
    fclose(f);
}

/* row-major fill of 2x3 should produce numbers[col][row] correctly */
void test_manual_entry_2x3(){
    Matrix *m = NULL;
    write_input("2 | 3\n1\n2\n3\n4\n5\n6\n");
    freopen(INPUT_PATH, "r", stdin);
    manual_entry(&m);
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_EQUAL_FLOAT(1.0f, (float)m->numbers[0][0]);
    TEST_ASSERT_EQUAL_FLOAT(2.0f, (float)m->numbers[1][0]);
    TEST_ASSERT_EQUAL_FLOAT(3.0f, (float)m->numbers[2][0]);
    TEST_ASSERT_EQUAL_FLOAT(4.0f, (float)m->numbers[0][1]);
    TEST_ASSERT_EQUAL_FLOAT(5.0f, (float)m->numbers[1][1]);
    TEST_ASSERT_EQUAL_FLOAT(6.0f, (float)m->numbers[2][1]);
    destroy_matrix(m);
}

/* multi-char decimal should not be truncated by tiny buffer */
void test_manual_entry_decimal(){
    Matrix *m = NULL;
    write_input("1 | 1\n123.45\n");
    freopen(INPUT_PATH, "r", stdin);
    manual_entry(&m);
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_FLOAT_WITHIN(1e-3f, 123.45f, (float)m->numbers[0][0]);
    destroy_matrix(m);
}

void setUp(void){}
void tearDown(void){}

int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_manual_entry_2x3);
    RUN_TEST(test_manual_entry_decimal);
    return UNITY_END();
}
