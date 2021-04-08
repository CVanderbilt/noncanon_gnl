#ifndef KT_FUNCTIONS_H
# define KT_FUNCTIONS_H
# define KT_FUNCNUM 9
# define ISPRINTABLE(c) (c > 31 && c < 127)
# include "line_edition.h"

typedef enum 	e_key_type
{
	KT_UNRECOGNIZED,
	KT_PRINTABLE,
	KT_CHAR_FUNC,
	KT_UP,
	KT_DOWN,
	KT_RIGHT,
	KT_LEFT,
	KT_DELETE,
	KT_EOL
}				t_key_type;

typedef struct 	s_key
{
	int 		chars;
	char 		key[4];
	t_key_type 	type;
	char		*line;
	unsigned	cursor;
	t_line		*l;
	//estas inicializaciones se harán a través de line_edition_main
	int		 	(*hook)(void *data, char *line); 	//hook function to call at eol find
	void		*data;								//data to be passed to hook function (unchanged)
	const char 	*prompt;							//prompt to be printed after each eol
	unsigned	prompt_len; 
}				t_key;

void set_key_type(t_key *key);
void get_key(t_key *key);
int ft_manage_key(t_key *key);



#endif