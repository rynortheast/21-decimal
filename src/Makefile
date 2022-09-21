
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c11
GCOVFLAGS = -fprofile-arcs -ftest-coverage

HEADERS = s21_decimal.h $(wildcard ./HEADERS/*.c) 
SOURCES_ALL = $(wildcard ./auxiliary/*.c) 	\
	$(wildcard ./arithmetics/*.c)			\
	$(wildcard ./comparision/*.c)			\
	$(wildcard ./convertors/*.c)			\
	$(wildcard ./comparison/*.c)			\
	$(wildcard ./other/*.c) 				\

OBJ_LIBRARY := $(patsubst %.c, %.o, $(SOURCES_ALL))

%.o: %.c $(HEADER)
	gcc $(CFLAGS) -c $< -o $@

all: s21_decimal.a test gcov_report

s21_decimal.a: $(OBJ_LIBRARY) $(HEADERS)
	ar rcs s21_decimal.a $(OBJ_LIBRARY)
	ranlib s21_decimal.a
	rm -rf ./*/*.o

test: s21_decimal.a ./TESTS/SUITE_main.check
	mv ./TESTS/SUITE_main.check ./TESTS/SUITE_main.c
	gcc $(CFLAGS) ./TESTS/SUITE_main.c s21_decimal.a -o test.out -lcheck
	mv ./TESTS/SUITE_main.c ./TESTS/SUITE_main.check
	./test.out

gcov_report:
	mv ./TESTS/SUITE_main.check ./TESTS/SUITE_main.c
	gcc $(GCOVFLAGS) ./TESTS/SUITE_main.c $(SOURCES_ALL) -o report.out -lcheck
	./report.out
	gcov -f $(SOURCES_ALL)
	lcov -t "gcov_report" -o Coverage_Report.info -c -d .
	genhtml -o ./report Coverage_Report.info
	mv ./TESTS/SUITE_main.c ./TESTS/SUITE_main.check
	rm -f *.gcno *.gcda *.info report.out *.gcov
	open ./report/index-sort-f.html

rebuild: clean all

clean:
	rm -f *.o *.a *.gcno *.gcda *.info *.out *.gcov
	rm -rf ./report
