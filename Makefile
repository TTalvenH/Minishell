# Program name
NAME = minishell

# compiler
CC = cc

# compiler flags
CFLAGS = -Wall -Werror -Wextra -g -MMD -O0
INCLUDE = -Iinclude -Ilibft/include 
LIB = -lreadline 
DEBUG_FLAGS = -g -O0 -fsanitize=address

# src files and path for them
VPATH = src:src/builtins:src/readline_parser
SRC = main.c smart_history.c line_handling_func.c env_funcs.c env_helper_funcs.c minishell_utils.c piping.c cd.c pwd.c exit.c env.c export.c unset.c echo.c handle_builtins.c create_child.c readline_parser1.c readline_parser2.c readline_parser3.c readline_parser4.c readline_parser5.c

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
debug: all

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
	rm -rf $(NAME) $(HOME)/.minishell_history
	make fclean -C libft

re: fclean all 

.PHONY: all debug clean fclean re
-include $(DEP)