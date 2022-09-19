#include "./../s21_decimal.h"

// TODO [s21_round] Что за 127 бит?   
// TODO [s21_round] Необходимо провести дополнительные тесты                                                                    с
int s21_round(s21_decimal value, s21_decimal * result) {
  
  // s21_decimal value2 = {0};
  // s21_decimal value3 = {0};
  // s21_decimal add = {{ 1, 0, 0, 0 }};
  // s21_decimal compare = {{ 5, 0, 0, 0 }};
  
  if (!s21_truncate(value, result) && getScale(value)) {
    s21_decimal remains = {{0, 0, 0, 0}};
    s21_sub(value, *result, &remains);
    s21_truncate(*setScale(&remains, getScale(remains) - 1), &remains);
    if (s21_is_greater_or_equal(remains, ((s21_decimal) {{5, 0, 0, 0}}))) {
      if (getSign(value)) s21_sub(*result, ((s21_decimal) {{1, 0, 0, 0}}), result);
      else s21_add(*result, ((s21_decimal) {{1, 0, 0, 0}}), result);
    }
  }

  // int sign = getSign(value);
  // if (getScale(value)) {
  //   if (sign) setBit(&value, 127, 0);
  //   s21_truncate(value, &value2);
  //   s21_sub(value, value2, &value3);
  //   setScale(&value3, getScale(value3) - 1);
  //   s21_truncate(value3, &value3);
  //   if (s21_is_less(value3, compare)) {
  //     s21_copy(result, value2);
  //   } else {
  //     if (s21_is_equal(value3, compare)) {
  //       if (getBit(value2, 0) == 1) {
  //         s21_add(value2, add, result);
  //       } else {
  //         s21_copy(result, value2);
  //       }
  //     } else {
  //       s21_add(value2, add, result);
  //     }
  //   }
  //   if (sign) setBit(result, 127, 1);
  // } else {
  //   s21_copy(result, value);
  // }

  return 0;
}
