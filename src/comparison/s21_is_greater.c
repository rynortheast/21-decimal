#include "./../HEADERS/s21_decimal.h"

int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
    int res = 0;
    int sign_a = getSign(value_1), sign_b = getSign(value_2);
    if (!(isNull(value_1) && isNull(value_2))) {
        if (sign_a != sign_b) {
            if (sign_a == 0) {
                res = 1;
            } else {
                res = 0;
            }
        } else {
            alignmentScale(&value_1, &value_2);
            for (int i = 95; i >= 0; i--) {
                int bit_a = getBit(value_1, i), bit_b = getBit(value_2, i);
                if (bit_a != bit_b) {
                    if (bit_a != 0) {
                        res = (sign_a == 0) ? 1 : 0;
                        break;
                    } else {
                        res = (sign_a == 0) ? 0 : 1;
                        break;
                    }
                }
            }  
        }
    }
    return res;
}
