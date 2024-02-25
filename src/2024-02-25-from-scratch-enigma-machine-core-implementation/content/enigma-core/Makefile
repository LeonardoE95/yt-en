CFLAGS=-Wall -ggdb -std=c11 -pedantic

examples:
	$(CC) $(CFLAGS) examples/simple.c enigma.h -o examples/simple
	$(CC) $(CFLAGS) examples/cli.c enigma.h -o examples/cli -lreadline

clean:
	rm -f examples/simple
	rm -f examples/cli

.PHONY: examples
