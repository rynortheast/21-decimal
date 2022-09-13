#include "./../s21_decimal.h"

int get_sign(s21_decimal *d) {
  unsigned int mask = 1u << 31;
  return !!(d->bits[3] & mask);
}

int scale_equalize(s21_decimal *a, s21_decimal *b) {
  s21_decimal *big = NULL;
  s21_decimal *small = NULL;
  s21_decimal temp;
  temp.bits[0] = temp.bits[1] = temp.bits[2] = temp.bits[3] = 0;
  int per = 0;
  int out = 1;
  if (get_scale(a) != get_scale(b)) {
    if (get_scale(a) > get_scale(b)) {
      big = a;
      small = b;
      per = 1;
    } else {
      big = b;
      small = a;
    }
    int scaleSmall = get_scale(small);
    int scaleBig = get_scale(big);
    int newscale = scaleBig - scaleSmall;
    while (newscale) {
      temp.bits[0] = small->bits[0];
      temp.bits[1] = small->bits[1];
      temp.bits[2] = small->bits[2];
      temp.bits[3] = small->bits[3];
      s21_scale_increase1(small, 1);
      if (!get_bit(*small, 96)) {
        bits_copy(&temp, *small);
        scaleSmall++;
        newscale--;
      } else {
        bits_copy(small, temp);
        break;
      }
    }
    set_scale(&temp, scaleSmall);
    int newScale2 = scaleBig - scaleSmall;
    while (newScale2) {
      s21_scale_decrease(big, newScale2);
      set_scale(big, scaleSmall);
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
    int bit1 = get_bit(*a, i);
    int bit2 = get_bit(*b, i);

    if (!bit1 && !bit2) {
      if (e) {
        set_bit(res, i, 1);
        e = 0;
      } else {
        set_bit(res, i, 0);
      }
    } else if (bit1 != bit2) {
      if (e) {
        set_bit(res, i, 0);
        e = 1;
      } else {
        set_bit(res, i, 1);
      }
    } else {
      if (e) {
        set_bit(res, i, 1);
        e = 1;
      } else {
        set_bit(res, i, 0);
        e = 1;
      }
    }
    if (i == 95 && e == 1 && !get_bit(*a, 97) && !get_bit(*b, 97)) {
      resultat = 1;
    }
  }
  return resultat;
}

int get_scale(s21_decimal *d) {
    return (char)(d->bits[3] >> 16);
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
  s21_set_scale(value, (get_scale(value) - shift));
}

void set_scale(s21_decimal *d, int scale) {
  int e = 0;
  if (get_sign(d)) {
    e = 1;
  }
  if (scale > 0 && scale < 28) {
    int cMask = ~(0xFF << 16);
    d->bits[3] &= cMask;
    int mask = scale << 16;
    d->bits[3] |= mask;
  }
  if (e) {
    set_sign(d, 1);
  }
}

void s21_set_scale(s21_decimal *value, int size) {
  value->bits[3] &= ~(S21_MAX_UINT / 2);
  value->bits[3] >>= 16;
  value->bits[3] |= size;
  value->bits[3] <<= 16;
}

void set_sign(s21_decimal *d, int sign) {
  unsigned int mask = 1u << 31;
  if (sign != 0) {
    d->bits[3] |= mask;
  } else {
    d->bits[3] &= ~mask;
  }
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  result->bits[0] = result->bits[1] = result->bits[2] = result->bits[3] = 0;
  int res = 5;
  int result_sign = 5;
  int sign1 = get_sign(&value_1);
  int sign2 = get_sign(&value_2);
  scale_equalize(&value_1, &value_2);
  if (sign1 != sign2) {
    result_sign = sign1;
    set_sign(&value_1, 0);
    set_sign(&value_2, 0);
    res = s21_add(value_1, value_2, result);
    set_sign(result, result_sign);
  } else {
    if (s21_is_equal(value_1, value_2)) {
      result->bits[0] = 0;
      result->bits[1] = 0;
      result->bits[2] = 0;
      result->bits[3] = 0;
      res = 0;
    } else {
      set_sign(&value_1, 0);
      set_sign(&value_2, 0);
      s21_decimal *small, *big;
      if (s21_is_less(value_1, value_2)) {
        small = &value_1;
        big = &value_2;
        result_sign = !sign2;
      } else {
        small = &value_2;
        big = &value_1;
        result_sign = sign1;
      }
      convert(small);
      res = s21_add(*small, *big, result);
      set_sign(result, result_sign);
    }
  }
  if (res && result_sign) res = 2;
  return res;
}

int s21_is_equal(s21_decimal a, s21_decimal b) {
  int res = 1;
  if (get_sign(&a) == get_sign(&b)) {
    scale_equalize(&a, &b);
    for (int i = 95; i >= 0; i--) {
      int bit1 = get_bit(a, i);
      int bit2 = get_bit(b, i);
      if (bit1 != bit2) {
        res = 0;
      }
    }
  } else if (!a.bits[0] && !a.bits[1] && !a.bits[2] && !b.bits[0] &&
              !b.bits[1] && !b.bits[2]) {
    res = 1;
  } else {
    res = 0;
  }
  return res;
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
  set_bit(d, 97, 1);
}

int s21_is_less(s21_decimal a, s21_decimal b) { return s21_is_greater(b, a); }

int s21_is_greater(s21_decimal a, s21_decimal b) {
  int res = 0;
  int sign1 = get_sign(&a);
  int sign2 = get_sign(&b);
  if (!zero(a, b)) {
    if (sign1 == 0 && sign2 == 1) {
      res = 1;
    } else if (sign1 == 1 && sign2 == 0) {
      res = 0;
    } else if (sign1 == sign2) {
      scale_equalize(&a, &b);
      for (int i = 95; i >= 0; i--) {
        int bit1 = get_bit(a, i);
        int bit2 = get_bit(b, i);
        if (bit1 != bit2) {
          if (!(bit1 == 0) && bit2 == 0 && sign1 == 0) {
            res = 1;
            break;
          } else if (!(bit1 == 0) && bit2 == 0 && sign1 == 1) {
            res = 0;
            break;
          } else if (!(bit2 == 0) && sign1 == 0) {
            res = 0;
            break;
          } else if (!(bit2 == 0) && sign1 == 1) {
            res = 1;
            break;
          }
        }
      }
    }
  }
  return res;
}

int zero(s21_decimal a, s21_decimal b) {
  int res = 0;
  s21_decimal *p1 = &a;
  s21_decimal *p2 = &b;

  if (p1 && p2) {
    if (!a.bits[0] && !b.bits[0] && !a.bits[1] && !b.bits[1] && !a.bits[2] &&
        !b.bits[2])
      res = 1;
  }
  return res;
}

void set_bit(s21_decimal *d, int bit, int a) {
  unsigned int mask = 1u << (bit % 32);
  if (bit / 32 < 4 && a) {
    d->bits[bit / 32] |= mask;
  } else if (bit / 32 < 4 && !a) {
    d->bits[bit / 32] &= ~mask;
  }
}

void bits_copy(s21_decimal *dest, s21_decimal src) {
  dest->bits[0] = src.bits[0];
  dest->bits[1] = src.bits[1];
  dest->bits[2] = src.bits[2];
  dest->bits[3] = src.bits[3];
}

int get_bit(s21_decimal d, int bit) {
  unsigned int mask = 1u << (bit % 32);
  return !!(d.bits[bit / 32] & mask);
}

void s21_scale_increase1(s21_decimal *value, int shift) {
  if ((get_scale(value) + shift) < 29) {
    set_scale(value, (get_scale(value) + shift));
    s21_decimal tmp;
    tmp = *value;
    s21_decimal tmp1;
    tmp1 = *value;
    left(&tmp, 1);
    left(&tmp1, 3);
    if (bit_addition(&tmp, &tmp1, value)) {
      set_bit(value, 96, 1);
    }
  }
}

void left(s21_decimal *d, int g) {
  int lastbit = last_bit(*d);
  if (lastbit + g > 95) {
    return;
  }
  for (int i = 0; i < g; i++) {
    int bit31 = get_bit(*d, 31);
    int bit63 = get_bit(*d, 63);
    d->bits[0] <<= 1;
    d->bits[1] <<= 1;
    d->bits[2] <<= 1;
    if (bit31) set_bit(d, 32, 1);
    if (bit63) set_bit(d, 64, 1);
  }
}

int last_bit(s21_decimal d) {
  int bit = 95;
  for (; bit >= 0 && get_bit(d, bit) == 0; bit--) {
  }
  return bit;
}
