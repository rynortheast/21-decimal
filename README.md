
# 21-decimal

<div align="center">PREVIEW_WILL_BE_ADDED_LATER.png</div>

***

Educational project s21_decimal from organization «School 21»
ᅠ
Implementation 21_decimal.h library in the C programming language. Library add the ability to work with the "decimal" type, which is not in the language standard. This format can be useful for financial calculations, for example, where calculation error typical of floating-point types are unacceptable.

<details>
<summary>[ SPOILER ] Overview of implemented code</summary>

### s21_decimal structure

```c
typedef struct {
    int bits[4];
} s21_decimal;
```

### Arithmetic Operators

| No. | Function | Description | Status |
| ------ | ------ | ------ | ------ |
| 1 | int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) | addition | ✅ |
| 2 | int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) | subtraction | ✅ |
| 3 | int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) | multiplication | ✅ |
| 4 | int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) | division | ✅ |
| 5 | int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) | modulo | ✅ |

### Comparison Operators

| No. | Function | Description | Status |
| ------ | ------ | ------ | ------ |
| 1 | int s21_is_less(s21_decimal, s21_decimal) | less than (<) | ☑️ |
| 2 | int s21_is_less_or_equal(s21_decimal, s21_decimal) | less than or equal to (<=) | ☑️ |
| 3 | int s21_is_greater(s21_decimal, s21_decimal) | greater than (>) | ☑️ |
| 4 | int s21_is_greater_or_equal(s21_decimal, s21_decimal) | greater than or equal to (>=) | ☑️ |
| 5 | int s21_is_equal(s21_decimal, s21_decimal) | equal to (==) | ☑️ |
| 6 | int s21_is_not_equal(s21_decimal, s21_decimal) | not equal to (!=) | ☑️ |

### Convertors and parsers

| No. | Function | Description | Status |
| ------ | ------ | ------ | ------ |
| 1 | int s21_from_int_to_decimal(int src, s21_decimal *dst) | from int | ☑️ |
| 2 | int s21_from_float_to_decimal(float src, s21_decimal *dst) | from float | ☑️ |
| 3 | int s21_from_decimal_to_int(s21_decimal src, int *dst) | to int | ☑️ |
| 4 | int s21_from_decimal_to_float(s21_decimal src, float *dst) | to float | ☑️ |

### Another functions

| No. | Function | Description | Status |
| ------ | ------ | ------ | ------ |
| 1 | int s21_floor(s21_decimal value, s21_decimal *result) | rounds a specified Decimal number to the closest integer toward negative infinity | ✅ |
| 2 | int s21_round(s21_decimal value, s21_decimal *result) | rounds a decimal value to the nearest integer | ✅ |
| 3 | int s21_truncate(s21_decimal value, s21_decimal *result) | returns the integral digits of the specified Decimal; any fractional digits are discarded, including trailing zeroes | ✅ |
| 4 | int s21_negate(s21_decimal value, s21_decimal *result) | returns the result of multiplying the specified Decimal value by negative one | ✅ |

</details>

— Developer: [RV-304 (@rynortheast)](https://github.com/rynortheast) ✅, [Artem Kain (@artemxgod)](https://github.com/artemxgod) ☑️  
— Thanks to 21-SCHOOL for provided assignment and special learning conditions ✌️🔥

## Usage

1. Clone this repository via
    - SSH `git@github.com:rynortheast/21-decimal.git` or
    - HTTPS `https://github.com/rynortheast/21-decimal.git`
2. Change code base if necessary
3. Run `make test` to build project and run main tests
4. Run `make personal` to build project and run personal code
5. Run `make s21_decimal.a` to build a static lib
