# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ellaca-f <ericllaca@gmail.com>             +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/11/07 13:05:48 by eherrero          #+#    #+#              #
#    Updated: 2021/04/16 12:51:33 by ellaca-f         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=libncgnl.a

CC=gcc

CFLAGS=-Wall -Wextra -Werror

RM=rm -f

LDFLAGS=-L.

LDLIBS=-lft

HDDIRS=-I ../../includes/



SRC=kt_functions.c line_edition.c main_simple.c \
utils.c sig_handler.c motion.c ask_cursor_pos.c motion2.c\
kt_history.c kt_eol.c kt_del.c kt_print.c kt_move_unrec.c

OBJ=$(SRC:.c=.o)

OBJ2=$(SRC2:.c=.o) 

$(NAME):
	$(CC) $(CFLAGS) -c $(SRC) $(HDDIRS)
	ar rc $(NAME) $(OBJ)
	ranlib $(NAME)

$(EXTRA):

all: $(NAME)

clean:
	$(RM) $(OBJ) $(OBJ2)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: clean fclean all bonus