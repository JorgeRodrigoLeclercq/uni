# Variables y configuración  -----------------------------------------------------------

BIN_FILES  = servidor $(CLIENT_SOURCES:.c=)

CC = gcc

CPPFLAGS = -I$(INSTALL_PATH)/include

PEDANTIC_PARANOID_FREAK =       -O0 -g -Wshadow -Wcast-align \
				-Waggregate-return -Wmissing-prototypes -Wmissing-declarations \
				-Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations \
				-Wmissing-noreturn -Wredundant-decls -Wnested-externs \
				-Wpointer-arith -Wwrite-strings -finline-functions

REASONABLY_CAREFUL_DUDE =	-Wall -Wextra -O0 -g3 -ggdb3
NO_PRAYER_FOR_THE_WICKED =	-w -O3

LDFLAGS = -L$(INSTALL_PATH)/lib/
LDLIBS = -lpthread

CLIENT_SOURCES := $(wildcard clients/app-cliente-*.c)
CLIENT_OBJECTS := $(CLIENT_SOURCES:.c=.o)
CLIENT_BINARIES := $(CLIENT_SOURCES:.c=)

# Reglas de compilación  ---------------------------------------------------------------
all: clean
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

# Compilación de archivos  -------------------------------------------------------------
claves.o: claves.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -fPIC -c $<

proxy-mq.o: proxy-mq.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -fPIC -c $<

error.o: error.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -fPIC -c $<

lib/libclaves.so: proxy-mq.o error.o 
	mkdir -p lib/
	$(CC) -shared -o $@ $^

servidor: servidor-mq.o claves.o
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

clients/app-cliente-%: clients/app-cliente-%.o lib/libclaves.so
	$(CC) $(CPPFLAGS) $(LDFLAGS) -L./lib/ $^ $(LDLIBS) -lclaves -o $@

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clients/%.o: clients/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Reglas de limpieza ---------------------------------------------------------
clean:
	rm -f $(BIN_FILES) *.o clients/*.o
	rm -f *.gcno
	rm -f lib/libclaves.so
	rm -f /dev/mqueue/*
.PHONY : clean

# Ejecución ------------------------------------------------------------------
run-server: all servidor 
	./servidor&
.PHONY : run-server

run-client-%: clients/app-cliente-%
	./$<
.PHONY : run-client-%
