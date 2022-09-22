#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

#include <limits.h>
#include <stdio.h>
#include <math.h>

typedef struct {
  unsigned int bits[4];
} s21_decimal;

//  TODO [fbits] Необходимо посмотреть можно ли от этого избавиться.
//  Используется в s21_from_float_to_decimal.
typedef union {
  int ui;
  float fl;
} fbits;

//  TODO [S21_MAX_UINT] Необходимо посмотреть можно ли от этого избавиться.
//  Используется в decreaseScale. Изначально там была переменная UINT_MAX,
//  что в printf показывает аналогичное значение, однако при расчётах в 
//  одной из интераций результат оказывался непраивльным, что делало весь
//  итоговый результат некорректным.
#define S21_MAX_UINT 4294967295

#include "./HEADERS/s21_auxiliary.h"
#include "./HEADERS/s21_comparison.h"
#include "./HEADERS/s21_arithmetics.h"
#include "./HEADERS/s21_convertors.h"
#include "./HEADERS/s21_other.h"

#endif  //  SRC_S21_DECIMAL_H_
