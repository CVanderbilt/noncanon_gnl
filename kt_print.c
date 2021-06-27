#include "kt_functions.h"
#include "utils.h"
#include "motion.h"

int	kf_print(t_key *k)
{
	char	*save;
	char	buff[2];

	tputs(tgetstr("sc", NULL), 0, &ft_putchar0);
	save = ft_strdup(k->l.str + k->l.cursor);
	if (!save)
		return (1);
	int	row;
	int	col;
	cursor_position(0, &row, &col);
	buff[1] = 0;
	buff[0] = k->key[0];
	write(0, buff, 1);
	ft_putstr_fd(0, save);
	tputs(tgetstr("rc", NULL), 0, &ft_putchar0);
	int sobran = ft_strlen(save);
	sobran -= k->w.ws_col - col;
	if (sobran < 0)
		sobran = 0;
	sobran = (row + (sobran + k->w.ws_col - 1) / k->w.ws_col) + 1;
	if (!((col + ft_strlen(save)) % k->w.ws_col)
		&& k->l.cursor < k->l.cursor_max
		&& sobran >= k->w.ws_row)
		tputs(tgetstr("up", 0), 1, ft_putchar0);
	free (save);
	k->l.write(&k->l, buff);
	move_cursor_right(k);
	return (1);
}
