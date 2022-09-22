#include "./../s21_decimal.h"

//  TODO [s21_div] Функция не работает, проверку провалила.
//  Предположу, что проблемой может является остаток. Если обработать
//  целое число легко, то остаток необходимо обработать по-особенному.
//
//  ПРИМЕР: [10 * 4.2 = 42] => [10 * 4 = 40] + [10 * 0.2 = 2]
//
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  result->bits[0] = result->bits[1] = result->bits[2] = result->bits[3] = 0;
  int res = 0;
  int sign_result = 5;
  if (getSign(value_1) != getSign(value_2)) {
    sign_result = 1;
  } else {
    sign_result = 0;
  }
  setSign(&value_1, 0);
  setSign(&value_2, 0);
  int bit1 = getBitLast(value_1);

  for (int i = 0; i <= bit1; i++) {
    s21_decimal tmp_res = {{0, 0, 0, 0}};
    int value_bit_1 = getBit(value_1, i);

    if (value_bit_1) {
      tmp_res = value_2;
      leftShift(&tmp_res, i);
      res = addBit(*result, tmp_res, result);
    }
  }
  if (res) {
    while (getScale(value_1) > 0 || getScale(value_2) > 0) {
      s21_decimal *value_1_1, *value_2_1;
      if (getBitLast(value_1) > getBitLast(value_2) && getScale(value_1) > 0) {
        value_1_1 = &value_1;
        value_2_1 = &value_2;
      } else if (getBitLast(value_2) > getBitLast(value_1) &&
                 getScale(value_2) > 0) {
        value_1_1 = &value_2;
        value_2_1 = &value_1;
      } else {
        break;
      }
      int scale2 = getScale(*value_1_1);
      decreaseScale(value_1_1, 1);
      setScale(value_1_1, --scale2);
      return s21_mul(*value_1_1, *value_2_1, result);
    }
  }

  int scale = getScale(value_1) + getScale(value_2);
  setScale(result, scale);
  setSign(result, sign_result);
  if (res && sign_result) res = 2;
  return res;
}
