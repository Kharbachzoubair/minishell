# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -g

# Project name
NAME = minishell

# Directories
LIBFT_DIR = libft
EXEC_DIR = execution

OBJ_DIR = objs

# Include paths
INC = -I./parsing/includes -I./execution -I$(LIBFT_DIR)

# Libft
LIBFT = $(LIBFT_DIR)/libft.a
EXEC = $(EXEC_DIR)/exec.a

# Readline flags
READLINE_FLAGS = -lreadline

# Source files - adjust these paths to match your actual file structure
SRC_FILES = main.c \
			heredoc.c \
			parsing/srcs/lexer/tokenizer.c \
			parsing/srcs/lexer/token_utils.c \
			parsing/srcs/lexer/lexer_utils.c \
			parsing/srcs/expansion/expansions.c \
			parsing/srcs/expansion/expansions_utils.c \
			parsing/srcs/expansion/expansion_utils2.c \
			parsing/srcs/expansion/expansion_helper.c \
			parsing/srcs/parser/helper.c \
			parsing/srcs/parser/helper2.c \
			parsing/srcs/parser/quotes.c \
			parsing/srcs/commands/command_utils.c \
			parsing/srcs/commands/command.c \
			parsing/srcs/commands/word_handler.c \
			parsing/srcs/redirection/redirections.c \
			parsing/srcs/redirection/rediraction_util.c \
			parsing/srcs/redirection/redirection_handler.c \
			parsing/srcs/utils/error.c \
			parsing/srcs/utils/memory.c 

# Object files
OBJ_FILES = $(SRC_FILES:.c=.o)
OBJS = $(addprefix $(OBJ_DIR)/, $(OBJ_FILES))

# Rules
all: $(NAME)

$(NAME): $(OBJS)
	echo hey
	make -C $(LIBFT_DIR)
	make -C $(EXEC_DIR)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(EXEC) $(READLINE_FLAGS) -o $(NAME)

# Generic rule for all object files
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(LIBFT):
	make -C $(LIBFT_DIR)

$(EXEC):
	make -C $(EXEC_DIR)

clean:
	@make -C $(LIBFT_DIR) clean
	@make -C $(EXEC_DIR) clean
	rm -rf $(OBJ_DIR)

fclean: clean
	@make -C $(LIBFT_DIR) fclean
	@make -C $(EXEC_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
