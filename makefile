CC = gcc -std=c11

CFLAGS = -lm -I./include/

OFLAGS = -O2

GFLAGS = -g -Wall

SOURCES = ./src/*.c

all: Cards

Cards: ctags
	$(CC) $(SOURCES) $(CFLAGS) $(GFLAGS) -o Cards 

run:
	./Cards

clean:
	rm Cards

fresh: clean Cards run

optimized:
	$(CC) $(SOURCES) $(CFLAGS) $(OFLAGS) -o Cards

ctags: 
	ctags -R *
