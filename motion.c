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

unsigned short get_col(t_key *k)
{
	return ((k->l.cursor + k->prompt_len) % k->w.ws_col);
}

int move_cursor_left(t_key *k)
{
	unsigned short col;
	int i;

	i = -1;
	col = get_col(k);
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
	col = get_col(k);
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
	int i;

	i = 0;
	while (move_cursor_left(k))
		i++;
	return (i);
}

int move_cursors_to_end(t_key *k)
{
	int i;

	i = 0;
	while (move_cursor_right(k))
		i++;
	return (i);
}
