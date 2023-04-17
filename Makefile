# Program name
NAME = minishell

# compiler
CC = cc

# compiler flags
CFLAGS = -Wall -Werror -Wextra -MMD -O3
INCLUDE = -Iinclude -Ilibft/include
LIB = -lreadline 
DEBUG_FLAGS = -g -O0 -fsanitize=address

# src files and path for them
VPATH = src
SRC = main.c smart_history.c readline_parser.c line_handling_func.c minishell_utils.c piping.c

# obj files and path for them
OBJ_DIR = obj
OBJ =$(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

#dependencies
DEP =$(OBJ:.o=.d)

#Libft
LIBFT_FLAGS = -Ilibft -Llibft -lft
LIBFT = libft.a

# rules
all: $(LIBFT) $(NAME)

debug: CFLAGS := $(filter-out -O3,$(CFLAGS))
debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(NAME)

$(LIBFT):
	make -C libft

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIB) $(LIBFT_FLAGS) -o $@
	
$(OBJ_DIR)/%.o: %.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	make clean -C libft

fclean: clean
	rm -rf $(NAME) /tmp/.minishell_history
	make fclean -C libft

re: fclean all 

.PHONY: all debug clean fclean re
-include $(DEP)