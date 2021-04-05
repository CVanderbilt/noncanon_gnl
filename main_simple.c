#include <stdio.h>
#include <termcap.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <errno.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <term.h>
#include "utils.h"
#include "kt_functions.h"
#include "line_edition.h"

#ifdef unix
static char term_buffer[2048];
#else
#define term_buffer 0
#endif

void ft_fatal(const char *str, const char *str2)
{
	if (str) printf("%s", str);
	if (str2) printf("%s", str2);
	exit(1);
}

//int tgetnum (char *name);
//int tgetflag (char *name);
//char *tgetstr (char *name, char **area);

void ft_putchar(char c)
{
	write(0, &c, 1);
}



int main()
{
	struct termios tty;

	if(tcgetattr(STDIN_FILENO, &tty) != 0)
    	printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
	tty.c_lflag &= ~ICANON; /* Canonical mode disabled */
	tty.c_lflag &= ~ECHO;	/* Disable echo */
	tty.c_oflag &= ~OPOST; /* Prevent special interpretation of output bytes (e.g. newline chars) */
	tty.c_cc[VMIN] = 1;
	tty.c_cc[VTIME] = 0;
	if (tcsetattr(STDIN_FILENO, TCSADRAIN, &tty) != 0)
    	printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));

	char *termtype = getenv ("TERM");
	int success;

	if (termtype == 0)
		ft_fatal("Specify a terminal type with `setenv TERM <yourtype>'.\n", 0);

	tgetent(NULL, termtype); //importante al parecer, >>>AVERIGUAR<<<

	int (*functptr[8])(char*, int); //array de funciones que llamar al tratar cada tipo de char
	//Hay tantas como tipos de key hay


	char buff[4];
	t_key key;
	//t_history h;
	key.line = ft_strdup("");
	key.cursor = 0;
	key.l = new_line();

	//h = hs_init();
	write(0, "prompt", 6);

	

	while(1)
	{
		key.type = KT_UNRECOGNIZED;
		get_key(&key);
		ft_manage_key(&key);
		//ft_refresh(key.line);
		if (key.key[0] == 'q')
			break ;
	}
	write(0, "\nexiting...\n", 12);
	write(0, "last line: ", 11);
	ft_putstr_fd(0, key.l->str);
	write(0, "\n", 1);

	struct termios	tattr;

	tcgetattr(STDIN_FILENO, &tattr);
	tattr.c_lflag |= (ECHO | ICANON);
	tattr.c_oflag |= (OPOST);
	tcsetattr(STDIN_FILENO, TCSADRAIN, &tattr);
	return (0);
}