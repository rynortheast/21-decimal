#include "./../s21_decimal.h"

// - - - = -
// - - + = +
// + - - = +
// + - + = -

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int status = 0, valueSign_1 = getSign(value_1),
      valueSign_2 = getSign(value_2);
  for (int x = 0; x < 4; x += 1) result->bits[x] = 0;

  if (valueSign_1 != valueSign_2) {
    status = s21_add(*setSign(&value_1, 0), *setSign(&value_2, 0), result);
    status = status ? (valueSign_1 ? 2 : 1) : status;
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
  return status;
}
