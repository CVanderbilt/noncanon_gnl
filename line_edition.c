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

int line_advance_cursor(t_line *l, unsigned int n)
{
	char *tmp;
	//write(0, "2k", 2);
	if (l->cursor < l->cursor_max)
	{
		l->cursor = l->cursor + 1;
		tmp = ft_itoa(l->cursor);
		ft_putstr_fd(1, "+1");
		ft_putstr_fd(1, tmp);
		ft_putstr_fd(1, "\n");
		free(tmp);
		return (1);
	}
	tmp = ft_itoa(l->cursor);
	ft_putstr_fd(1, "+0");
	ft_putstr_fd(1, tmp);
	ft_putstr_fd(1, "\n");
	free(tmp);
	//dprintf(2, "+cursor: %d\n", l->cursor);
	return 0;
}

int line_back_cursor(t_line *l, unsigned int n)
{
	//write(0, "1k", 2);
	char *tmp;
	if (l->cursor > 0)
	{
		l->cursor = l->cursor -1;
		tmp = ft_itoa(l->cursor);
		ft_putstr_fd(1, "-1");
		ft_putstr_fd(1, tmp);
		ft_putstr_fd(1, "\n");
		free(tmp);
		return (1);
	}
	tmp = ft_itoa(l->cursor);
	ft_putstr_fd(1, "-0");
	ft_putstr_fd(1, tmp);
	ft_putstr_fd(1, "\n");
	free(tmp);
	//dprintf(2, "-cursor: %d\n", l->cursor);
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
	tmp = ft_itoa(l->cursor_max);
	write(1, "cm: ", 4);
	ft_putstr_fd(1, tmp);
	write(1, "\n", 1);
	free(tmp);
	return (l);
}

int line_delete_cursor(t_line *l)
{
	int i;

	i = l->cursor - 1;
	if (i < 0)
		return (1);
	l->cursor = i;
	//h0
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

t_line *new_line(void)
{
	t_line *l;

	if (!(l = (t_line*)malloc(sizeof(t_line))))
		return (0);
	//new_line()->cursor_advance(l, 2);
	l->cursor = 0;
	l->cursor_max = 0;
	if (!(l->str = ft_strdup("")))
	{
		free (l);
		return (0);
	}
	l->cursor_advance = &line_advance_cursor;
	l->cursor_back = &line_back_cursor;
	l->write = &line_write;
	l->cursor_delete = &line_delete_cursor;
	l->reset = &line_reset;
	l->err = 0;
	return (l);	
}