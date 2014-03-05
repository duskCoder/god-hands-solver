
CC       ?= gcc
CFLAGS   += -W -Wall -std=gnu99 -Wextra
LDFLAGS  +=
NAME      = god_hands_solver
SRC       = god_hands.c

all: depend $(NAME)

depend: .depend

.depend: $(SRC)
	@$(RM) .depend
	@$(CC) $(CFLAGS) -MM $^ > .depend

include .depend

OBJ     = $(SRC:.c=.o)

$(NAME): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all depend clean fclean all re
