#ifndef LINE_EDITION_H
# define LINE_EDITION_H
# define PROMPT "prompt"
# define PROMPT_SIZE 6

	typedef struct 		s_line
	{
		unsigned int 	cursor;
		unsigned int	cursor_max;
		char			*str;
		int				(*cursor_advance)(struct s_line*);
		int				(*cursor_back)(struct s_line*);
		int				(*cursor_delete)(struct s_line*);
		struct s_line 	*(*write)(struct s_line*, char *);
		struct s_line	*(*reset)(struct s_line*);
		unsigned int	err;
	}					t_line;

	//public
	int line_edition_loop(void *, const char *str, int (*hook)(void *, char *));
	//internal
	void set_term_basic(void);
	void set_term_specific(void);
	int line_advance_cursor(t_line *);
	int line_back_cursor(t_line *);
	int line_delete_cursor(t_line *);
	t_line *line_reset(t_line *);
	t_line *line_write(t_line *, char *str);
	t_line new_line(void);

#endif