#include "./../s21_decimal.h"

// TODO [s21_sub] Он прошел вертер?
// TODO [s21_sub] Так и не понял как работает convert.
// TODO [s21_sub] ОШИБКА В ТЕСТЕ. СМ. ВНИЗУ ФАЙЛА.
// ПРИМЕР. 3 - 2 = 1. Единственный способ реализовать это через + это выражение -2 + 3,
// НО! функции s21_add выполняет подобные выражения через s21_sub, то есть выходит цикл, кек.
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal * result) {
  int status = 0, valueSign_1 = getSign(value_1), valueSign_2 = getSign(value_2);
  for (int x = 0; x < 4; x += 1)
    result->bits[x] = 0;

  // - - - = -
  // - - + = + (-)
  // + - - = + (+)
  // + - + = -

  if (valueSign_1 != valueSign_2) {
    status = s21_add(*setSign(&value_1, 0), *setSign(&value_2, 0), result);
    setSign(result, valueSign_1);
  } else {
    alignmentScale(&value_1, &value_2);
    if (!s21_is_equal(value_1, value_2)) {
      setSign(&value_1, 0);
      setSign(&value_2, 0);
      if (s21_is_less(value_1, value_2)) {
        status = s21_add(*convert(&value_1), value_2, result);
        setSign(result, !valueSign_2);
      } else {
        status = s21_add(*convert(&value_2), value_1, result);
        setSign(result, valueSign_1);
      }
    }
  }
  // if (status && result_sign) res = 2;
  return status;
}

// START_TEST(sub_11) {
//   s21_decimal val1 = {
//       {UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};
//   s21_decimal val2 = {{4, 0, 0, 0}};
//   s21_decimal res;
//   ck_assert_int_eq(2, s21_sub(val1, val2, &res));
// }
// END_TEST

// START_TEST(sub_14) {
//   s21_decimal val1 = {{4, 0, 0, ~(UINT_MAX / 2)}};
//   s21_decimal val2 = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
//   s21_decimal res;
//   ck_assert_int_eq(2, s21_sub(val1, val2, &res));
// }
// END_TEST

// START_TEST(sub_20) {
//   s21_decimal dec1;
//   s21_decimal dec2;
//   int tmp1 = 100;
//   int tmp2 = 99999;
//   int res_s21 = 0;
//   int res = -99899;
//   s21_decimal res1;
//   s21_from_int_to_decimal(tmp1, &dec1);
//   s21_from_int_to_decimal(tmp2, &dec2);
//   s21_sub(dec1, dec2, &res1);
//   s21_from_decimal_to_int(res1, &res_s21);
//   ck_assert_int_eq(res_s21, res);
// }
// END_TEST
