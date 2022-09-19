#include "./../HEADERS/s21_decimal.h"

// Если коротко, то из четырех возможных резльтутатов функции + и - покрывают три,
// последним является деление на ноль, что по логике только и нужно обработать здесь.

// 0. Проверяем, что второе значением не равно нулю
// 1. ЦИКЛ. Проверяем, что значение1 > значения2
// 2. Используем s21_sub
// 3. Продолжаем если возврощаемое значение равно нулю.
// 4. Если значение не равно нулю, то отправляем в return для s21_div.

// Result будет пустым в случаем провала?
// Result может быть вещественным? 

int s21_div1(s21_decimal value_1, s21_decimal value_2, s21_decimal *result, s21_decimal *tmp);

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal * result) {
  result->bits[0] = result->bits[1] = result->bits[2] = result->bits[3] = 0;
  int res = 1;
  int sign = 0;
  int sign1 = getSign(value_1);
  int sign2 = getSign(value_2);
  if (sign1 != sign2) sign = 1;
  setSign(&value_1, 0);
  setSign(&value_2, 0);
  if (!value_2.bits[0] && !value_2.bits[1] && !value_2.bits[2]) {
    res = 3;
  } else {
    s21_decimal tmp = {0};
    setScale(&value_1, 0);
    setScale(&value_2, 0);
    s21_div1(value_1, value_2, result, &tmp);
    res = 0;
  }
  setSign(result, sign);
  return res;
}

int s21_div1(s21_decimal value_1, s21_decimal value_2, s21_decimal *result, s21_decimal *tmp) {
  int res = 0;
  for (int i = getBitLast(value_1); i >= 0; i--) {
    if (getBit(value_1, i)) setBit(tmp, 0, 1);
    if (s21_is_greater_or_equal(*tmp, value_2)) {
      s21_sub(*tmp, value_2, tmp);
      if (i != 0) leftShift(tmp, 1);
      if (getBit(value_1, i - 1)) setBit(tmp, 0, 1);
      leftShift(result, 1);
      setBit(result, 0, 1);
    } else {
      leftShift(result, 1);
      if (i != 0) leftShift(tmp, 1);
      if ((i - 1) >= 0 && getBit(value_1, i - 1)) setBit(tmp, 0, 1);
    }
  }
  return res;
}
