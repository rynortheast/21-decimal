#include "./../s21_decimal.h"

int main() {
  s21_decimal TEST = {{0, 0, 0, -2147418112}};
  s21_decimal ITOG = {{0, 0, 0, 0}};

  printf("TEST: [%d | %d | %d | %d] = [scale = %d]\n", TEST.bits[0],
         TEST.bits[1], TEST.bits[2], TEST.bits[3], getScale(TEST));

  s21_round(TEST, &ITOG);

  printf("ITOG: [%d | %d | %d | %d] = [scale = %d]\n", ITOG.bits[0],
         ITOG.bits[1], ITOG.bits[2], ITOG.bits[3], getScale(ITOG));

  return 0;
}
