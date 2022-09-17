#include "./../s21_decimal.h"

// TODO [s21_add] Без понятий какое должно быть начальное значение status
// Что возвращает функция getSign? Минус это 0?
// Если знаки равны, то значения суммируются. Если не равны, то вычитаются. Зачем там делают setSign?
// Из 4 возможных исходов работают только три - ОК, INF, -INF. 
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal * result) {
  int status = 0, valueSign_1 = getSign(value_1), valueSign_2 = getSign(value_2);
  for (int x = 0; x < 4; x += 1)
    result->bits[x] = 0;
  if (valueSign_1 == valueSign_2) {
    alignmentScale(&value_1, &value_2);
    if (addBit(value_1, value_2, result)) {
      if ((!getScale(value_1) || !getScale(value_2)) && !valueSign_1) {
        status = 1;
      } else if ((!getScale(value_1) || !getScale(value_2)) && valueSign_1) {
        status = 2;
      } else if (getScale(value_1) > 0 && getScale(value_2) > 0) {
        setScale(decreaseScale(&value_1, 1), getScale(value_1) - 1);
        setScale(decreaseScale(&value_2, 1), getScale(value_2) - 1);
        status = s21_add(value_1, value_2, result);
      }
    } else {
      setScale(setSign(result, valueSign_1), getScale(value_1));
    }
  } else if (valueSign_1 && !valueSign_2) {
    status = s21_sub(value_2, setSign(&value_1, 0), result);
  } else if (!valueSign_1 && valueSign_2) {
    status = s21_sub(value_1, setSign(&value_2, 0), result);
  }
  return status;
}
