#include "./../s21_decimal.h"

int s21_floor(s21_decimal value, s21_decimal *result) {
  if (!s21_truncate(value, s21_reset(result)) && getSign(value))
    s21_add(*result, ((s21_decimal) {{1, 0, 0, 0}}), result);
  return 0;
}
