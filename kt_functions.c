#include "kt_functions.h"
#include "utils.h"
#include "motion.h"

void	set_key_type(t_key *key)
{
	if (ISPRINTABLE(key->key[0]))
		key->type = KT_PRINTABLE;
	else if (key->key[0] == '\x1b')
	{
		if (key->key[1] == '[')
		{
			if (key->key[2] >= 'A' && key->key[2] <= 'D')
				key->type = KT_UP + (key->key[2] - 'A');
		}
	}
	else if (key->key[0] == 127)
		key->type = KT_DELETE;
	else if (key->key[0] == '\n')
		key->type = KT_EOL;
	else
		key->type = KT_UNRECOGNIZED;
}

int	get_key(t_key *key)
{
	read(0, key->key, 1);
	if (key->key[0] == 4 && !key->l.cursor_max)
		return (0);
	if (key->key[0] == '\x1b')
		read(0, key->key + 1, 3);
	set_key_type(key);
	return (1);
}

void	line_deletion(t_key *k)
{
	int	eol1;
	int	offset;
	int	c;
	int	r;
	int	i;

	if (!cursor_position(0, &r, &c))
		return ;
	offset = get_offset(k, c);
	eol1 = k->w.ws_col - offset - 1;
	goto_cursor(k, 0);
	tputs(tgetstr("dm", NULL), 0, ft_putchar0);
	i = 0;
	while (i <= eol1)
	{
		tputs(tgetstr("dc", NULL), 0, ft_putchar0);
		i++;
	}
	tputs(tgetstr("ed", NULL), 0, ft_putchar0);
	if (offset + k->l.cursor_max > k->w.ws_col)
		tputs(tgetstr("cd", NULL), k->w.ws_row - r, ft_putchar0);
}

int	ft_save(t_key *key, const char *str)
{
	if (key->h.hist[key->h.pos])
		free(key->h.hist[key->h.pos]);
	key->h.hist[key->h.pos] = ft_strdup(str);
	return (1);
}

int	ft_manage_key(t_key *key)
{
	int	(*functptr[KT_FUNCNUM])(t_key*);

	functptr[0] = kf_unrec;
	functptr[1] = kf_print;
	functptr[2] = kf_move;
	functptr[3] = kf_updown;
	functptr[4] = kf_updown;
	functptr[5] = kf_move;
	functptr[6] = kf_move;
	functptr[7] = kf_del;
	functptr[8] = kf_eol;
	return (functptr[key->type](key));
}
