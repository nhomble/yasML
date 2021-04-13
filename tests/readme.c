#include "unity.h"
#include "../yasML.h"

void setUp(void){}
void tearDown(void){}

void identity_multiply(void){
  Matrix *i1, *m1, *res;

  i1 = identity(2);
  m1 = constructor(2, 2);
  m1->numbers[0][0] = 1;
  m1->numbers[0][1] = 2;
  m1->numbers[1][0] = 3;
  m1->numbers[1][1] = 4;

  res = multiply(i1, m1);
  print(res);

  TEST_ASSERT_EQUAL(1, equals(m1, res));
}

int main(void){
  UNITY_BEGIN();

  RUN_TEST(identity_multiply);

  return UNITY_END();
}
