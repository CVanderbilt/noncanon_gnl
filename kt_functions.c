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
	return (1);
}

/*
*	Rehacer estas funciones para q usen ft_insertstr
*	modificar ft_insertstr para que el cursor se le pase como 
*	*int, de manera que se actualize dentro de la propia función
*/

int kf_print(t_key *k)
{
	char buff[2];

	/*char *tmp;
	tmp = tgetstr("im", NULL);
	tputs(tmp, 1, &ft_putchar0);
	free(tmp);*/
	//tputs(tgetstr("im", NULL), 1, &ft_putchar0);
	buff[1] = 0;
	buff[0] = k->key[0];
	write(0, buff, 1);
	k->l->write(k->l, buff);
	k->l->cursor_advance(k->l, 1);
	/*tmp = tgetstr("ei", NULL);
	tputs(tmp, 1, &ft_putchar0);
	free(tmp);*/
	//tputs(tgetstr("ei", NULL), 1, &ft_putchar0);
	return (1);
}

int kf_move(t_key *k)
{
	char *tmp;

	/*
	*	para todos estos problemas con los frees:
	*	hacer una struct con todos los strings ya buscados, al final los liberamos
	*	pero no los pediremos múltiples veces
	*	un array con una enum? -> cada string "le", "nd", etc será cada 'tipo'
	*
	*	??? parece que solo hay que no liberar, aunq el manual dice q si
	*	habrá que revisar los leaks
	*/

	tmp = tgetstr(k->type == KT_LEFT ? "le" : "nd", NULL);
	if (k->type == KT_LEFT ? k->l->cursor_back(k->l, 1) : k->l->cursor_advance(k->l, 1))
	//	write(0, k->key, 4);
		tputs(tmp, 1, &ft_putchar0);
	//free(tmp);
	return (1);
}

int kf_updown(t_key *k)
{
	return (1);
}

int kf_del(t_key *k)
{
	//funcion de deleteo en line_edition
	char *tmp;
	char *save;
	int i;
	int len;

	//delete en la consola
	if (k->l->cursor == 0)
		return (1);
	save = ft_strdup(k->l->str + k->l->cursor);
	if (!save)
		return (0);
	
	tputs(tgetstr("le", NULL), 1, &ft_putchar0);
	tputs(tgetstr("cd", NULL), 0, &ft_putchar0);
	ft_putstr_fd(0, save);
	i = -1;
	len = ft_strlen(save);
	free(save);
	while (++i < len)
		tputs(tgetstr("le", NULL), 1, &ft_putchar0);
	//delete en la consola
	k->l->cursor_delete(k->l);
	return (1);
}
void ft_save(const char *str)
{

}

int kf_eol(t_key *k)
{
	int i;
	
	i = -1;
	ft_save(k->l->str);
	write(0, "\n", 1);
	while(++i < k->l->cursor + PROMPT_SIZE)
		tputs(tgetstr("le", NULL), 1, &ft_putchar0);
	ft_putstr_fd(0, PROMPT);
	k->l->reset(k->l);
	return (1);
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