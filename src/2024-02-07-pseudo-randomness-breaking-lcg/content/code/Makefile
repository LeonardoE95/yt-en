CFLAGS=-ggdb

all:
	$(CC) $(CFLAGS) custom-lcg.c -o custom-lcg
	$(CC) $(CFLAGS) rand-lcg.c -o rand-lcg
	$(CC) $(CFLAGS) rand-lcg-period.c -o rand-lcg-period

clean:
	rm custom-lcg
	rm rand-lcg
	rm rand-lcg-period
