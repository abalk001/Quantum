NAME = bohr
SRCS = main.c utils.c specters.c wave_fct.c
OBJS = $(SRCS:.c=.o)
CC = gcc 
CFLAGS = -Wall -Wextra -Werror -g 
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -lm -o $(NAME)

%.o:%.c bohr.h
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	rm -rf $(OBJS)
fclean: clean
	rm -rf $(NAME) *.csv

re: fclean all 

sim: 
	./$(NAME) && python3 sim.py
.PHONY: re fclean all clean 
