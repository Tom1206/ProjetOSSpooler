CC= gcc
CFLAGS= -W -Wall -Werror
LDFLAGS=
INCLUDES = -I "./include"
EXEC= bin/demon bin/deposer
SRC= $(wildcard src/*.c)
OBJ= $(patsubst src/%.c,obj/%.o,$(SRC))

all: $(EXEC)

bin/demon: obj/demon.o obj/utilitaires.o
	$(CC) $(INCLUDES) -o $@ $^ $(LDFLAGS)

bin/deposer: obj/deposer.o obj/utilitaires.o
	$(CC) $(INCLUDES) -o $@ $^ $(LDFLAGS)

obj/%.o: src/%.c
	$(CC) $(INCLUDES) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	rm -rf $(OBJ)

mrproper: clean
	rm -rf $(EXEC)
