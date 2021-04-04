#include "kt_functions.h"
#include <unistd.h>
#include <stdlib.h>
#include "utils.h"

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
	return (1);
}

int kf_print2(t_key *k)
{
	return (1);
}

int kf_updown(t_key *k)
{
	return (1);
}

int kf_del(t_key *k)
{
	return (1);
}

int kf_eol(t_key *k)
{
	return (1);
}

int ft_manage_key(t_key *key)
{
	int (*functptr[KT_FUNCNUM])(t_key*);
	
	functptr[0] = kf_unrec; 	//kt_unrecognized
	functptr[1] = kf_print;		//kt_printable
	functptr[2] = kf_print2;	//kt_char_func ?
	functptr[3] = kf_updown;	//kt_up
	functptr[4] = functptr[3];	//kt_down
	functptr[5] = functptr[2];	//kt_left
	functptr[6] = functptr[2];	//kt_right
	functptr[7] = kf_del;		//kt_delete
	functptr[8] = kf_eol; 		//kt_eol

	return (functptr[key->type](key));
}