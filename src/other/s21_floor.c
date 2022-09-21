#include "./../s21_decimal.h"

int s21_floor(s21_decimal value, s21_decimal* result) {
  if (!s21_truncate(value, s21_reset(result)) && getScale(value) &&
      getSign(value))
    s21_sub(*result, ((s21_decimal){{1, 0, 0, 0}}), result);
  return 0;
}
