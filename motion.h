#ifndef MOTION_H
# define MOTION_H

unsigned short get_col(t_key *k);
void move_cursor_left(t_key *k);
void move_cursor_right(t_key *k);
void move_cursors_to_back(t_key *k);
void move_cursors_to_end(t_key *k);

#endif