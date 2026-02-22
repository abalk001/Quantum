NAME = Quan_tun
SRCS = main.c 
OBJS = $(SRCS.c=.o)
CC = gcc 
CFLAGS = -Wall -Werror -Wextra 
EXLIB = -lm 

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(EXLIB) -o $(NAME)

%.o:%.c 
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	rm -rf $(OBJS)

fclean: clean 
	rm -rf $(NAME) *.csv

re: fclean all 

sim: all 
	./$(NAME)
	
.PHONY: all fclean re clean 
