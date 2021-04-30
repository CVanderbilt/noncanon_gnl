#include <stdio.h>
#include <termcap.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <errno.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <term.h>
#include "utils.h"
#include "kt_functions.h"
#include "line_edition.h"
#include "motion.h"

int get_offset(t_key *k, int c)
{
	int offset;

	offset = c - k->l.cursor % k->w.ws_col;
	if (offset < 0)
		offset = k->w.ws_col + offset;
	return (offset);
}

unsigned short get_col(void)
{
	int col;

	cursor_position(0, 0, &col);
	return ((unsigned)col);
}

void goto_cursor(t_key *k, unsigned int dst)
{
	int l;
	int c;
	int target_col;
	int target_row;
	int offset;
	int teorical_row;

	if (dst > k->l.cursor_max || dst == k->l.cursor) //revisar si es >= o solo > y si ya está en el cursor destino
		return ;
	cursor_position(0, &l, &c);
	offset = get_offset(k, c);
	target_col = offset + dst % k->w.ws_col;
	teorical_row = (offset + k->l.cursor) / k->w.ws_col; //estamos en esta de nuestra fila/comando
	target_row = (offset + dst) / k->w.ws_col;
	//dprintf(2, "teorical %d, target %d, actual %d\n", teorical_row, target_row, l);
	int diff = teorical_row - target_row;
	target_row = l - diff;
	tputs(tgoto(tgetstr("cm", NULL), target_col, target_row), 0, ft_putchar0);
	k->l.cursor = dst;
}

int move_cursor_left(t_key *k)
{
	unsigned short col;
	int i;

	i = -1;
	col = get_col();
	if (!k->l.cursor_back(&k->l))
		return (0);
	if (col == 0)
	{
		tputs(tgetstr("up", 0), 1, ft_putchar0);
		while(++i < k->w.ws_col - 1)
			tputs(tgetstr("nd", 0), 1, ft_putchar0);
	}
	else
		tputs(tgetstr("le", 0), 1, ft_putchar0);
	return (1);
}

int move_cursor_right(t_key *k)
{
	unsigned short col;
	int i;

	i = -1;
	col = get_col();
	if (!k->l.cursor_advance(&k->l))
		return (0);
	if (col == k->w.ws_col - 1)
	{
		tputs(tgetstr("do", 0), 1, ft_putchar0);
		tputs(tgetstr("cr", 0), 1, ft_putchar0);
	}
	else
		tputs(tgetstr("nd", 0), 1, ft_putchar0);
	return (1);
}

int move_cursors_to_back(t_key *k)
{
	goto_cursor(k, 0);
	return (1);
	/*
	int i;

	i = 0;
	while (move_cursor_left(k))
		i++;
	return (i);*/
}

int move_cursors_to_end(t_key *k)
{
	goto_cursor(k, k->l.cursor_max);
	return (1);
	/*
	int i;

	i = 0;
	while (move_cursor_right(k))
		i++;
	return (i);
	*/
}
