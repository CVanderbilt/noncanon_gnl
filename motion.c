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

void	goto_cursor(t_key *k, unsigned int dst)
{
	int	l;
	int	c;
	int	target_col;
	int	target_row;
	int	offset;

	if (dst > k->l.cursor_max || dst == k->l.cursor)
		return ;
	cursor_position(0, &l, &c);
	offset = get_offset(k, c);
	target_col = offset + dst % k->w.ws_col;
	target_row = (offset + dst) / k->w.ws_col;
	target_row = l - ((offset + k->l.cursor) / k->w.ws_col - target_row);
	tputs(tgoto(tgetstr("cm", NULL), target_col, target_row), 0, ft_putchar0);
	k->l.cursor = dst;
}

int	move_cursors_to_back(t_key *k)
{
	goto_cursor(k, 0);
	return (1);
}

int	move_cursors_to_end(t_key *k)
{
	goto_cursor(k, k->l.cursor_max);
	return (1);
}
