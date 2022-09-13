#include "s21_decimal.h"

void TEST_1_1();
void TEST_1_2();

int main() {

    TEST_1_1();

    return 0;
}

void TEST_1_1() {
    s21_decimal TEST_1 = {{5, 0, 0, 0}};
    s21_decimal TEST_2 = {{5, 0, 0, 0}};

    printf("TEST: %d | %d | %u | %u\n", TEST_1.bits[0], TEST_1.bits[1], TEST_1.bits[2], TEST_1.bits[3]);
    printf("TEST: %d | %d | %u | %u\n\n", TEST_2.bits[0], TEST_2.bits[1], TEST_2.bits[2], TEST_2.bits[3]);

    setBit(&TEST_1, 94, 1);
    setBit(&TEST_2, 94, 1);

    setScale(&TEST_2, 2);

    printf("TEST: %d | %d | %u | %u\n", TEST_1.bits[0], TEST_1.bits[1], TEST_1.bits[2], TEST_1.bits[3]);
    printf("TEST: %d | %d | %u | %u\n\n", TEST_2.bits[0], TEST_2.bits[1], TEST_2.bits[2], TEST_2.bits[3]);

    scale_equalize(&TEST_1, &TEST_2);

    printf("TEST: %d | %d | %u | %u\n", TEST_1.bits[0], TEST_1.bits[1], TEST_1.bits[2], TEST_1.bits[3]);
    printf("TEST: %d | %d | %u | %u\n\n", TEST_2.bits[0], TEST_2.bits[1], TEST_2.bits[2], TEST_2.bits[3]);
}

void TEST_1_2() {
    s21_decimal TEST_1 = {{5, 0, 0, 0}};
    s21_decimal TEST_2 = {{5, 0, 0, 0}};

    printf("TEST: %d | %d | %u | %u\n", TEST_1.bits[0], TEST_1.bits[1], TEST_1.bits[2], TEST_1.bits[3]);
    printf("TEST: %d | %d | %u | %u\n\n", TEST_2.bits[0], TEST_2.bits[1], TEST_2.bits[2], TEST_2.bits[3]);

    setBit(&TEST_1, 94, 1);
    setBit(&TEST_2, 94, 1);

    setScale(&TEST_2, 2);

    printf("TEST: %d | %d | %u | %u\n", TEST_1.bits[0], TEST_1.bits[1], TEST_1.bits[2], TEST_1.bits[3]);
    printf("TEST: %d | %d | %u | %u\n\n", TEST_2.bits[0], TEST_2.bits[1], TEST_2.bits[2], TEST_2.bits[3]);

    norm_scale(&TEST_1, &TEST_2);

    printf("TEST: %d | %d | %u | %u\n", TEST_1.bits[0], TEST_1.bits[1], TEST_1.bits[2], TEST_1.bits[3]);
    printf("TEST: %d | %d | %u | %u\n\n", TEST_2.bits[0], TEST_2.bits[1], TEST_2.bits[2], TEST_2.bits[3]);
}
