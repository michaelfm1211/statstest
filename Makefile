CFLAGS = -Wall -Wextra -Werror -pedantic -c
PROGS = 1propz 2propz

all: $(PROGS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

1propz: 1propz.o common.o
	$(CC) -o $@ $^

2propz: 2propz.o common.o
	$(CC) -o $@ $^

.PHONY: clean
clean:
	rm -rf *.o $(PROGS) *.latex *.aux *.log *.pdf *.dvi
