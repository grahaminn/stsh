CFLAGS=-c -Wall -ledit

LDFLAGS=

lval.o: src/lval.c
	cc -c src/lval.c -o build/lval.o

read.o: src/read.c
	cc -c src/read.c -o build/read.o

eval.o: src/eval.c
	cc -c src/eval.c -o build/eval.o

print.o: src/print.c
	cc -c src/print.c -o build/print.o

mpc.o: src/libs/mpc/mpc.c
	cc -c src/libs/mpc/mpc.c -o build/mpc.o

stsh.o: src/stsh.c
	cc -c src/stsh.c -o build/stsh.o

stsh: mpc.o lval.o read.o eval.o print.o stsh.o
	cc -o stsh -ledit build/*.o

clean:
	rm stsh build/*
