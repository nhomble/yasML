#include "unity.h"
#include "../yasML.h"

void setUp(void){}
void tearDown(void){}

void regression_gh_5(void){
    Matrix *m1 = constructor(3, 2);
    Matrix *m2 = constructor(2, 3);
    Matrix *expected = constructor(2, 2);
    expected->numbers[0][0] = 38;
    expected->numbers[0][1] = 101;
    expected->numbers[1][0] = 32;
    expected->numbers[1][1] = 86;

    // going to make this nicer
    int counter = 1;
    for(int i = 0; i < m2->rows; i++){
        for(int j = 0; j < m2->columns; j++){
            m2->numbers[j][i] = counter++;
        }
    }
    counter = 9;
    for(int i = 0; i < m1->rows; i++){
        for(int j = 0; j < m1->columns; j++){
            m1->numbers[j][i] = counter--;
        }
    }
    Matrix *res = multiply(m2, m1);
    
    TEST_ASSERT_EQUAL(1, equals(expected, res));
}

int main(void){
    UNITY_BEGIN();

    RUN_TEST(regression_gh_5);

    return UNITY_END();
}