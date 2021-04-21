#include <signal.h>

#ifndef SIG_HANDLER
# define SIG_HANDLER

void 	sig_handler(int signo);
int 	sig_init(void);

#endif