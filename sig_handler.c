#include "utils.h"
#include "kt_functions.h"
#include "sig_handler.h"
#include <stdio.h>
#include <sys/ioctl.h>

//ctrl-c sigint -> salto de linea reseteando la linea, no se guard
//ctrl-4 (ctrl-\) -> nada
//ctrl-d no es una señal, lo maneja el read

void handler(int signo)
{
	if (signo == SIGINT)
	{
		new_line_reset(g_key);
		write_prompt(g_key);
	}
}

int sig_init(void)
{
	if (signal(SIGINT, handler) == SIG_ERR)
  		return (0);
	
	return (1);
}

void set_wdata(struct winsize (*w))
{	
	ioctl(STDIN_FILENO, TIOCGWINSZ, w);
}