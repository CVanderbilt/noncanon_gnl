#ifndef MOTION_H
# define MOTION_H

unsigned short get_col(t_key *k);
int cursor_position(const int tty, int *const rowptr, int *const colptr);
int move_cursor_left(t_key *k);
int move_cursor_right(t_key *k);
int move_cursors_to_back(t_key *k);
int move_cursors_to_end(t_key *k);

#endif