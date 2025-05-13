# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -g

# Project name
NAME = minishell

# Directories
LIBFT_DIR = libft
OBJ_DIR = objs

# Include paths
INC = -I./parsing/includes -I./execution -I$(LIBFT_DIR)

# Libft
LIBFT = $(LIBFT_DIR)/libft.a

# Readline flags
READLINE_FLAGS = -lreadline

# Source files - adjust these paths to match your actual file structure
SRC_FILES = main.c \
			parsing/srcs/lexer/tokenizer.c \
			parsing/srcs/lexer/token_utils.c \
			parsing/srcs/lexer/lexer_utils.c \
			parsing/srcs/parser/parser.c \
			parsing/srcs/parser/expansions.c \
			parsing/srcs/parser/expansions_utils.c \
			parsing/srcs/parser/quotes.c \
			parsing/srcs/parser/redirections.c \
			parsing/srcs/parser/command_utils.c \
			parsing/srcs/utils/error.c \
			parsing/srcs/utils/memory.c \
			execution/builtins/pwd.c \
			execution/builtins/echo.c \
			execution/builtins/env.c \
			execution/builtins/export.c \
			execution/builtins/unset.c \
			execution/builtins/cd.c \
			execution/external/external.c \
			execution/helpers/helpers.c

# Object files
OBJ_FILES = $(SRC_FILES:.c=.o)
OBJS = $(addprefix $(OBJ_DIR)/, $(OBJ_FILES))

# Rules
all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(READLINE_FLAGS) -o $(NAME)

# Generic rule for all object files
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(LIBFT):
	@make -C $(LIBFT_DIR)

clean:
	@make -C $(LIBFT_DIR) clean
	rm -rf $(OBJ_DIR)

fclean: clean
	@make -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
