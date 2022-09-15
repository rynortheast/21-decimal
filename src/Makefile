
FLAGS = -Wall -Werror -Wextra -pedantic -std=c11

auxiliary = $(wildcard ./auxiliary/*.c)
convertors = $(wildcard ./convertors/*.c)
comparison = $(wildcard ./comparison/*.c)
arithmetics = $(wildcard ./arithmetics/*.c)
comparision = $(wildcard ./comparision/*.c)
other = $(wildcard ./other/*.c)

HEADER = $(wildcard ./*.h)
FILES = $(auxiliary) $(convertors) $(comparison) $(arithmetics) $(comparision) $(other)

all: s21_math.a

s21_math.a: 
	gcc $(FLAGS) $(FILES) s21_test.c
	# ar rc s21_math.a $(FILES_o)
	# rm -f $(FILES_o)

# test: s21_math.a $(SRC/main.c
# 	gcc $(FLAGS) $(SRC)/main.c -o test s21_math.a

rebuild:
	make clean
	make all

clean:
	rm -f s21_math.a test
