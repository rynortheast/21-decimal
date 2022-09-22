#include "./../s21_decimal.h"

//  TODO [s21_mod] Необходим рефакторинг кода.
//  TODO [s21_mod] Функция не работает, проверку провалило.
//  Скорее всего она работает, но из-за проблем с s21_div и s21_mul 
//  выдает некорректный результат. Необходимо проверять, тестировать.
//
//  ПРИМЕР: [10 % 4 = 2] => div -> truncate -> mul -> sub
//
int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  result->bits[0] = result->bits[1] = result->bits[2] = result->bits[3] = 0;
  int res = 5;
  if (s21_is_equal(value_1, value_2)) {
    result->bits[0] = 0;
    result->bits[1] = 0;
    result->bits[2] = 0;
    result->bits[3] = 0;
    res = 0;
  } else if (s21_is_less(value_1, value_2) && !isNull(value_1) &&
             !isNull(value_2)) {
    result->bits[0] = value_1.bits[0];
    result->bits[1] = value_1.bits[1];
    result->bits[2] = value_1.bits[2];
    result->bits[3] = value_1.bits[3];
    res = 0;
  } else {
    s21_decimal mod = {0};
    setSign(&value_1, 0);
    setSign(&value_1, 0);
    res = s21_div(value_1, value_2, &mod);
    if (!res) {
      s21_truncate(mod, &mod);
      s21_mul(mod, value_2, &mod);
      s21_sub(value_1, mod, result);
    }
  }
  return res;
}
