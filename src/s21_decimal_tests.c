#include "s21_decimal.h"
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

int last_bit(s21_decimal_alt alt);
int s21_is_greater_or_equal(s21_decimal a, s21_decimal b);
int s21_is_less_or_equal(s21_decimal a, s21_decimal b);
int s21_is_less(s21_decimal a, s21_decimal b);
int s21_is_equal(s21_decimal a, s21_decimal b);
int s21_is_greater(s21_decimal a, s21_decimal b);
bool is_null(s21_decimal_alt alt);

bool s21_get_bit_int(unsigned int num, int pos);
void s21_left_shift(s21_decimal_alt *alt);
bool s21_get_sign_std(s21_decimal dec);
int s21_get_exp_std(s21_decimal dec);

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
void s21_rescale(s21_decimal *value_1, s21_decimal *value_2);
s21_decimal_alt s21_convert_std_to_alt(s21_decimal std);
void s21_rescale_alt(s21_decimal_alt *alt_value_1, s21_decimal_alt *alt_value_2);
s21_decimal s21_convert_alt_to_std(s21_decimal_alt alt);
void s21_null_decimal_alt(s21_decimal_alt *alt);
int s21_add_alt(s21_decimal_alt alt_value_1, s21_decimal_alt alt_value_2, s21_decimal_alt *alt_result);
void s21_bank_rounding(s21_decimal_alt *alt, int mod);
int s21_sub_alt(s21_decimal_alt alt_value_1, s21_decimal_alt alt_value_2, s21_decimal_alt *alt_result);
int div_by_ten(s21_decimal_alt *alt);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
s21_decimal_alt div_with_modulo(s21_decimal_alt alt_value_1, s21_decimal_alt alt_value_2, s21_decimal_alt *alt_result);
void init_decimal(s21_decimal *decimal);
void s21_rescale_alt_to_smaller(s21_decimal_alt *alt_value_1, s21_decimal_alt *alt_value_2);
int s21_mul_alt(s21_decimal_alt alt_value_1, s21_decimal_alt alt_value_2, s21_decimal_alt *alt_result);
void s21_right_shift(s21_decimal_alt *alt);
void align(s21_decimal_alt *alt_value_1, s21_decimal_alt *alt_value_2);
bool compare_bits(s21_decimal_alt alt1, s21_decimal_alt alt2);

int main() {

    s21_decimal TEST_1 = {{5, 0, 0, 0}};
    s21_decimal TEST_2 = {{5, 0, 0, 0}};
    s21_decimal RESULT;

    s21_add(TEST_1, TEST_2, &RESULT);

    printf("TEST: %d", RESULT.bits[0]);

    return 0;
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int return_code = 0;
    s21_rescale(&value_1, &value_2);
    s21_decimal_alt alt_value_1 = s21_convert_std_to_alt(value_1);
    s21_decimal_alt alt_value_2 = s21_convert_std_to_alt(value_2);
    s21_decimal_alt alt_result;
    s21_null_decimal_alt(&alt_result);
    if (alt_value_1.sign == alt_value_2.sign) {
        return_code = s21_add_alt(alt_value_1, alt_value_2, &alt_result);
    } else {
        if (alt_value_1.sign) {
            alt_value_1.sign = 0;
            value_1 = s21_convert_alt_to_std(alt_value_1);
            return_code = s21_sub(value_2, value_1, result);
            alt_result = s21_convert_std_to_alt(*result);
        } else {
            alt_value_2.sign = 0;
            value_2 = s21_convert_alt_to_std(alt_value_2);
            return_code = s21_sub(value_1, value_2, result);
            alt_result = s21_convert_std_to_alt(*result);
        }
    }
    *result = s21_convert_alt_to_std(alt_result);
    return return_code;
}

