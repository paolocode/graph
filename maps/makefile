CC = g++
CFLAGS = -Wall -c
DEPS = ../utils/csv_util.h

All: graph.o

%.o: %.cc %.h $(DEPS)
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: clean

clean:
	\rm -f *.o *~