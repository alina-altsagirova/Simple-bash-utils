CFLAGS = -Wall -Werror -Wextra

TARGET_GREP = s21_grep
GREP_SOURCES = $(wildcard grep/*.c)
GREP_OBJECTS = $(GREP_SOURCES:.c=.o)

TARGET_CAT = s21_cat
CAT_HEADERS = $(wildcard cat/*.h)
CAT_SOURCES = $(wildcard cat/*.c)
CAT_OBJECTS = $(addprefix cat/, $(notdir $(CAT_SOURCES:.c=.o)))

.PHONY: all clean test_grep test_cat valgrind_grep valgrind_cat

all: $(TARGET_GREP) $(TARGET_CAT)

$(TARGET_GREP): $(GREP_OBJECTS)
	gcc $(CFLAGS) -o $@ $^

s21_grep/%.o: grep/%.c grep/grep.h
	gcc $(CFLAGS) -c $< -o $@

$(TARGET_CAT): $(CAT_OBJECTS)
	gcc $(CFLAGS) -o $@ $^

s21_cat/%.o: s21_cat/%.c $(CAT_HEADERS)
	gcc $(CFLAGS) -c $< -o $@

test_grep:
	./bash_test/test.sh grep

test_cat:
	./bash_test/test.sh cat

valgrind_grep:
	CHECK_VALGRIND=1 ./bash_test/test.sh grep

valgrind_cat:
	CHECK_VALGRIND=1 ./bash_test/test.sh cat

clean:
	rm -f grep/*.o $(TARGET_GREP)
	rm -f cat/*.o $(TARGET_CAT)