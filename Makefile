lval.o: src/lval.h src/lval.c
	cc -c src/lval.c

read.o: src/read.h src/read.c
	cc -c src/read.c

eval.o: src/eval.h src/eval.c
	cc -c src/eval.c

print.o: src/print.h src/print.c
	cc -c src/print.c

mpc.o: src/libs/mpc/mpc.h src/libs/mpc/mpc.c
	cc -c src/libs/mpc/mpc.c

stsh: lval.o read.o eval.o print.o mpc.o src/stsh.c
	cc src/stsh.c

