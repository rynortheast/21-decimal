#ifndef SRC_S21_AUXILIARY_H_
#define SRC_S21_AUXILIARY_H_

int getBit(s21_decimal value, int bit);
int getBitLast(s21_decimal value);
s21_decimal *setBit(s21_decimal *value, int bit, int a);
int addBit(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

int getScale(s21_decimal value);
s21_decimal *setScale(s21_decimal *d, int scale);
s21_decimal *increaseScale(s21_decimal *value, int shift);
s21_decimal *decreaseScale(s21_decimal *value, int shift);
void alignmentScale(s21_decimal *value_1, s21_decimal *value_2);

int getSign(s21_decimal value);
s21_decimal *setSign(s21_decimal *value, int bit);

s21_decimal *convert(s21_decimal *value);
int isNull(s21_decimal value);
int getFloatExp(float *value);
s21_decimal *leftShift(s21_decimal *value, int shift);

#endif  //  SRC_S21_AUXILIARY_H_
