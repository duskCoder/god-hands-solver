CC = gcc
CFLAGS = -Wall -W -std=c99 -g

god_hands_solver: main.o
	$(CC) -o $@ $^ $(LDFLAGS)

main.o: main.c
	$(CC) -o $@ -c $< $(CFLAGS)

