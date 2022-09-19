#include "./../s21_decimal.h"

s21_decimal * s21_reset(s21_decimal * value) {
  for (int x = 0; x < 4; x += 1)
    value->bits[x] = 0;
  return value;
}
