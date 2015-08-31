CFLAGS=-Wall -g

all: ex1 ex3

clean:
	find . -regex ./ex[0-9]\* -exec rm -f {} +
	rm -rf *.dSYM
