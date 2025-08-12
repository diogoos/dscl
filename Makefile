LIB = build/libdscl.a 

CFLAGS = -Wall -pedantic -Iinclude
CC = gcc

SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:src/%.c=build/%.o)
TEST_SRCS = $(wildcard tests/test_*.c)
TEST_BINS = $(TEST_SRCS:tests/%.c=build/%)

BENCH_SRCS = $(wildcard tests/benchmark_*.c)
BENCH_BINS = $(BENCH_SRCS:tests/%.c=build/%)

$(shell mkdir -p build)

.PHONY: all clean test benchmark

all: $(LIB) $(TEST_BINS) $(BENCH_BINS)

$(LIB): $(OBJS)
	ar rcs $@ $^

build/%.o: src/%.c include/dscl/%.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_BINS): build/%: tests/%.c $(LIB)
	$(CC) $(CFLAGS) $< $(LIB) -o $@

$(BENCH_BINS): build/%: tests/%.c $(LIB)
	$(CC) $(CFLAGS) $< $(LIB) -o $@

test: $(TEST_BINS)
	@echo "Running all tests..."
	@for test in $(TEST_BINS); do \
		echo "Running $$test..."; \
		./$$test || exit 1; \
	done
	@echo "All tests passed."

benchmark: $(BENCH_BINS)
	@echo "Running all benchmarks..."
	@for bench in $(BENCH_BINS); do \
		echo "Running $$bench..."; \
		./$$bench || exit 1; \
	done

clean:
	rm -rf build/
