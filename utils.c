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

char *ft_append(const char *s1, const char *s2)
{
    int i;
    int j;
    char *ret;
    int error;

    error = 0;
    if (!s1 || !s2)
        return (0);
    if (!(ret = (char*)malloc(ft_strlen(s1) + ft_strlen(s2) + 1)))
        error = 1;
    i = -1;
    j = -1;
    while (s1[++i] && !error)
        ret[i] = s1[i];
    while (s2[++j] && !error)
        ret[i + j] = s2[j];
    if (!error)
        ret[i + j] = 0;
    return (ret);
}

char *ft_strjoin(char *s1, char *s2, char c)
{
    char *ret;

    ret = ft_append(s1, s2);
    if (c == 'L' || c == 'B')
        free (s1);
    if (c == 'R' || c == 'B')
        free (s2);
    return (ret);
}
int ft_putchar0(int c)
{
	return (write(0, &c, 1));
}

static int	ft_dgts(long int n)
{
	int div;
	int i;

	div = 1;
	i = 0;
	n = n < 0 ? n * -1 : n;
	if (n < 10)
		return (1);
	while (n > 0)
	{
		n /= 10;
		i++;
	}
	return (i);
}

char		*ft_itoa(int n)
{
	int			dgts;
	int			min;
	long int	n2;
	char		*p;

	dgts = n >= 0 ? ft_dgts(n) : ft_dgts(n) + 1;
	if (!(p = malloc(dgts + 1)))
		return (0);
	min = n >= 0 ? 0 : 1;
	p[0] = '-';
	p[dgts] = 0;
	n2 = n;
	n2 *= n >= 0 ? 1 : -1;
	while (--dgts >= min)
	{
		p[dgts] = n2 % 10 + 48;
		n2 /= 10;
	}
	return (p);
}