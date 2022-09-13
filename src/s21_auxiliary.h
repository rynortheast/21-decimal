#ifndef SRC_S21_AUXILIARY_H_
#define SRC_S21_AUXILIARY_H_

int get_sign(s21_decimal *d);
int scale_equalize(s21_decimal *a, s21_decimal *b); // norm scale
void s21_scale_increase1(s21_decimal *value, int shift); // scale up.. .
int get_scale(s21_decimal *d);
int bit_addition(s21_decimal *a, s21_decimal *b, s21_decimal *res); // check bit
void set_scale(s21_decimal *d, int scale); // def_scale
void set_sign(s21_decimal *d, int sign); // def_sign
int s21_is_equal(s21_decimal a, s21_decimal b);
int s21_is_less(s21_decimal a, s21_decimal b);
int s21_is_greater(s21_decimal a, s21_decimal b);
int zero(s21_decimal a, s21_decimal b); // is null
void set_bit(s21_decimal *d, int bit, int a); // def bit
void bits_copy(s21_decimal *dest, s21_decimal src); // decimal dec copy
int get_bit(s21_decimal d, int bit);
void left(s21_decimal *d, int g); // offset left
int last_bit(s21_decimal d); // get fin bit

void s21_scale_decrease(s21_decimal *value, int shift);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
void convert(s21_decimal *d);
void s21_set_scale(s21_decimal *value, int size);

#endif  //  SRC_S21_AUXILIARY_H_
