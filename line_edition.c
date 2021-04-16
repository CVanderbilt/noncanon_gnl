#include "line_edition.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

t_line *line_error(t_line *l)
{
	l->err = 1;
	return (l);
}

int line_advance_cursor(t_line *l)
{
	if (l->cursor < l->cursor_max)
	{
		l->cursor = l->cursor + 1;
		return (1);
	}
	return 0;
}

int line_back_cursor(t_line *l)
{
	if (l->cursor > 0)
	{
		l->cursor = l->cursor -1;
		return (1);
	}
	return 0;
}

t_line *line_write(t_line *l, char *str)
{
	char *tmp;
	char *tmp2;
	char *tmp3;

	
	if (!(tmp = ft_strdup(l->str + l->cursor))) //copiamos lo que hay a la derecha del cursor
		return line_error(l);
	l->str[l->cursor] = 0; //nulo en el cursor para parar de copiar ahí
	tmp2 = ft_strjoin(l->str, str, 'L'); //joineamos l->str(hasta cursor) con str a insertar
	if (!tmp2)
	{
		free (tmp);
		return (line_error(l));
	}
	tmp3 = ft_strjoin(tmp2, tmp, 0); //join de (prev+str) con lo q había a la derecha
	free (tmp);
	free(tmp2);
	if (!tmp3)
		return (line_error(l));
	l->str = tmp3;
	l->cursor_max = ft_strlen(l->str);
	return (l);
}

int line_delete_cursor(t_line *l)
{
	int i;

	i = l->cursor - 1;
	if (i < 0)
		return (1);
	l->cursor = i;
	while (l->str[i + 1])
	{
		l->str[i] = l->str[i + 1];
		i++;
	}
	l->str[i] = 0;
	l->cursor_max = ft_strlen(l->str);
	return (1);
}

t_line *line_reset(t_line *l)
{
	int i;

	i = -1;
	l->cursor = 0;
	l->cursor_max = 0;
	free (l->str);
	l->str = ft_strdup("");
	return (l);
}

t_line new_line(void)
{
	t_line l;

	l.cursor = 0;
	l.cursor_max = 0;
	if (!(l.str = ft_strdup("")))
	{
		l.err = 1;
		return (l);
	}
	l.cursor_advance = &line_advance_cursor;
	l.cursor_back = &line_back_cursor;
	l.write = &line_write;
	l.cursor_delete = &line_delete_cursor;
	l.reset = &line_reset;
	l.err = 0;
	return (l);	
}

t_history new_history(void)
{
	t_history h;

	h.i = 0;
	h.pos = 0;
	h.limit_up = 0;
	h.limit_down = 0;
	h.hist[0] = NULL;
	h.hist[1] = NULL;
	h.hist[2] = NULL;
	h.hist[3] = NULL;
	h.hist[4] = NULL;
	return (h);
}
