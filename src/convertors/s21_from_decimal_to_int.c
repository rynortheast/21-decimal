#include "./../s21_decimal.h"

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int res = 1;
  if (src.bits[1] == 0 && src.bits[2] == 0) {
    *dst = src.bits[0];
    if (getSign(src)) {
      *dst *= -1;
    }
    *dst /= (int)pow(10, getScale(src));
    res = 0;
  }
  return res;
}