void s21_rescale(s21_decimal *value_1, s21_decimal *value_2) {
    s21_decimal_alt alt_value_1 = s21_convert_std_to_alt(*value_1);
    s21_decimal_alt alt_value_2 = s21_convert_std_to_alt(*value_2);
    s21_rescale_alt(&alt_value_1, &alt_value_2);
    *value_1 = s21_convert_alt_to_std(alt_value_1);
    *value_2 = s21_convert_alt_to_std(alt_value_2);
}

s21_decimal_alt s21_convert_std_to_alt(s21_decimal std) {
    s21_decimal_alt alt;
    s21_null_decimal_alt(&alt);
    alt.sign = s21_get_sign_std(std);
    alt.exp = s21_get_exp_std(std);
    int i = 0;
    for (int j = 0; j < 3; j++) {
        for (int k = 0; k < 32; k++)
            alt.bits[i++] = s21_get_bit_int(std.bits[j], k);
    }
    return alt;
}

void s21_rescale_alt(s21_decimal_alt *alt_value_1, s21_decimal_alt *alt_value_2) {
    s21_decimal_alt ten;
    s21_null_decimal_alt(&ten);
    ten.bits[1] = 1;
    ten.bits[3] = 1;
    if (!is_null(*alt_value_1) && !is_null(*alt_value_2)) {
        if (alt_value_1 -> exp > alt_value_2 -> exp) {
            int exp_dif = alt_value_1 -> exp - alt_value_2 -> exp;
            for (int i = 0; i < exp_dif; i++)
                s21_mul_alt(*alt_value_2, ten, alt_value_2);
            if (last_bit(*alt_value_2) > 95) {
                s21_rescale_alt_to_smaller(alt_value_1, alt_value_2);
            }
            alt_value_2 -> exp = alt_value_1 -> exp;
        } else if (alt_value_2 -> exp > alt_value_1 -> exp) {
            s21_rescale_alt(alt_value_2, alt_value_1);
        }
    } else if (is_null(*alt_value_1)) {
        alt_value_1 -> exp = alt_value_2 -> exp;
    } else {
        alt_value_2 -> exp = alt_value_1 -> exp;
    }
}

s21_decimal s21_convert_alt_to_std(s21_decimal_alt alt) {
    s21_decimal std;
    init_decimal(&std);
    int k = 95;
    for (int i = 2; i >= 0; i--)
        for (int j = 31; j >= 0; j--)
            std.bits[i] = (std.bits[i] << 1) + alt.bits[k--];
    std.bits[3] = alt.sign;
    std.bits[3] <<= 7;
    k = 128;
    for (int i = 7; i >= 0; i--) {
        std.bits[3] <<= 1;
        if (alt.exp >= k) {
            alt.exp -= k;
            std.bits[3]++;
        }
        k >>= 1;
    }
    std.bits[3] <<= 16;
    return std;
}

void s21_null_decimal_alt(s21_decimal_alt *alt) {
    alt -> sign = 0;
    alt -> exp = 0;
    for (int i = 0; i < 192; i++)
        alt -> bits[i] = 0;
}

int s21_add_alt(s21_decimal_alt alt_value_1, s21_decimal_alt alt_value_2, s21_decimal_alt *alt_result) {
    int return_code = 0;
    bool t_bit = 0;  // бит переноса
    s21_null_decimal_alt(alt_result);
    for (int i = 0; i < 192; i++) {
        alt_result -> bits[i] = (alt_value_1.bits[i] ^ alt_value_2.bits[i]) ^ t_bit;
        // нужно попробовать максимально упростить это выражение
        if ((alt_value_1.bits[i] & alt_value_2.bits[i]) || (alt_value_1.bits[i] & t_bit) || \
        (alt_value_2.bits[i] & t_bit))
        // "если хотя бы два бита из трех равны единице"
            t_bit = 1;
        else
            t_bit = 0;
    }
    alt_result -> exp = alt_value_1.exp;
    alt_result -> sign = alt_value_1.sign;
    if (last_bit(*alt_result) > 95) {
        if (alt_result -> exp <= 0) {
            return_code = 1;  // произошло переполнение
        } else {
            int mod = div_by_ten(alt_result);
            s21_bank_rounding(alt_result, mod);
        }
    }
    return return_code;
}

