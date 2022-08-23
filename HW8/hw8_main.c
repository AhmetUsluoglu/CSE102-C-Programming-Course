/*
** main.c:
**
** The test/driver program for the homework.
**
** Author: Yakup Genc. (c) 2018-2021
**
** Revision: 2021.04.22.19.55
** 
*/

#include <stdio.h>
#include "hw8_lib.h"


void test_clean_file () 
{
	int delnum = 3;
	char* input = "input.txt";
	char* output = "output.txt";
	char* delete[3] = {"tamir","elma","ahmet"};
	clean_file(input, output, delete, delnum);
}


void test_maze_move ()
{
    cell_type maze[8][8] = 
    {
        {cell_wall, cell_wall, cell_wall, cell_wall, cell_wall, cell_wall, cell_target,cell_wall},
        {cell_wall, cell_free, cell_free, cell_free, cell_wall, cell_free, cell_free, cell_wall},
        {cell_wall, cell_wall, cell_wall, cell_free, cell_wall, cell_free, cell_wall, cell_wall},
        {cell_wall, cell_free, cell_free, cell_p1,   cell_wall, cell_free, cell_free, cell_wall},
        {cell_wall, cell_free, cell_wall, cell_free, cell_free, cell_wall, cell_free, cell_wall},
        {cell_wall, cell_p2,   cell_wall, cell_wall, cell_free, cell_wall, cell_free, cell_wall},
        {cell_wall, cell_free, cell_free, cell_free, cell_free, cell_free, cell_free, cell_wall},
        {cell_wall, cell_wall, cell_wall, cell_wall, cell_wall, cell_wall, cell_wall, cell_wall}
    };
    maze_move(maze,cell_p1,move_up);
}



void test_towers_of_hanoi ()
{
	printf("TO BE IMPLEMENTED\n");
}


/*
** main function for testing the functions...
**
*/
int main(void) {
	test_clean_file ();
	/*test_maze_move ();
	/*test_towers_of_hanoi ();*/
	return (0);
} /* end main */
