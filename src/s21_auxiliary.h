#ifndef SRC_S21_AUXILIARY_H_
#define SRC_S21_AUXILIARY_H_

int getSign(s21_decimal value);
int getScale(s21_decimal value);
void setSign(s21_decimal * value, int sign);                //  Желательно убрать VOID
int getBit(s21_decimal value, int bit);
void setScale(s21_decimal *d, int scale);                   //  Желательно убрать VOID
int isNull(s21_decimal value);                              //  Есть вопросы к функции
void scaleIncrease(s21_decimal * value, int shift);         //  Убрать VOID + рефактиринг
void s21_copy(s21_decimal * dest, s21_decimal src);         //  Убрать в отдельный файл
int getBitLast(s21_decimal value);                          //  Необходим рефакторинг
void setBit(s21_decimal * value, int bit, int a);           //  Убрать VOID + есть вопросы к функции
void shiftLeft(s21_decimal * value, int shift);             //  Поменять наименование + есть вопросы к функции
int scaleAlignment(s21_decimal * value_1, s21_decimal * value_2);   //  РЕФАКТОРИНГ + есть вопросы к функции
int addBit(s21_decimal * value_1, s21_decimal * value_2, s21_decimal * result);     //  Необходим рефакторинг
void scaleDecrease(s21_decimal * value, int shift);         //  Необходим рефакторинг

void convert(s21_decimal *d);                               //  Необходим для s21_sub

#endif  //  SRC_S21_AUXILIARY_H_
