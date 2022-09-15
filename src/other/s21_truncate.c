#include "./../s21_decimal.h"

int s21_truncate(s21_decimal value, s21_decimal * result) {
    int scale = getScale(value);
    if (scale) {
        s21_decimal tmp = {0};
        s21_copy(&tmp, value);
        scaleDecrease(&tmp, scale);
        s21_copy(result, tmp);
    } else {
        s21_copy(result, value);
    }
    return 0;
}
