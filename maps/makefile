CC = g++
CFLAGS = -std=c++11 -Wall -c
LFLAGS = -std=c++11 -Wall
DEPS = ../utils/csv_util.h

All: graph.o

%.o: %.cc %.h $(DEPS)
	$(CC) $(CFLAGS) -o $@ $<

graph_test: graph_test.o graph.o ../utils/csv_util.o
	$(CC) $(LFLAGS) -o $@ $^

.PHONY: test

test: graph_test
	./graph_test

.PHONY: clean

clean:
	\rm -f *.o *~