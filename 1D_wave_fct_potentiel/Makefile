NAME = potential 
SRCS = potential.c inifinit_potentiel.c
OBJS = $(SRCS:.c=.o)
CC = gcc 
CFLAFGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAFGS) $(OBJS) -lm -o $(NAME)
%.o:%.c potential.h 
	$(CC) $(CFLAFGS) -c $< -o $@

clean:
	rm -rf $(OBJS) 
fclean: clean
	rm -rf $(NAME) *.csv  
re: fclean all 

sim:
	./$(NAME) && python3 sim.py


.PHONY: re sim fclean clean all 