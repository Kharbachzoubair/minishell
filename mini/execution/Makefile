CC = cc

CFLAGS = -Wall -Wextra -Werror

NAME = exec.a

AR = ar

SRCS = builtins/pwd.c \
	builtins/echo.c \
	builtins/env.c \
	builtins/exit.c \
	builtins/export.c \
	builtins/unset.c \
	builtins/cd.c \
	external/external.c \
	helpers/helpers.c \
	execute.c \

OBJS = ${SRCS:.c=.o}

%.o : %.c shell.h
	${CC} -o $@ ${CFLAGS} -c $<

all : ${NAME}

${NAME} : ${OBJS}
	${AR} rcs ${NAME} ${OBJS}


clean :
	rm -rf ${OBJS} ${BOBJS}

fclean : clean
	rm -rf ${NAME}

re : fclean all
