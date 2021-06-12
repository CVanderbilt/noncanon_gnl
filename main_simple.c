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
#include "motion.h"

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void	set_term_basic(void)
{
	struct termios	tattr;

	tcgetattr(STDIN_FILENO, &tattr);
	tattr.c_lflag |= (ECHO | ICANON);
	tattr.c_oflag |= (OPOST);
	tcsetattr(STDIN_FILENO, TCSADRAIN, &tattr);
}

void	set_term_specific(void)
{
	struct termios	tty;
	char			*termtype;

	if (tcgetattr(STDIN_FILENO, &tty) != 0)
		printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
	tty.c_lflag &= ~ICANON;
	tty.c_lflag &= ~ECHO;
	tty.c_oflag &= ~OPOST;
	tty.c_cc[VMIN] = 1;
	tty.c_cc[VTIME] = 0;
	if (tcsetattr(STDIN_FILENO, TCSADRAIN, &tty) != 0)
		printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
	termtype = getenv ("TERM");
//	if (termtype == 0) creo que salta esto cuando stdin es un file
	tgetent(NULL, termtype);
}

void	ft_putchar(char c)
{
	write(0, &c, 1);
}

int	line_edition_loop_end(t_key *key)
{
	int	i;

	i = -1;
	while (++i < 5)
		free (key->h.hist[i]);
	free (key->l.str);
	set_term_basic();
	return (0);
}

int	line_edition_loop(
	void *data, const char *prompt, int (*hook)(void *, char *))
{
	t_key	key;

	set_term_specific();
	sig_init();
	set_wdata(&key);
	g_key = &key;
	key.h = new_history();
	key.l = new_line();
	key.data = data;
	key.prompt = prompt;
	key.prompt_len = ft_strlen(prompt);
	write(0, key.prompt, key.prompt_len);
	key.hook = hook;
	while (1)
	{
		key.type = KT_UNRECOGNIZED;
		if (!get_key(&key))
			break ;
		if (!ft_manage_key(&key))
			break ;
	}
	return (line_edition_loop_end(&key));
}
