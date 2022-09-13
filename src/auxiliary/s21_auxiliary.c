#include "./../s21_decimal.h"

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
        if (bit_addition(&tmp, &tmp1, value)) {
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

int norm_scale(s21_decimal *a, s21_decimal *b) {
    int res = 0;
    if (getScale(*a) != getScale(*b)) {
        if (getScale(*a) < getScale(*b)) {
            norm_scale(b, a);
        } else {
            s21_decimal *high = a, *low = b;
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
            res = 0;
        }
    }
    return res;
}

int scale_equalize(s21_decimal *a, s21_decimal *b) {
  s21_decimal *big = NULL;
  s21_decimal *small = NULL;
  s21_decimal temp;
  temp.bits[0] = temp.bits[1] = temp.bits[2] = temp.bits[3] = 0;
  int per = 0;
  int out = 1;
  if (getScale(*a) != getScale(*b)) {
    if (getScale(*a) > getScale(*b)) {
      big = a;
      small = b;
      per = 1;
    } else {
      big = b;
      small = a;
    }
    int scaleSmall = getScale(*small);
    int scaleBig = getScale(*big);
    int newscale = scaleBig - scaleSmall;
    while (newscale) {
      temp.bits[0] = small->bits[0];
      temp.bits[1] = small->bits[1];
      temp.bits[2] = small->bits[2];
      temp.bits[3] = small->bits[3];
      scaleIncrease(small, 1);
      if (!getBit(*small, 96)) {
        s21_copy(&temp, *small);
        scaleSmall++;
        newscale--;
      } else {
        s21_copy(small, temp);
        break;
      }
    }
    setScale(&temp, scaleSmall);
    int newScale2 = scaleBig - scaleSmall;
    while (newScale2) {
      s21_scale_decrease(big, newScale2);
      setScale(big, scaleSmall);
      newScale2--;
    }
    if (per) {
      a = big;
      b = &temp;
    } else {
      a = &temp;
      b = big;
    }
    out = 0;
  }
  return out;
}

int bit_addition(s21_decimal *a, s21_decimal *b, s21_decimal *res) {
  int e = 0;
  int resultat = 0;
  for (int i = 0; i < 96; i++) {
    int bit1 = getBit(*a, i);
    int bit2 = getBit(*b, i);

    if (!bit1 && !bit2) {
      if (e) {
        setBit(res, i, 1);
        e = 0;
      } else {
        setBit(res, i, 0);
      }
    } else if (bit1 != bit2) {
      if (e) {
        setBit(res, i, 0);
        e = 1;
      } else {
        setBit(res, i, 1);
      }
    } else {
      if (e) {
        setBit(res, i, 1);
        e = 1;
      } else {
        setBit(res, i, 0);
        e = 1;
      }
    }
    if (i == 95 && e == 1 && !getBit(*a, 97) && !getBit(*b, 97)) {
      resultat = 1;
    }
  }
  return resultat;
}

void s21_scale_decrease(s21_decimal *value, int shift) {
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
  bit_addition(d, &add, &result);

  d->bits[0] = result.bits[0];
  d->bits[1] = result.bits[1];
  d->bits[2] = result.bits[2];
  setBit(d, 97, 1);
}
