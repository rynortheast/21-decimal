#ifndef SRC_S21_OTHER_H_
#define SRC_S21_OTHER_H_

int s21_floor(s21_decimal value, s21_decimal* result);
int s21_negate(s21_decimal value, s21_decimal* result);
int s21_truncate(s21_decimal value, s21_decimal* result);
int s21_round(s21_decimal value, s21_decimal* result);
s21_decimal* s21_copy(s21_decimal* dest, s21_decimal src);
s21_decimal* s21_reset(s21_decimal* value);

#endif  //  SRC_S21_OTHER_H_
