# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eherrero <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/11/07 13:05:48 by eherrero          #+#    #+#              #
#    Updated: 2019/11/11 15:00:10 by eherrero         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=non_canon_gnl.a

CC=gcc

CFLAGS=-Wall -Wextra -Werror -O3

RM=rm -f

LDFLAGS=-L.

LDLIBS=-lft

HDDIRS=-I includes/



SRC=kt_functions.c line_edition.c main_simple.c utils.c

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