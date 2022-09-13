#include "./../s21_decimal.h"

int s21_is_less(s21_decimal a, s21_decimal b) {
  return s21_is_greater(b, a);
}
