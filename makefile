CC= gcc
CFLAGS= -W -Wall -Werror
LDFLAGS=
INCLUDES = -I "./include"
EXEC= ./bin/main.out
SRC= $(wildcard src/*.c)
OBJ= $(patsubst src/%.c,obj/%.o,$(SRC))

all: $(EXEC)

bin/main.out: $(OBJ)
	$(CC) $(INCLUDES) -o $@ $^ $(LDFLAGS)

obj/%.o: src/%.c
	$(CC) $(INCLUDES) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	rm -rf $(OBJ)

mrproper: clean
	rm -rf $(EXEC)
