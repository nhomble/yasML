#include "unity.h"
#include "../yasML.h"

void test_gram_schmidt_null(){
    TEST_ASSERT_NULL(gram_schmidt(NULL));
}

void setUp(void){}
void tearDown(void){}

int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_gram_schmidt_null);
    return UNITY_END();
}