int last_bit(s21_decimal_alt alt) {
    int i = -1;
    if (!is_null(alt)) {
        for (i = 191; i >= 0; i--)
            if (alt.bits[i] == 1)
                break;
    }
    return i;
}

void s21_bank_rounding(s21_decimal_alt *alt, int mod) {
    if ((mod == 5 && alt -> bits[0]) || mod > 5) {
        s21_decimal_alt one;
        s21_null_decimal_alt(&one);
        one.bits[0] = 1;
        one.exp = alt -> exp;
        one.sign = alt -> sign;
        s21_add_alt(*alt, one, alt);
        if (last_bit(*alt) > 95) {
            s21_sub_alt(*alt, one, alt);
            mod = div_by_ten(alt);
            s21_bank_rounding(alt, mod);
        }
    }
}

int s21_sub_alt(s21_decimal_alt alt_value_1, s21_decimal_alt alt_value_2, s21_decimal_alt *alt_result) {
    int return_code = 0;
    bool t_bit = 0;
    for (int i = 0; i < 192; i++) {
        alt_result -> bits[i] = alt_value_1.bits[i] ^ alt_value_2.bits[i] ^ t_bit;
        if (alt_value_1.bits[i] == 0 && alt_value_2.bits[i] == 1)
            t_bit = 1;
        else if (alt_value_1.bits[i] == 1 && alt_value_2.bits[i] == 0)
            t_bit = 0;
    }
    return return_code;
}

int div_by_ten(s21_decimal_alt *alt) {
    s21_decimal_alt result;
    s21_null_decimal_alt(&result);
    // возможно, эта конструкция не нужна и есть решение получше
    int exp = alt -> exp;
    int sign = alt -> sign;
    result.sign = 0;
    result.exp = 0;
    s21_decimal_alt ten;
    s21_null_decimal_alt(&ten);
    ten.bits[1] = 1;
    ten.bits[3] = 1;
    s21_decimal_alt modulo = div_with_modulo(*alt, ten, alt);
    int res = s21_convert_alt_to_std(modulo).bits[0];
    // возможно, эта конструкция не нужна и есть решение получше
    alt -> exp = exp - 1;
    alt -> sign = sign;
    return res;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int return_code = 0;
    s21_rescale(&value_1, &value_2);
    s21_decimal_alt alt_value_1 = s21_convert_std_to_alt(value_1);
    s21_decimal_alt alt_value_2 = s21_convert_std_to_alt(value_2);
    s21_decimal_alt alt_result;
    s21_null_decimal_alt(&alt_result);
    if (alt_value_1.sign == alt_value_2.sign) {
        if ((s21_is_greater_or_equal(value_1, value_2) && !alt_value_1.sign) || \
        (s21_is_less_or_equal(value_1, value_2) && alt_value_1.sign)) {
                return_code = s21_sub_alt(alt_value_1, alt_value_2, &alt_result);
            } else {
                alt_value_1.sign = alt_value_1.sign ^ 1;
                alt_value_2.sign = alt_value_2.sign ^ 1;
                return_code = s21_sub_alt(alt_value_2, alt_value_1, &alt_result);
            }
        alt_result.sign = alt_value_1.sign;
    } else {
        alt_value_2.sign = alt_value_2.sign ^ 1;
        value_2 = s21_convert_alt_to_std(alt_value_2);
        return_code = s21_add(value_1, value_2, result);
        alt_result = s21_convert_std_to_alt(*result);
    }
    alt_result.exp = alt_value_1.exp;
    *result = s21_convert_alt_to_std(alt_result);
    return return_code;
}

