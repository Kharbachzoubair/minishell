CC = cc
CFLAGS = -Wall -Wextra -Werror -g


NAME = minishell


LIBFT_DIR = libft
OBJ_DIR = objs


INC = -I./parsing/includes -I./execution -I$(LIBFT_DIR)


LIBFT = $(LIBFT_DIR)/libft.a


READLINE_FLAGS = -lreadline


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
			parsing/srcs/parser/quotes.c \
			parsing/srcs/commands/command_utils.c \
			parsing/srcs/commands/command.c \
			parsing/srcs/commands/word_handler.c \
			parsing/srcs/redirection/redirections.c \
			parsing/srcs/redirection/rediraction_util.c \
			parsing/srcs/redirection/redirection_handler.c \
			parsing/srcs/utils/error.c \
			parsing/srcs/utils/memory.c \
			execution/builtins/pwd.c \
			execution/builtins/echo.c \
			execution/builtins/env.c \
			execution/builtins/exit.c \
			execution/builtins/export.c \
			execution/builtins/unset.c \
			execution/builtins/cd.c \
			execution/external/external.c \
			execution/helpers/helpers.c \
			execution/execute.c


OBJ_FILES = $(SRC_FILES:.c=.o)
OBJS = $(addprefix $(OBJ_DIR)/, $(OBJ_FILES))


all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(READLINE_FLAGS) -o $(NAME)


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
