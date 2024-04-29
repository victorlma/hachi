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

.PHONY: debug release docs clean downloadtestsuite

$(BINPATH)-debug: $(SRCDIR)/main.c $(HEADERS) $(CFILES)
	mkdir -p $(BUILDIR)
	$(CC) $(DFLAGS) -o $(BINPATH)-debug $< $(LIBS)


$(BINPATH)-release: $(SRCDIR)/main.c $(HEADERS) $(CFILES)
	mkdir -p $(BUILDIR)
	$(CC) $(RFLAGS) -o $(BINPATH)-release $< $(LIBS)

debug: $(BINPATH)-debug
	@gdb -q ./$<

release: $(BINPATH)-release $(HEADERS) $(CFILES)
	@./$<


downloadtestsuite:
	mkdir -p tst
	cd tst ; \
	wget https://github.com/Timendus/chip8-test-suite/raw/main/bin/1-chip8-logo.ch8 ; \
	wget https://github.com/Timendus/chip8-test-suite/raw/main/bin/2-ibm-logo.ch8 ; \
	wget https://github.com/Timendus/chip8-test-suite/raw/main/bin/3-corax+.ch8 ; \
	wget https://github.com/Timendus/chip8-test-suite/raw/main/bin/4-flags.ch8 ; \
	wget https://github.com/Timendus/chip8-test-suite/raw/main/bin/5-quirks.ch8 ; \
	wget https://github.com/Timendus/chip8-test-suite/raw/main/bin/6-keypad.ch8 ; \
	wget https://github.com/Timendus/chip8-test-suite/raw/main/bin/7-beep.ch8 ; \
    wget https://github.com/Timendus/chip8-test-suite/raw/main/bin/8-scrolling.ch8 

clean:
	rm -rf $(BUILDIR)
