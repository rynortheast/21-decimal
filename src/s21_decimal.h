#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

// #include <stdbool.h>
#include <stdio.h>
// #include <math.h>

typedef struct {
  int bits[4];
} s21_decimal;

#define S21_MAX_UINT 4294967295

#include "s21_auxiliary.h"
#include "s21_arithmetics.h"

#endif  //  SRC_S21_DECIMAL_H_
