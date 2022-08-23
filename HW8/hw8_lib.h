/*
** hw8_lib.h:
**
** The header file declaring library functions.
**
** Author: Yakup Genc. (c) 2018-2021
**
** Revision: 2021.04.22.19.55
** 
*/

typedef enum {cell_wall = 35, cell_free = 32, cell_p1 = 65, cell_p2 = 66, cell_target = 84}cell_type;

typedef enum {move_left = 1, move_right = 3, move_up = 5, move_down = 2}move_type;

void clean_file(char* infile, char * outfile, char* words_to_delete[1], int number_of_words);


int maze_move(cell_type maze[][8], cell_type player, move_type move);


void towers_of_hanoi(char start_peg, char end_peg, char aux_peg, int n);
