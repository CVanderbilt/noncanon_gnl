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
	else if (signo == SIGWINCH)
		set_wdata(&g_key->w);
}

int sig_init(void)
{
	if (signal(SIGINT, handler) == SIG_ERR)
  		return (0);
	if (signal(SIGWINCH, handler) == SIG_ERR)
		return (0);	
	if (signal(SIGQUIT, handler) == SIG_ERR)
  		return (0);
	return (1);
}

void set_wdata(struct winsize (*w))
{	
	ioctl(STDIN_FILENO, TIOCGWINSZ, w);
	//tputs(tgetstr("co", NULL), w->ws_col, ft_putchar0);
	//tputs(tgetstr("li", NULL), w->ws_row, ft_putchar0);
}