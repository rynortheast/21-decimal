#include "./../s21_decimal.h"

int s21_round(s21_decimal value, s21_decimal * result) {
    s21_decimal value2 = {0};
    s21_decimal value3 = {0};
    s21_decimal add = {{ 1, 0, 0, 0 }};
    s21_decimal compare = {{ 5, 0, 0, 0 }};
    
    int sign = getSign(value);
    if (getScale(value)) {
        if (sign) setBit(&value, 127, 0);
        s21_truncate(value, &value2);
        s21_sub(value, value2, &value3);
        setScale(&value3, getScale(value3) - 1);
        s21_truncate(value3, &value3);
        if (s21_is_less(value3, compare)) {
        if (sign) setBit(&value2, 127, 1);
            s21_copy(result, value2);
        } else {
        if (s21_is_equal(value3, compare)) {
            if (getBit(value2, 0) == 1) {
            s21_add(value2, add, result);
            if (sign) setBit(result, 127, 1);
            } else {
                s21_copy(result, value2);
            if (sign) setBit(result, 127, 1);
            }
        } else {
            s21_add(value2, add, result);
            if (sign) setBit(result, 127, 1);
        }
        }
    } else {
        s21_copy(result, value);
    }
    if (sign) setBit(&value, 127, 1);
    return 0;
}
