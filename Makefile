CC=gcc
DFLAGS=-ggdb -Wall -Wextra --std=c99 --pedantic
RFLAGS=-O2
LIBS=`pkg-config --libs ncurses`
SRCDIR=src
BUILDIR=build
BINPATH=$(BUILDIR)/hachi
DOCSPATH=docs
HEADERS=$(wildcard $(SRCDIR)/*.h)
CFILES=$(HEADERS:.h=.c)

.PHONY: debug release docs clean $(HEADERS)

debug: $(BINPATH)-debug
	@gdb ./$<

release: $(BINPATH)-release $(HEADERS) $(CFILES)
	@./$<

$(BINPATH)-debug: $(SRCDIR)/main.c $(HEADERS) $(CFILES)
	mkdir -p $(BUILDIR)
	$(CC) $(DFLAGS) -o $(BINPATH)-debug $< $(LIBS)


$(BINPATH)-release: $(SRCDIR)/main.c $(HEADERS) $(CFILES)
	mkdir -p $(BUILDIR)
	$(CC) $(RFLAGS) -o $(BINPATH)-release $< $(LIBS)

clean:
	rm -rf $(BUILDIR)
