LIB = build/simplelib.a 

CFLAGS = -Wall -pedantic -Iinclude
CC = gcc

SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:src/%.c=build/%.o)
TEST_SRCS = $(wildcard tests/test_*.c)
TEST_BINS = $(TEST_SRCS:tests/%.c=build/%)

$(shell mkdir -p build)

.PHONY: all clean test

all: $(LIB) $(TEST_BINS)

$(LIB): $(OBJS)
	ar rcs $@ $^

build/%.o: src/%.c include/%.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_BINS): build/%: tests/%.c $(LIB)
	$(CC) $(CFLAGS) $< $(LIB) -o $@

test: $(TEST_BINS)
	@echo "Running all tests..."
	@for test in $(TEST_BINS); do \
		echo "Running $$test..."; \
		./$$test || exit 1; \
	done
	@echo "All tests passed."

clean:
	rm -rf build/

