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

void set_key_type(t_key *key)
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

void get_key(t_key *key)
{
	read(0, key->key, 1);
	if (key->key[0] == '\x1b')
		read(0, key->key + 1, 3);
	set_key_type(key);
}

int kf_unrec(t_key *k)
{
	(void)k;
	return (1);
}

/*
*	Rehacer estas funciones para q usen ft_insertstr
*	modificar ft_insertstr para que el cursor se le pase como 
*	*int, de manera que se actualize dentro de la propia función
*/

void	check_history(t_key *k)
{
	ft_putstr_fd(2, "ME CAGUEN TODO");
	dprintf(2, "\npos -> %i\ni -> %i\nlast -> %i\nlimits\nup: %i |||||| down: %i\n", k->h.pos, k->h.i, k->h.last, k->h.limit_up, k->h.limit_down);
	for (int i = 0; i < 5; i++)
		dprintf(2, "line %d: %s \n", i, k->h.hist[i]);
}

int kf_print(t_key *k)
{
	char *save;
	char buff[2];

	/*char *tmp;
	tmp = tgetstr("im", NULL);
	tputs(tmp, 1, &ft_putchar0);
	free(tmp);*/
	tputs(tgetstr("sc", NULL), 0, &ft_putchar0);
	save = ft_strdup(k->l.str + k->l.cursor);
	if (!save)
		return (1); //memory error
	buff[1] = 0;
	buff[0] = k->key[0];
	write(0, buff, 1);
	ft_putstr_fd(0, save); //revisar como escribimos
	tputs(tgetstr("rc", NULL), 0, &ft_putchar0);
	if ((get_col(k) + ft_strlen(save)) == k->w.ws_col)
		tputs(tgetstr("up", 0), 1, ft_putchar0);
	free (save);
	k->l.write(&k->l, buff);
	move_cursor_right(k);

	/*tmp = tgetstr("ei", NULL);
	tputs(tmp, 1, &ft_putchar0);
	free(tmp);*/
	return (1);
}

int kf_move(t_key *k)
{

	/*
	*	para todos estos problemas con los frees:
	*	hacer una struct con todos los strings ya buscados, al final los liberamos
	*	pero no los pediremos múltiples veces
	*	un array con una enum? -> cada string "le", "nd", etc será cada 'tipo'
	*
	*	??? parece que solo hay que no liberar, aunq el manual dice q si
	*	habrá que revisar los leaks
	*/

	if (k->type == KT_LEFT)
		move_cursor_left(k);
	else if (k->type == KT_RIGHT)
		move_cursor_right(k);
	return (1);
}

void line_deletion(t_key *k)
{
	int line_len;
	int lines_n;
	float f;

	line_len = ft_strlen(k->l.str) + ft_strlen(k->prompt);
	f = (float)line_len / (float)k->w.ws_col;
	lines_n = (int)f;
	if (f - lines_n != 0)
		lines_n++;
	if (lines_n > 0)
		tputs(tgetstr("up", 0), lines_n, ft_putchar0);
	tputs(tgetstr("cr", 0), 0, ft_putchar0);
	tputs(tgetstr("cd", 0), lines_n, ft_putchar0);
	k->l.reset(&k->l);
}

int kf_hist_print(t_key *k)
{
	int	c;

	line_deletion(k);
	tputs(tgetstr("cr", NULL), 1, &ft_putchar0);
	tputs(tgetstr("im", NULL), 1, &ft_putchar0);
	ft_putstr_fd(0, k->prompt);
	write(0, k->h.hist[k->h.pos], ft_strlen(k->h.hist[k->h.pos]));
	k->l.reset(&k->l);
	k->l.write(&k->l, k->h.hist[k->h.pos]);
	c = k->l.cursor_advance(&k->l);
	while (c)
		c = k->l.cursor_advance(&k->l);
	tputs(tgetstr("ei", NULL), 1, &ft_putchar0);
	return (1);
}

int	hs_move_cursor(t_key *k, t_motion m)
{
	if (m == MOTION_UP)
		k->h.pos--;
	else if (m == MOTION_DOWN)
		k->h.pos = (k->h.pos + 1) % 5;
	if (k->h.pos < 0)
		k->h.pos = 4;
	return (1);
}



int kf_updown(t_key *k)
{
	ft_save(k, k->l.str);
	if (k->type == KT_UP)
	{
		if (!k->h.limit_up)
		{
			k->h.limit_down = 0;
			hs_move_cursor(k, MOTION_UP);
			if (k->h.pos == k->h.i && (hs_move_cursor(k, MOTION_DOWN)))
				return (++k->h.limit_up);
			if (k->h.hist[k->h.pos])
				kf_hist_print(k);
			else
				hs_move_cursor(k, MOTION_DOWN);
		}
		return (1);
	}
	else
	{
		if (!k->h.limit_down)
		{
			k->h.limit_up = 0;
			hs_move_cursor(k, MOTION_DOWN);
			if (k->h.pos == k->h.i)
				k->h.limit_down++;
			if (k->h.hist[k->h.pos])
				kf_hist_print(k);
			return (1);
		}
	}
	return (1);
}

int kf_del(t_key *k)
{
	//funcion de deleteo en line_edition
	char *save;
	int i;
	int len;

	//delete en la consola
	if (k->l.cursor == 0)
		return (1);
	save = ft_strdup(k->l.str + k->l.cursor);
	if (!save)
		return (0);
	
	tputs(tgetstr("le", NULL), 1, &ft_putchar0);
	tputs(tgetstr("sc", NULL), 0, &ft_putchar0);
	tputs(tgetstr("cd", NULL), 0, &ft_putchar0);
	ft_putstr_fd(0, save);
	i = -1;
	len = ft_strlen(save);
	free(save);
	tputs(tgetstr("rc", NULL), 0, ft_putchar0);
	//delete en la consola
	k->l.cursor_delete(&k->l);
	return (1);
}

int ft_save(t_key *key, const char *str)
{
	if (key->h.hist[key->h.pos])
		free(key->h.hist[key->h.pos]);
	key->h.hist[key->h.pos] = ft_strdup(str);
	return (1);
}

int	ft_save_new(t_key *key, const char *str)
{
	key->h.pos = key->h.i;
	key->h.last = key->h.i;
	ft_save(key, str);
	key->h.i++;
	if (key->h.i > 4)
		key->h.i = 0;
	key->h.pos = key->h.i;
	key->h.limit_down = 1;
	key->h.limit_up = 0;
	return (1);
}

int kf_eol(t_key *k)
{
	int ret;
	char *tmp;
	
	tmp = ft_strdup(k->l.str);
	move_cursors_to_end(k);
	new_line_reset(k);
	if (*tmp)
		ft_save_new(k, tmp);  // poner condicion para no guardar strings vacias
	set_term_basic();
	ret = k->hook(k->data, tmp);
	set_term_specific();
	if (ret)
		write_prompt(k);
	return (ret);
}

int ft_manage_key(t_key *key)
{
	int (*functptr[KT_FUNCNUM])(t_key*);
	
	functptr[0] = kf_unrec; 	//kt_unrecognized
	functptr[1] = kf_print;		//kt_printable
	functptr[2] = kf_move;	//kt_char_func ?
	functptr[3] = kf_updown;	//kt_up
	functptr[4] = functptr[3];	//kt_down
	functptr[5] = functptr[2];	//kt_left
	functptr[6] = functptr[2];	//kt_right
	functptr[7] = kf_del;		//kt_delete
	functptr[8] = kf_eol; 		//kt_eol

	return (functptr[key->type](key));
}