#include "./../s21_decimal.h"

int s21_floor(s21_decimal value, s21_decimal *result) {
    int scale = getScale(value);
    if (scale) {
        s21_decimal sub = {{1, 0, 0, 0}};
        s21_truncate(value, result);
        if (getSign(value)) {
        setSign(result, 1);
        s21_add(*result, sub, result);
        setSign(result, 1);
        }
    } else {
        s21_copy(result, value);
    }
    return 0;
}
