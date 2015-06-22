PREFIX?=/usr/local
CFLAGS=-std=c11 -Wall -g -I${PREFIX}/apr/include/apr-1 -Iinclude/mpc
LDFLAGS=-L${PREFIX}/apr/lib -lapr-1 -ledit

SOURCEDIR = src
BUILDDIR = build
EXECUTABLE = stsh

SOURCES = $(wildcard $(SOURCEDIR)/*.c)
OBJECTS = $(patsubst $(SOURCEDIR)/%.c, $(BUILDDIR)/%.o,$(SOURCES))

all: dir $(BUILDDIR)/$(EXECUTABLE)

dir:
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/$(EXECUTABLE): $(OBJECTS) $(BUILDDIR)/mpc.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(OBJECTS): $(BUILDDIR)/%.o : $(SOURCEDIR)/%.c
	$(CC) -c $(CFLAGS) $(LDFLAGS) $< -o $@

$(BUILDDIR)/mpc.o: include/mpc/mpc.c
	$(CC) -c include/mpc/mpc.c -o $(BUILDDIR)/mpc.o

clean:
	rm -f $(BUILDDIR)/*.o $(BUILDDIR)/$(EXECUTABLE)
