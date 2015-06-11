CFLAGS=-c -Wall -ledit

LDFLAGS=

lval.o: src/lval.c
	cc -c src/lval.c

read.o: src/read.c
	cc -c src/read.c

eval.o: src/eval.c
	cc -c src/eval.c

print.o: src/print.c
	cc -c src/print.c

mpc.o: src/libs/mpc/mpc.c
	cc -c src/libs/mpc/mpc.c

stsh.o: src/stsh.c
	cc -c src/stsh.c

stsh: mpc.o lval.o read.o eval.o print.o stsh.o
	cc -o stsh -ledit mpc.o lval.o read.o eval.o print.o stsh.o

clean:
	rm *.o stsh
