#include "./../s21_decimal.h"

int s21_truncate(s21_decimal value, s21_decimal *result) {
  s21_copy(s21_reset(result),
           getScale(value) ? *decreaseScale(&value, getScale(value)) : value);
  return 0;
}