int s21_is_greater_or_equal(s21_decimal a, s21_decimal b) {
    int res = 0;
    res = s21_is_equal(a, b);
    if (res == 0) res = s21_is_greater(a, b);
    return res;
}

int s21_is_less_or_equal(s21_decimal a, s21_decimal b) {
    int res = 0;
    res = s21_is_equal(a, b);
    if (res == 0) res = s21_is_less(a, b);
    return res;
}

int s21_is_equal(s21_decimal a, s21_decimal b) {
    int equal = 1;
    int zero_sign = 0;
    if ((a.bits[0] + a.bits[1] + a.bits[2]) == 0 \
            && (b.bits[0] + b.bits[1] + b.bits[2]) == 0) {
        zero_sign = 1;
    }
    bool aSign = s21_get_sign_std(a);
    bool bSign = s21_get_sign_std(b);
    if (zero_sign) {
        equal = 1;
    } else if (aSign != bSign) {
        equal = 0;
    } else {
        s21_rescale(&a, &b);
        for (register int i = 0; i <= 2; i++) {
            if (a.bits[i] != b.bits[i]) {
                equal = 0;
                break;
            }
        }
    }
    return equal;
}

int s21_is_less(s21_decimal a, s21_decimal b) {
    int less = 0;
    int equal = s21_is_equal(a, b);
    if (equal == 0) {
        less = s21_is_greater(a, b) ? 0 : 1;
    }
    return less;
}

int s21_is_greater(s21_decimal a, s21_decimal b) {
    int greater = 0;
    int zero_sign = 0;
    bool aSign = 0, bSign = 0;
    if ((a.bits[0] + a.bits[1] + a.bits[2]) == 0 \
            && (b.bits[0] + b.bits[1] + b.bits[2]) == 0) {
        zero_sign = 1;
    }
    aSign = s21_get_sign_std(a);
    bSign = s21_get_sign_std(b);  // if their signs are not matching = they are ne
    if (zero_sign) {
        greater = 0;
    } else if ((aSign == 0) && (bSign == 1)) {
        greater = 1;
    } else if ((aSign == 1) && (bSign == 0)) {
        greater = 0;
    } else if (aSign == bSign) {
        s21_rescale(&a, &b);
        for (register int i = 2; i >= 0; i--) {
            if (a.bits[i] > b.bits[i]) {
                greater = (aSign == 1) ? 0 : 1;
                break;
            } else if (a.bits[i] < b.bits[i]) {
                greater = (aSign == 1) ? 1 : 0;
                break;
            }
        }
    }
    return (greater);
}

s21_decimal_alt div_with_modulo(s21_decimal_alt alt_value_1, s21_decimal_alt alt_value_2, \
s21_decimal_alt *alt_result) {
    int denominator_left_bit = last_bit(alt_value_2);
    s21_decimal_alt modulo;
    s21_null_decimal_alt(&modulo);
    s21_null_decimal_alt(alt_result);
    if (compare_bits(alt_value_1, alt_value_2)) {
        align(&alt_value_1, &alt_value_2);
    }
    for (int i = 0; i < 191; i++) {
        if (compare_bits(alt_value_1, alt_value_2)) {
            s21_sub_alt(alt_value_1, alt_value_2, &alt_value_1);
            alt_result -> bits[0] = 1;
        }
        if (last_bit(alt_value_2) <= denominator_left_bit)
            break;
        if (last_bit(alt_value_2) > denominator_left_bit)
            s21_right_shift(&alt_value_2);
        s21_left_shift(alt_result);
    }
    return alt_value_1;
}

void init_decimal(s21_decimal *decimal) {
    for (int i = 0; i < 4; i++) {
        decimal->bits[i] = 0;
    }
}

