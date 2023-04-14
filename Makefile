# Program name
NAME = minishell

# compiler
CC = cc

# compiler flags
CFLAGS = -Wall -Werror -Wextra -MMD -O3
INCLUDE = -I./include
LIB = -lreadline 
DEBUG_FLAGS = -g -O0 -fsanitize=address

# src files and path for them
VPATH = src
SRC = main.c smart_history.c ft_strlcpy.c get_next_line.c get_next_line_utils.c\
readline_parser.c \

# obj files and path for them
OBJ_DIR = obj
OBJ =$(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

#dependencies
DEP =$(OBJ:.o=.d)

# rules
all: $(NAME)

debug: CFLAGS := $(filter-out -O3,$(CFLAGS))
debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIB) -o $@
	
$(OBJ_DIR)/%.o: %.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: 	clean
	rm -rf $(NAME) /tmp/.minishell_history

re: fclean all 

.PHONY: all debug clean fclean re
-include $(DEP)