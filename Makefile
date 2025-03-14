BIN_FILES  = servidor cliente

CC = gcc

CPPFLAGS = -I$(INSTALL_PATH)/include

# Warning flags -----------------------------------------------------------------
PEDANTIC_PARANOID_FREAK =       -O0 -g -Wshadow -Wcast-align \
				-Waggregate-return -Wmissing-prototypes -Wmissing-declarations \
				-Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations \
				-Wmissing-noreturn -Wredundant-decls -Wnested-externs \
				-Wpointer-arith -Wwrite-strings -finline-functions

REASONABLY_CAREFUL_DUDE =	-Wall -Wextra -O0 -g3 -ggdb3
NO_PRAYER_FOR_THE_WICKED =	-w -O3

LDFLAGS = -L$(INSTALL_PATH)/lib/
LDLIBS = -lpthread


# Diff compilation options -----------------------------------------------------
all: clean clean-queues
all: CFLAGS=$(NO_PRAYER_FOR_THE_WICKED)
all: $(BIN_FILES)
.PHONY : all

coverage : CFLAGS=$(REASONABLY_CAREFUL_DUDE) -fprofile-arcs -ftest-coverage
coverage : LDLIBS+=-lgcov
coverage : $(BIN_FILES)
.PHONY : coverage

debug : CFLAGS=$(REASONABLY_CAREFUL_DUDE)
debug : $(BIN_FILES)
.PHONY : debug

pedantic : CFLAGS=$(PEDANTIC_PARANOID_FREAK)
pedantic : $(BIN_FILES)
.PHONY : pedantic

# File compilation -------------------------------------------------------------
claves.o: claves.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -fPIC -c $<

proxy.o: proxy.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -fPIC -c $<

error.o: error.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -fPIC -c $<

lib/libclaves.so: claves.o proxy.o error.o 
	mkdir -p lib/
	$(CC) -shared -o $@ $^

cliente: cliente.o lib/libclaves.so
	$(CC) $(CPPFLAGS) $(LDFLAGS) -L./lib/ $^ $(LDLIBS) -lclaves -o $@

servidor: servidor.o servicios_server.o error.o
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $<

# Clean options ----------------------------------------------------------------
clean:
	rm -f $(BIN_FILES) *.o
	rm -f *.gcno
.PHONY : clean

clean-all: clean
	rm -f lib/libclaves.so
.PHONY : clean-all

clean-queues:
	rm -f /dev/mqueue/*
.PHONY : clean-queues

# Run options ------------------------------------------------------------------
run-servidor: all servidor 
	./servidor&
.PHONY : run-servidor

run-cliente: cliente
	./cliente
.PHONY : run-cliente