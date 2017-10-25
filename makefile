CC= gcc
CFLAGS= -W -Wall -Werror
LDFLAGS=
INCLUDES = -I "./include"
EXEC= bin/main.out bin/demon
SRC= $(wildcard src/*.c)
OBJ= $(patsubst src/%.c,obj/%.o,$(SRC))

all: $(EXEC)

bin/main.out: obj/main.o obj/dossiers.o obj/utilitaires.o
	$(CC) $(INCLUDES) -o $@ $^ $(LDFLAGS)

bin/demon: obj/demon.o
	$(CC) $(INCLUDES) -o $@ $^ $(LDFLAGS)

obj/%.o: src/%.c
	$(CC) $(INCLUDES) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	rm -rf $(OBJ)

mrproper: clean
	rm -rf $(EXEC)
