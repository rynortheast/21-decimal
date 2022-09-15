#include "s21_decimal.h"

void TEST_func_scaleAlignment();

int main() {

    TEST_func_scaleAlignment();

    return 0;
}

void TEST_func_scaleAlignment() {
    s21_decimal TEST_1 = {{5, 0, 0, 0}};
    s21_decimal TEST_2 = {{5, 0, 0, 0}};

    printf("TEST: %d | %d | %u | %u\n", TEST_1.bits[0], TEST_1.bits[1], TEST_1.bits[2], TEST_1.bits[3]);
    printf("TEST: %d | %d | %u | %u\n\n", TEST_2.bits[0], TEST_2.bits[1], TEST_2.bits[2], TEST_2.bits[3]);

    setBit(&TEST_1, 94, 1);
    setBit(&TEST_2, 94, 1);

    setScale(&TEST_2, 2);

    printf("TEST: %d | %d | %u | %u\n", TEST_1.bits[0], TEST_1.bits[1], TEST_1.bits[2], TEST_1.bits[3]);
    printf("TEST: %d | %d | %u | %u\n\n", TEST_2.bits[0], TEST_2.bits[1], TEST_2.bits[2], TEST_2.bits[3]);

    scaleAlignment(&TEST_1, &TEST_2);

    printf("TEST: %d | %d | %u | %u\n", TEST_1.bits[0], TEST_1.bits[1], TEST_1.bits[2], TEST_1.bits[3]);
    printf("TEST: %d | %d | %u | %u\n\n", TEST_2.bits[0], TEST_2.bits[1], TEST_2.bits[2], TEST_2.bits[3]);
}
