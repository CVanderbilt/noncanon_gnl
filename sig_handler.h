#ifndef SIG_HANDLER_H
# define SIG_HANDLER_H
# include <signal.h>

void	sig_handler(int signo);
int		sig_init(void);

#endif