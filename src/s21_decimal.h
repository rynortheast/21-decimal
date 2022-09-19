#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

#include <limits.h>
#include <stdio.h>
#include <math.h>

typedef struct {
  int bits[4];
} s21_decimal;

typedef union {
  int ui;
  float fl;
} fbits;

#define S21_INF 1.0 / 0.0

#include "./HEADERS/s21_auxiliary.h"
#include "./HEADERS/s21_comparison.h"
#include "./HEADERS/s21_arithmetics.h"
#include "./HEADERS/s21_convertors.h"
#include "./HEADERS/s21_other.h"

#endif  //  SRC_S21_DECIMAL_H_
