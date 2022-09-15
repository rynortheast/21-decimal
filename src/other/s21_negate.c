#include "./../s21_decimal.h"

int s21_negate(s21_decimal value, s21_decimal * result) {
    s21_copy(result, value);
    if (getSign(*result))
        setSign(result, 1);
    else
        setSign(result, -1);
    return 0;
}
