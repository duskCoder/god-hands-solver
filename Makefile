CC = gcc
CFLAGS = -Wall -W -std=c99 -g

god_hands_solver: god_hands.o
	$(CC) -o $@ $^ $(LDFLAGS)

god_hands.o: god_hands.c
	$(CC) -o $@ -c $< $(CFLAGS)

