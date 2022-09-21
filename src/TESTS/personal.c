#include "./../s21_decimal.h"

int main() {

  s21_decimal TEST = {{1680878458, 3164, 0, 393216}};
  s21_decimal ITOG = {{0, 0, 0, 0}};

  printf("TEST: [%d | %d | %d | %d]\n", TEST.bits[0], TEST.bits[1], TEST.bits[2], TEST.bits[3]);

  s21_truncate(TEST, &ITOG);

  printf("ITOG: [%d | %d | %d | %d]\n", ITOG.bits[0], ITOG.bits[1], ITOG.bits[2], ITOG.bits[3]);

  return 0;
}