void s21_rescale_alt_to_smaller(s21_decimal_alt *alt_value_1, s21_decimal_alt *alt_value_2) {
    int mod1 = 0, mod2 = 0;
    while (last_bit(*alt_value_2) > 95) {
        mod1 = div_by_ten(alt_value_1);
        mod2 = div_by_ten(alt_value_2);
    }
    s21_bank_rounding(alt_value_1, mod1);
    s21_bank_rounding(alt_value_2, mod2);
    if (last_bit(*alt_value_2) > 95) {
        mod1 = div_by_ten(alt_value_1);
        mod2 = div_by_ten(alt_value_2);
        s21_bank_rounding(alt_value_1, mod1);
        s21_bank_rounding(alt_value_2, mod2);
    }
}

int s21_mul_alt(s21_decimal_alt alt_value_1, s21_decimal_alt alt_value_2, s21_decimal_alt *alt_result) {
    int return_code = 0;
    s21_null_decimal_alt(alt_result);
    if (!is_null(alt_value_1) && !(is_null(alt_value_2))) {
        for (int i = 0; i < 192; i++) {
            if (alt_value_2.bits[i] == 1)
                s21_add_alt(*alt_result, alt_value_1, alt_result);
            s21_left_shift(&alt_value_1);
        }
        alt_result -> exp = alt_value_1.exp + alt_value_2.exp;
        alt_result -> sign = alt_value_1.sign ^ alt_value_2.sign;
        if (last_bit(*alt_result) > 95 || alt_result -> exp > 28) {
            int mod = 0;
            while (alt_result -> exp > 0 && (last_bit(*alt_result) > 95 || alt_result ->  exp > 28)) {
                if (alt_result -> exp == 0)
                    break;
                mod = div_by_ten(alt_result);
            }
            if (alt_result -> exp > 0)
                s21_bank_rounding(alt_result, mod);
            else
                return_code = 1;
        }
        if (is_null(*alt_result))
            return_code = 2;
    }
    return return_code;
}

bool is_null(s21_decimal_alt alt) {
    bool result = 0;
    for (int i = 0; i < 192; i++)
        result += alt.bits[i];
    return !result;
}

bool s21_get_bit_int(unsigned int num, int pos) {
    return (num >> pos) & 1;
}

bool s21_get_sign_std(s21_decimal dec) {
    return s21_get_bit_int(dec.bits[3], 31);
}

int s21_get_exp_std(s21_decimal dec) {
    return (dec.bits[3] % 2147483648) >> 16;
}

void s21_right_shift(s21_decimal_alt *alt) {
    for (int i = 0; i < 191; i++)
        alt -> bits[i] = alt -> bits[i + 1];
    alt -> bits[191] = 0;
}

void s21_left_shift(s21_decimal_alt *alt) {
    for (int i = 191; i > 0; i--)
        alt -> bits[i] = alt -> bits[i - 1];
    alt -> bits[0] = 0;
}

void align(s21_decimal_alt *alt_value_1, s21_decimal_alt *alt_value_2) {
    if (!is_null(*alt_value_1) && !is_null(*alt_value_2)) {
        int l1 = last_bit(*alt_value_1);
        int l2 = last_bit(*alt_value_2);
        while (last_bit(*alt_value_1) != last_bit(*alt_value_2))
            if (compare_bits(*alt_value_1, *alt_value_2))
                s21_left_shift(alt_value_2);
            else
                s21_left_shift(alt_value_1);
        if (!compare_bits(*alt_value_1, *alt_value_2))
            s21_left_shift(alt_value_1);
        if (alt_value_1 -> bits[0] == 0 && alt_value_2 -> bits[0] == 0 \
        && last_bit(*alt_value_1) > l1 && last_bit(*alt_value_2) > l2) {
            s21_right_shift(alt_value_1);
            s21_right_shift(alt_value_2);
        }
    }
}

bool compare_bits(s21_decimal_alt alt1, s21_decimal_alt alt2) {
    int i = 191;
    while (i >= 0 && alt1.bits[i] == alt2.bits[i])  {
        i--;
    }
    return i == -1 ? 1 : alt1.bits[i];
}
