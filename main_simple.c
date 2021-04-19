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

void ft_fatal(const char *str, const char *str2)
{
	if (str) printf("%s", str);
	if (str2) printf("%s", str2);
	exit(1);
}

void set_term_basic(void)
{
	struct termios	tattr;

	tcgetattr(STDIN_FILENO, &tattr);
	tattr.c_lflag |= (ECHO | ICANON);
	tattr.c_oflag |= (OPOST);
	tcsetattr(STDIN_FILENO, TCSADRAIN, &tattr);
}

void set_term_specific(void)
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

	if (termtype == 0)
		ft_fatal("Specify a terminal type with `setenv TERM <yourtype>'.\n", 0);

	tgetent(NULL, termtype); //importante al parecer, >>>AVERIGUAR<<<
}

//int tgetnum (char *name);
//int tgetflag (char *name);
//char *tgetstr (char *name, char **area);

void ft_putchar(char c)
{
	write(0, &c, 1);
}



int line_edition_loop(void *data, const char *prompt, int (*hook)(void *, char *))
{
	set_term_specific();

	t_key key;

	key.cursor = 0;
	key.h = new_history();
	key.l = new_line();
	key.data = data;
	key.prompt = prompt;
	key.prompt_len = ft_strlen(prompt);
	write(0, key.prompt, key.prompt_len);
	key.hook = hook;
	while(1)
	{
		key.type = KT_UNRECOGNIZED;
		get_key(&key);
		if (!ft_manage_key(&key))
			break ;
		//ft_refresh(key.line);
	}
	for (int i = 0; i < 5; i++)
		free(key.h.hist[i]);
	write(0, "\nexiting...\n", 12);
	write(0, "last line: ", 11);
	ft_putstr_fd(0, key.l.str);
	free (key.l.str);
	write(0, "\n", 1);
	set_term_basic();
	return (0);
}