#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include "utils.h"
#include <stdio.h>

char *read_until_c(int fd, char e)
{
	char *ret;
	char *aux;
	char buff[2];
	int rb;

	buff[1] = 0;
	ret = ft_strdup("");
	while (1)
	{
		rb = read (fd, buff, 1);
		if (rb > 0)
		{
			aux = ft_strjoin(ret, buff, 0);
			free (ret);
			if (!aux)
				return (0);
			ret = aux;
			if (buff[0] == e)
				break ;
		}
		else if (!rb)
			break ;
		else
		{
			free (ret);
			return (0);
		}
	}
	return (ret);
}

int cursor_position(const int tty, int *const rowptr, int *const colptr)
{
	char *response;
	int i;
	int r;
	int c;

	//ft_putstr_fd(tty, "\033[6n");
	r = -1;
	c = -1;
	write(tty, "\033[6n", 4);
	response = read_until_c(tty, 'R');
	while (1)
	{
		if (ft_strlen(response) < 6)
			break ;
		if (response[0] != 27 || response[1] != '[')
			break ;
		i = 2;
		r = ft_atoi(response + i);
		while (ft_is_digit(response[i]))
			i++;
		if (response[i] != ';')
			break ;
		c = ft_atoi(response + ++i);
		if (rowptr)
			*rowptr = r - 1;
		if (colptr)
			*colptr = c - 1;
		break ;
	}
	free (response);
	if (c < 0)
		return (0);
	return (1);
}