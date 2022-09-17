#include "./../s21_decimal.h"

//  TODO [getBit] Можно ли здесь использовать оператор %?
int getBit(s21_decimal value, int bit) {
  return !!(value.bits[bit / 32] & (1u << (bit % 32)));
}

int getBitLast(s21_decimal value) {
  int bitLast = 95;
  for (; (bit >= 0) && (getBit(value, bitLast) == 0); bitLast -= 1);
  return bitLast;
}

//  TODO [setBit] Можно ли здесь использовать операторы / и %?
s21_decimal * setBit(s21_decimal * value, int pos, int bit) {
  if (pos / 32 < 4 && bit) 
    value->bits[pos / 32] |= (1u << (pos % 32));
  else if (pos / 32 < 4 && !bit) 
    value->bits[pos / 32] &= ~(1u << (pos % 32));
  return value;
}

//  TODO [addBit] Что делает эта функция?
//  TODO [addBit] Что значит переменная fres?
//  TODO [addBit] Можно ли IF на 38 строке убрать из цикла?
int addBit(s21_decimal value_1, s21_decimal value_2, s21_decimal * result) {
  int fres = 0, exp = 0;
  for (int i = 0; i < 96; i += 1) {
    int bitValue_1 = getBit(value_1, i), bitValue_2 = getBit(value_2, i);

    if (!bitValue_1 && !bitValue_2)
      exp ? setBit(result, i, (exp = 0) + 1) : setBit(result, i, 0);
    else if (bitValue_1 != bitValue_2)
      exp ? setBit(result, i, (exp = 1) - 1) : setBit(result, i, 1);
    else
      exp ? setBit(result, i, (exp = 1)) : setBit(result, i, (exp = 1) - 1);

    if (i == 95 && exp == 1 && !getBit(value_1, 97) && !getBit(value_2, 97))
      fres = 1;
  }
  return fres;
}

int getScale(s21_decimal value) { 
  return (char) (value.bits[3] >> 16);
}

//  TODO [setScale] Обязательно ли вызывать setSign в конце функции?
s21_decimal * setScale(s21_decimal * value, int scale) {
  int sign = getSign(*value) ? 1 : 0;
  if (scale > 0 && scale < 28) {
    value->bits[3] &= ~(0xFF << 16);
    value->bits[3] |= scale << 16;
  }
  if (sign) setSign(value, 1);
  return value;
}

//  TODO [increaseScale] Что проверяяет второй IF?
s21_decimal * increaseScale(s21_decimal * value, int shift) {
  if (getScale(*value) + shift < 29) {
    setScale(value, getScale(*value) + shift);
    s21_decimal auxValue_1 = *value, auxValue_2 = *value;
    if (addBit(*leftShift(&auxValue_1, 1), *leftShift(&auxValue_2, 3), value))
        setBit(value, 96, 1);
  }
  return value;
}

//  TODO [decreaseScale] Это точно работает правильно? Напрягает как используется переменная overflow
s21_decimal * decreaseScale(s21_decimal * value, int shift) {
  for (int y = 0; y < shift; y += 1) {
    unsigned long long overflow = value->bits[2];
    for (int x = 2; x >= 0; x -= 1) {
      value->bits[x] = overflow / 10;
      overflow = (overflow % 10) * (UINT_MAX + 1) + value->bits[i];
    }
  }
  setScale(value, (getScale(*value) - shift));
  return value;
}

//  TODO [alignmentScale] Первоначально функция возвращала INT, но это неиспользовалось.
//  TODO [alignmentScale] Здесь лучше внимательно изучить переписанный первый цикл, сравнить со старым.
void alignmentScale(s21_decimal * value_1, s21_decimal * value_2) {
  if (getScale(*value_1) != getScale(*value_2)) {
    if (getScale(*value_1) < getScale(*value_2)) {
      alignmentScale(value_2, value_1);
    } else {
      int scaleLow = getScale(*low), scaleHigh = getScale(*high);
      for (; scaleHigh - scaleLow; scaleLow += 1) {
        if (!getBit(*low, 96))
          increaseScale(value_2, 1);
        break;
      }

      // TODO [alignmentScale] Необходимо обсудить V
      // Если я все правильно понимаю, то реализовать
      // это можно одной строкой в цикле выше.
      s21_decimal * high = value_1, * low = value_2;
      int diff_scale = scaleHigh - scaleLow;
      while (scaleHigh - scaleLow) {
          decreaseScale(high, diff_scale);
          setScale(high, scaleLow);
          diff_scale--;
      }
    }
  }
}

int getSign(s21_decimal value) {
  return !!(value.bits[3] & (1u << 31));
}

//  TODO [setSign] Как sign может прийти -1? Нашел такой прикол в s21_from_float_to_decimal, s21_from_int_to_decimal, s21_negate
s21_decimal * setSign(s21_decimal * value, int sign) {
  value->bits[3] = (sign == -1) ? (value->bits[3] | (1u << 31)) : (value->bits[3] & ~(1u << 31));
  return value;
}

//  TODO [convert] Без понятий что делает эта функция
s21_decimal * convert(s21_decimal * value) {
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal add = {{1, 0, 0, 0}};
  for (int x = 0; x < 3; x += 1)
    value->bits[x] = ~value->bits[x];
  addBit(*value, add, &result);
  for (int x = 0; x < 3; x += 1)
    value->bits[x] = result.bits[x];
  setBit(value, 97, 1);
}

// TODO [isNull] Что вернет res если value не существует? Что тут вообще происходит?
// Мы получаем значение, помещаем его в новую память и спрашиваем существует ли эта память?
int isNull(s21_decimal value) {
  int res;
  s21_decimal *ptr = &value;
  if (ptr) {
      res = !value.bits[0] && !value.bits[1] && !value.bits[2];
  }
  return res;
}

int getFloatExp(float * value) {
  return ((*((int *) value) & ~(1u << 31)) >> 23) - 127;
}

// TODO [leftShift] Нужны ли тут переменные last_bit_1 и last_bit_2?
s21_decimal * leftShift(s21_decimal * value, int shift) {
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
    return value;
}
