#ifndef ULTRA_UTILS_H
# define ULTRA_UTILS_H

# include <unistd.h>
# include <stdlib.h>

/*
*	ESTO NO ES UN SYMLINK, ES UNA COPIA A LO CUTRE
*	HABRÁ QUE ARREGLARLO CUANDO PODAMOS.
*/

/*
*			string related functions
*/
char	*ft_strdup(const char *str);
char	*ft_strndup(const char *str, int n);
char	*ft_strjoin(char *s1, char *s2, char c);
char	*ft_append(const char *s1, const char *s2);
char	*ft_itoa(int n);

unsigned ft_strlen(const char *str);

int		ft_atoi(const char *str);
int		ft_strcmp(const char *s1, const char *s2);

/*
*			I/O related functions
*/
void	ft_putstr(const char *str);
void	ft_putstr_fd(int fd, const char *str);

/*
*			Checks
*/
int		ft_is_digit(char c);

/*
*			Frees
*/
void	*ft_free(void *ptr);
void	*ft_dummy(int n);
void	*ft_close(int fd);
#endif