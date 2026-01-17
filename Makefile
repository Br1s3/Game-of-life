CC = gcc
RM = rm -rf

CFLAGS +=\
-Wall \
-Wextra \
-g3


all: main


main: main.c
	$(CC) $< -o $@ $(CFLAGS)

clean:
	$(RM) main
