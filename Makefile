# Source files
SRC = main.c \
      tokenization/node_creation.c \
      tokenization/handling.c \
      tokenization/handle_quotes.c \
      parsing/creation.c \
      parsing/handling.c \
      parsing/env.c \
      parsing/quotation.c \
      parsing/expansion.c \
      execution/exec_cmd.c \
      execution/exec_pipe.c \
      execution/handel_heredoc.c \
      builtsin/builtsin.c \
      builtsin/echo.c \
      builtsin/exit.c \
      builtsin/env.c \
      builtsin/pwd.c \
      builtsin/unset.c \
      builtsin/cd.c \
      builtsin/env_utils.c \
      builtsin/export.c

# Object files
OBJS = ${SRC:.c=.o}

# Output name
NAME = minishell

# Library
LIBFTDIR = libft/
LIBFTFLAGS = -L ${LIBFTDIR} -lft
READLINEFLAGS = -lreadline
INCLUDE = ${LIBFTFLAGS} ${READLINEFLAGS}

# Compiler
RM = rm -f
GCC = gcc
CFLAGS = -Wall -Wextra -Werror -g -I.

# Targets
all: ${NAME}

.c.o:
	${GCC} ${CFLAGS} -c $< -o ${<:.c=.o}

${NAME}: ${OBJS}
	make -C ${LIBFTDIR}
	${GCC} ${CFLAGS} -o ${NAME} ${OBJS} ${INCLUDE}

clean:
	${RM} ${OBJS}
	@cd ${LIBFTDIR} && $(MAKE) clean

fclean: clean
	${RM} ${NAME}
	@cd ${LIBFTDIR} && $(MAKE) fclean

re: fclean all
