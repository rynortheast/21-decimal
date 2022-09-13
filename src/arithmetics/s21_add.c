#include "./../s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  result->bits[0] = result->bits[1] = result->bits[2] = result->bits[3] = 0;
  int res = 5;
  int sign1 = get_sign(&value_1);
  int sign2 = get_sign(&value_2);
  if (sign1 == sign2) {
    scale_equalize(&value_1, &value_2);
    if (bit_addition(&value_1, &value_2, result)) {
      if ((get_scale(&value_1) == 0 || get_scale(&value_2) == 0) &&
          sign1 == 0) {
        res = 1;
      } else if ((get_scale(&value_1) == 0 || get_scale(&value_2) == 0) &&
                  sign1 == 1) {
        res = 2;
      } else if (get_scale(&value_1) > 0 && get_scale(&value_2) > 0) {
        s21_scale_decrease(&value_1, 1);
        s21_scale_decrease(&value_2, 1);
        set_scale(&value_1, get_scale(&value_1) - 1);
        set_scale(&value_2, get_scale(&value_2) - 1);
        res = s21_add(value_1, value_2, result);
      }
    } else {
      set_sign(result, sign1);
      set_scale(result, get_scale(&value_1));
      res = 0;
    }
  } else if (get_sign(&value_1) && !get_sign(&value_2)) {
    set_sign(&value_1, 0);
    res = s21_sub(value_2, value_1, result);
  } else if (!get_sign(&value_1) && get_sign(&value_2)) {
    set_sign(&value_2, 0);
    res = s21_sub(value_1, value_2, result);
  }
  return res;
}
