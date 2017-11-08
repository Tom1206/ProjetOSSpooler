CC= gcc
CFLAGS= -W -Wall
LDFLAGS=
INCLUDES= -I "./include"
EXEC= bin/demon bin/deposer bin/retirer bin/lister
SRC= $(wildcard src/*.c)
OBJ= $(patsubst src/%.c,obj/%.o,$(SRC))

all: $(EXEC)

bin/demon: obj/demon.o obj/utilitaires.o
	$(CC) $(INCLUDES) -o $@ $^ $(LDFLAGS)

bin/deposer: obj/deposer.o obj/utilitaires.o
	$(CC) $(INCLUDES) -o $@ $^ $(LDFLAGS)
	chmod 6777 $@

bin/retirer: obj/retirer.o obj/utilitaires.o
	$(CC) $(INCLUDES) -o $@ $^ $(LDFLAGS)
	chmod 6777 $@

bin/lister: obj/lister.o obj/utilitaires.o
	$(CC) $(INCLUDES) -o $@ $^ $(LDFLAGS)
	chmod 6777 $@

obj/%.o: src/%.c
	$(CC) $(INCLUDES) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	rm -rf $(OBJ)

mrproper: clean
	rm -rf $(EXEC)
	rm -rf ./data/spool/*
