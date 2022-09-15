#include "./../s21_decimal.h"

int getExp(float * value) {
    int * valueInt = (int *) src;
    return ((*valueInt & ~(1u << 31)) >> 23) - 127;
}

int getSign(s21_decimal value) {
  return !!(value.bits[3] & (1u << 31));
}

int getScale(s21_decimal value) { 
  return (char) (value.bits[3] >> 16);
}

void setSign(s21_decimal * value, int sign) {
  if (sign == (-1)) value->bits[3] | (1u << 31);  // Почему здесь sign = -1?
  else value->bits[3] & ~(1u << 31);
}

int getBit(s21_decimal value, int bit) {
  return !!(value.bits[bit / 32] & (1u << (bit % 32)));   // Здесь разве можно использовать "%"?
}

void setScale(s21_decimal * value, int scale) {
  int sign = getSign(*value) ? 1 : 0;
  if (scale > 0 && scale < 28) {
    value->bits[3] &= ~(0xFF << 16);
    value->bits[3] |= scale << 16;
  }
  sign ? setSign(value, 1) : 0; // Почему здесь меняется знак? Почему нельзя это нельзя выполнить в начале?
}

// Очень странная функция. Она будто бессмысленна, т.к. изначальный её вид вообще
// не учитывал возможность отрицательного исхода.
int isNull(s21_decimal value) {
  return (&value) ? (!value.bits[0] && !value.bits[1] && !value.bits[2]) : 0;
}

// Необходим рефактиринг
void scaleIncrease(s21_decimal * value, int shift) {
    if ((getScale(*value) + shift) < 29) {
        setScale(value, getScale(*value) + shift);
        s21_decimal tmp = *value, tmp1 = *value;
        shiftLeft(&tmp, 1);
        shiftLeft(&tmp1, 3);
        if (addBit(&tmp, &tmp1, value)) {
            setBit(value, 96, 1);
        }
    }
}

void s21_copy(s21_decimal * dest, s21_decimal src) {
  for (int x = 0; x < 4; x += 1)
    dest->bits[x] = src.bits[x];
}

// Необходим рефакториг
int getBitLast(s21_decimal value) {
  int bit = 95;
  while ((bit >= 0) && (getBit(value, bit) == 0))
    bit -= 1;
  return bit;
}

// Можно ли тут вообще использовать знак деления?
// Что значит переменная "a"?
void setBit(s21_decimal * value, int bit, int a) {
  if (bit / 32 < 4 && a) 
    value->bits[bit / 32] |= (1u << (bit % 32));
  else if (bit / 32 < 4 && !a) 
    value->bits[bit / 32] &= ~(1u << (bit % 32));
}

//  Поменять наименование + рефакторинг
//  Нужны ли тут переменные last_bit_1 и last_bit_2?
void shiftLeft(s21_decimal * value, int shift) {
    if (!(getBitLast(*value) + shift > 95)) {
        for (int y = 0; y < shift; y += 1) {
            int last_bit_1 = getBit(*value, 31);
            int last_bit_2 = getBit(*value, 63);
            for (int x = 0; x < 3; x += 1)
              value->bits[x] <<= 1;
            if (last_bit_1) setBit(value, 32, 1);
            if (last_bit_2) setBit(value, 64, 1);
        }
    }
}

//  Необходим рефакторинг
//  Зачем здесь переменная res, кек?
int scaleAlignment(s21_decimal * value_1, s21_decimal * value_2) {
    int res = 0;
    if (getScale(*value_1) != getScale(*value_2)) {
        if (getScale(*value_1) < getScale(*value_2)) {
            scaleAlignment(value_2, value_1);
        } else {
            s21_decimal * high = value_1, * low = value_2;
            s21_decimal tmp = {0};
            int scaleLow = getScale(*low), scaleHigh = getScale(*high);
            int diff_scale = scaleHigh - scaleLow;
            while (diff_scale) {
                s21_copy(&tmp, *low);
                scaleIncrease(low, 1);
                if (!getBit(*low, 96)) {
                    s21_copy(&tmp, *low);
                    scaleLow++; diff_scale--;
                } else {
                    s21_copy(low, tmp);
                    break;
                }
            }
            setScale(&tmp, scaleLow);
            diff_scale = scaleHigh - scaleLow;
            while (diff_scale) {
                scaleDecrease(high, diff_scale);
                setScale(high, scaleLow);
                diff_scale--;
                res = 0;
            }
        }
    }
    return res;
}

//  Необходим рефакторинг
int addBit(s21_decimal * value_1, s21_decimal * value_2, s21_decimal * result) {
    int fres = 0, exp = 0;
    for (int i = 0; i < 96; i++) {
        int bit_a = getBit(*value_1, i), bit_b = getBit(*value_2, i);
        if (!bit_a && !bit_b) {
            if (exp) {
                setBit(result, i, 1);
                exp = 0;
            } else {
                setBit(result, i, 0);
            }
        } else if (bit_a != bit_b) {
            if (exp) {
                setBit(result, i, 0);
                exp = 1;
            } else {
                setBit(result, i, 1);
            }
        } else {
            if (exp) {
                setBit(result, i, 1);
                exp = 1;
            } else {
                setBit(result, i, 0);
                exp = 1;
            }
        }
        if (i == 95 && exp == 1 && !getBit(*value_1, 97) && !getBit(*value_2, 97)) {
        fres = 1;
        }
    }
    return fres;
}

//  Необходим рефакторинг
void scaleDecrease(s21_decimal * value, int shift) {
  int i, tmp, j = 0;
  unsigned long long overflow;
  while (j < shift) {
    overflow = value->bits[2];
    i = 2;
    while (i > 0) {
      tmp = overflow % 10;
      value->bits[i] = overflow / 10;
      i--;
      overflow = tmp * (S21_MAX_UINT + 1) + value->bits[i];
      if (i == 0) value->bits[i] = overflow / 10;
    }
    j++;
  }
  setScale(value, (getScale(*value) - shift));
}

void convert(s21_decimal *d) {
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal add = {{1, 0, 0, 0}};
  d->bits[0] = ~d->bits[0];
  d->bits[1] = ~d->bits[1];
  d->bits[2] = ~d->bits[2];
  addBit(d, &add, &result);

  d->bits[0] = result.bits[0];
  d->bits[1] = result.bits[1];
  d->bits[2] = result.bits[2];
  setBit(d, 97, 1);
}
