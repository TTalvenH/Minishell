NAME = Minishell
CC = cc
CFLAGS = -Wall -Werror -Wextra -MMD
VPATH = src
SRC = main.c
OBJ_DIR = obj
OBJ =$(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))
DEP =$(OBJ:.o=.d)

.PHONY: all clean fclean re

all:$(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	
-include $(DEP)

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -g -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: 	clean
	rm -rf $(NAME)

re: fclean all 
