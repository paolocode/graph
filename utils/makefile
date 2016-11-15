OBJS = csv_util.o
TESTS = csv_util_test
CC = g++
CFLAGS = -Wall -c
LFLAGS = -Wall
DEPS = csv_util.h

All: $(OBJS) $(TESTS)

%.o: %.cc $(DEPS)
	$(CC) $(CFLAGS) -o $@ $<

csv_util_test: csv_util_test.o csv_util.o
	$(CC) $(LFLAGS) -o $@ $^

.PHONY: test

test: csv_util_test
	./csv_util_test

.PHONY: clean

clean:
	\rm *.o *~