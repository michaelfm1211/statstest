CFLAGS = -Wall -Wextra -Werror -pedantic -c

all: 1propz 2propz

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

1propz: 1propz.o common.o
	$(CC) -o $@ $^

2propz: 2propz.o common.o
	$(CC) -o $@ $^

.PHONY: clean
clean:
	rm -rf *.o 1propz *.latex *.aux *.log *.pdf *.dvi
