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

void	ms_put_strn(t_key *k, const char *str, int n)
{
	unsigned int	col;
	unsigned int	len;
	int				i;

	col = get_col();
	len = ft_strlen(str);
	i = -1;
	while (str[++i] && i < n)
	{
		ft_logn("write char");
	//	getchar();
		write(0, str + i, 1);
	}
	if (!((col + len) % k->w.ws_col))
	{
		tputs(tgetstr("do", 0), 1, ft_putchar0);
		tputs(tgetstr("cr", 0), 1, ft_putchar0);
	}
}

int	get_offset(t_key *k, int c)
{
	int	offset;

	offset = c - k->l.cursor % k->w.ws_col;
	if (offset < 0)
		offset = k->w.ws_col + offset;
	return (offset);
}

unsigned short	get_col(void)
{
	int	col;

	cursor_position(0, 0, &col);
	return ((unsigned)col);
}

unsigned short	get_row(void)
{
	int	row;

	cursor_position(0, &row, 0);
	return ((unsigned)row);
}

int	move_cursor_left(t_key *k)
{
	int	row;
	int	col;
	int				i;

	i = -1;
	if (!cursor_position(0, &row, &col))
		return (1);
	if (!k->l.cursor_back(&k->l))
		return (0);
	if (col == 0)
	{
		if (row == 0)
		{
			tputs(tgetstr("sr", 0), 1, ft_putchar0);
			tputs(tgetstr("cr", 0), 1, ft_putchar0);
			int vindex = k->l.cursor - k->w.ws_col;
			int offset = k->l.last_offset;
			ft_log("vindex: "); ft_log(ft_itoa(vindex)); ft_log(", offset: "); ft_logn(ft_itoa(offset));
			//getchar();
			if (k->l.last_key == KT_UNRECOGNIZED || k->l.last_key == KT_EOL)
				offset = get_offset(k, col);
			//for (int i = 0; i < k->w.ws_col; i++)
			for (int i = 1; i <= k->w.ws_col; i++)
			{
				int aux = vindex + i;
				if (aux < 0)
				{
					if (aux + k->prompt_len >= 0)
					{
						ft_logn("escribe letra del prompt");
						write(1, k->prompt + aux + k->prompt_len, 1);
					}
					else
					{
						ft_logn("escribe un espacio");
						//getchar();
						write(0, " ", 1);
					}
				}
				else
				{
					ft_logn("escribe los chars");
				//	getchar();
					ms_put_strn(k, k->l.str + i + vindex, k->w.ws_col - i + 1);
					//k->l.cursor = k->w.ws_col - 1 - offset;
					break ;
	//				write(0, k->l.str + i + vindex, 1);
				}
				//getchar();
			}
			tputs(tgoto(tgetstr("cm", NULL), k->w.ws_col - 1, 0), 0, ft_putchar0);
			//va a escribir hasta width chars
			//offset + str hasta cursor 

		}
		else
		{
			tputs(tgetstr("up", 0), 1, ft_putchar0);
			while (++i < k->w.ws_col - 1)
				tputs(tgetstr("nd", 0), 1, ft_putchar0);
		}
	}
	else
		tputs(tgetstr("le", 0), 1, ft_putchar0);
	return (1);
}

int	move_cursor_right(t_key *k)
{
	unsigned short	col;
	int				i;

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
