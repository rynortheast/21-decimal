#include "./../HEADERS/s21_decimal.h"

s21_decimal * s21_copy(s21_decimal * dest, s21_decimal src) {
  for (int x = 0; x < 4; x += 1)
    dest->bits[x] = src.bits[x];
  return dest;
}
