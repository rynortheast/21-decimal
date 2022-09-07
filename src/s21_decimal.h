#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

#include <stdbool.h>

typedef struct {
  int bits[4];
} s21_decimal;

typedef struct {
    bool bits[192];
    bool sign;
    int exp;
} s21_decimal_alt;

#endif  //  SRC_S21_DECIMAL_H_