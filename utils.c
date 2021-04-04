#include "utils.h"
#include <unistd.h>
#include <stdlib.h>

char *ft_strdup(const char *str)
{
	int i;
	char *ret;

	i = 0;
	while(str[i])
		i++;
	if (!(ret = (char *)malloc(i + 1)))
		return (0);
	i = -1;
	while (str[++i])
		ret[i] = str[i];
	ret[i] = 0;
	return (ret);
}

unsigned int ft_strlen(const char *str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void ft_putstr_fd(int fd, const char *str)
{
	int i;
	
	i = -1;
	while (str[++i])
		write(fd, str + i, 1);
}