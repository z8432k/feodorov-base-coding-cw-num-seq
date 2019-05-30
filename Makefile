PROGRAM = numseq
CFLAGS = -Wall -Wextra -ggdb -ansi -O0
LDLIBS =

default: bin/$(PROGRAM)

bin/$(PROGRAM): bin/$(PROGRAM).o bin/error.o bin/array.o
	$(CC) $^ $(LDLIBS) -o $@

bin/$(PROGRAM).o: src/$(PROGRAM).c
	$(CC) $(CFLAGS) -Wno-unused-parameter -o $@ -c $<

bin/error.o: src/error.c
	$(CC) $(CFLAGS) -o $@ -c $<

bin/array.o: src/array.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -fv bin/*.o bin/$(PROGRAM)