#include "./../s21_decimal.h"

int s21_round(s21_decimal value, s21_decimal* result) {
  if (!s21_truncate(value, result) && getScale(value)) {
    s21_decimal remains = {{0, 0, 0, 0}};
    s21_sub(value, *result, &remains);
    s21_truncate(*setScale(&remains, getScale(remains) - 1), &remains);
    if (s21_is_greater_or_equal(remains, ((s21_decimal){{5, 0, 0, 0}}))) {
      if (getSign(value))
        s21_sub(*result, ((s21_decimal){{1, 0, 0, 0}}), result);
      else
        s21_add(*result, ((s21_decimal){{1, 0, 0, 0}}), result);
    }
  }
  return 0;
}
