#include "./../s21_decimal.h"

int s21_negate(s21_decimal value, s21_decimal* result) {
  s21_copy(s21_reset(result),
           getSign(value) ? *setSign(&value, 0) : *setSign(&value, 1));
  return 0;
}
