PREFIX?=/usr/local
CFLAGS=-std=c11 -Wall -g -I${PREFIX}/apr/include/apr-1 -Iinclude/mpc
LDFLAGS=-L${PREFIX}/apr/lib -lapr-1 -ledit

mpc.o: include/mpc/mpc.c
	cc -c include/mpc/mpc.c -o mpc.o

my_objects :=  $(patsubst %.c,%.o,$(wildcard src/*.c))

stsh: $(my_objects) mpc.o
	cc $(CFLAGS) $(LDFLAGS) $(my_objects) mpc.o -o stsh

all: stsh

clean:
	rm stsh build/*
