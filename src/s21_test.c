#include "s21_decimal.h"

int main() {

    s21_decimal TEST_1 = {{5, 0, 0, 0}};
    s21_decimal TEST_2 = {{5, 0, 0, 0}};
    s21_decimal RESULT;

    s21_add(TEST_1, TEST_2, &RESULT);

    printf("TEST: %d", RESULT.bits[0]);

    return 0;
}
