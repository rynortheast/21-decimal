#include "./../s21_decimal.h"

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
    int res = 1, exp = getExp(&src);
    if (dst && src != 0 && (src != S21_INF || src != -S21_INF)) {
        dst->bits[0] = 0; dst->bits[1] = 0;
        dst->bits[2] = 0; dst->bits[3] = 0;
        int sign = 1;
        if (src < 0) {
        src *= -1; sign = -1;
        }
        double tmp = (double)src;
        int scale = 0;
        while (scale < 28 && (int)tmp / (int)pow(2, 21) == 0) {
        tmp *= 10; scale++;
        }
        // tmp = round(tmp);
        if (scale <= 28 && (exp > -94 && exp < 96)) {
        fbits mant;
        tmp = (float)tmp;
        for (; fmod(tmp, 10) == 0 && scale > 0; scale--, tmp /= 10) {
        }
        mant.fl = tmp;
        exp = getExp(&mant.fl);
        setBit(dst, exp, 1);
        for (int i = exp - 1, j = 22; j >= 0; i--, j--) {
            if ((mant.ui & (1 << j)) != 0) {
            setBit(dst, i, 1);
            }
        }
        setSign(dst, sign);
        setScale(dst, scale);
        res = 0;
        }
    }
    return res;
}
