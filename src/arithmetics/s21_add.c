#include "./../s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  result->bits[0] = result->bits[1] = result->bits[2] = result->bits[3] = 0;
  int res = 5;
  int sign1 = getSign(value_1);
  int sign2 = getSign(value_2);
  if (sign1 == sign2) {
    norm_scale(&value_1, &value_2);
    if (bit_addition(&value_1, &value_2, result)) {
      if ((getScale(value_1) == 0 || getScale(value_2) == 0) &&
          sign1 == 0) {
        res = 1;
      } else if ((getScale(value_1) == 0 || getScale(value_2) == 0) &&
                  sign1 == 1) {
        res = 2;
      } else if (getScale(value_1) > 0 && getScale(value_2) > 0) {
        s21_scale_decrease(&value_1, 1);
        s21_scale_decrease(&value_2, 1);
        setScale(&value_1, getScale(value_1) - 1);
        setScale(&value_2, getScale(value_2) - 1);
        res = s21_add(value_1, value_2, result);
      }
    } else {
      setSign(result, sign1);
      setScale(result, getScale(value_1));
      res = 0;
    }
  } else if (getSign(value_1) && !getSign(value_2)) {
    setSign(&value_1, 0);
    res = s21_sub(value_2, value_1, result);
  } else if (!getSign(value_1) && getSign(value_2)) {
    setSign(&value_2, 0);
    res = s21_sub(value_1, value_2, result);
  }
  return res;
}
