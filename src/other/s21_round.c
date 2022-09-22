#include "./../s21_decimal.h"

int s21_round(s21_decimal value, s21_decimal* result) {
  if (!s21_truncate(value, result) && !s21_is_equal(value, *result)) {
    if (getSign(value))
      s21_sub(value, ((s21_decimal){{5, 0, 0, 65536}}), &value);
    else
      s21_add(value, ((s21_decimal){{5, 0, 0, 65536}}), &value);
    s21_truncate(value, result);
  }
  return 0;
}
