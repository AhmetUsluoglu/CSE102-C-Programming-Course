/*
** hw8_lib.c:
**
** The source file implementing library functions.
**
** Author: Yakup Genc. (c) 2018-2021
**
** Revision: 2021.04.22.19.55
** 
*/

#include <stdio.h>
#include "hw8_lib.h"

#define WORDSIZE 1
/*MAZE FUNCTIONS*/
void print_maze(cell_type maze[][8], int locX, int locY);
void current_location(cell_type maze[][8], int *locX, int* locY, cell_type player);
int move_player(unsigned int togo, cell_type maze[][8], int locX, int locY, cell_type player);
int up(cell_type maze[][8], int locX, int locY, cell_type player);
int up(cell_type maze[][8], int locX, int locY, cell_type player);
int down(cell_type maze[][8], int locX, int locY, cell_type player);
int left(cell_type maze[][8], int locX, int locY, cell_type player);
int right(cell_type maze[][8], int locX, int locY, cell_type player);
/*CLEAN FILE FUNCTIONS*/
int match (FILE* line, char* words_to_delete[WORDSIZE], int number_of_words,FILE* outfid);
int line (char* word, char* words_to_delete[WORDSIZE], int number_of_words,FILE* outfid);
int check_delete (char* word, char* words_to_delete[WORDSIZE], int number_of_words,FILE* outfid);
int check_char (char* word, char words_to_delete[]);
int find_size (char* word);
/*HANOI FUNCTIONS*/

/*Checks word at hand with the target word at hand(which should be deleted) and compares every char, if results 0 it means word is not on the deleted list.*/
int check_char (char* word, char words_to_delete[])
{	
	if(*word == '\0') return 1;
	if(*word == *words_to_delete) check_char (word + 1,words_to_delete+1);
	else return 0;
}

/*Delete or keep function, decidec wihch words will be send to print or delete.*/
int check_delete (char* word, char* words_to_delete[WORDSIZE], int number_of_words,FILE* outfid)
{
	int same;
	if (number_of_words == -1) return 0;
	/*Calls char check fuction to determine the fate of the word :D */
	same = check_char(word,words_to_delete[number_of_words]);
	if (same == 1) return 1;

	check_delete (word,words_to_delete, number_of_words - 1, outfid);
}

/*Match functions reads a word form the line then sends it to delete or keep function until it checks every word in line*/
int match(FILE* line, char* words_to_delete[WORDSIZE], int number_of_words, FILE* outfid)
{	
	int delete;
	char word[20];
	if (number_of_words == - 1) return 1;
	if(fscanf(line,"%s ",word) == EOF) return 0;
	delete = check_delete(word,words_to_delete,number_of_words - 1,outfid);

	/*If the resukt from check_delete is 0 it means the word should be printed*/
	if (delete == 0)	fprintf(outfid,"%s ",word);
		
	/*Calls itself over until every words are send to delete or keep function*/
	match (line,words_to_delete,number_of_words, outfid);
}

/*Program creates and opens a helper file and stores the line read from input file, then sends this file to recursive delete functions*/
int get_line(char* word, char* words_to_delete[WORDSIZE], int number_of_words, FILE* outfid)
{
	int deleted;
	char* linename = "line.txt";
	FILE* line = fopen("line.txt","w");
	if(line == NULL)
	{
		printf("could not open line.txt\n");
		return -1;
	}
	fputs(word,line);
	fclose(line);
	
	line = fopen("line.txt","r");
	if(line == NULL)
	{
		printf("could not open line.txt\n");
		return -1;
	}
	/*Calls recursive delete function until it reaches at the and of the read line, then it closes ands deletes helper file.*/
	deleted = match(line,words_to_delete,number_of_words,outfid);
	fclose(line);
	remove(linename);
}
/*In this function program reads a line from yhe input file and call recursive function to delete targeted words*/
void delete_words (FILE* infid, FILE* outfid, char* words_to_delete[WORDSIZE],  int number_of_words)
{
	char word[1001];
	int line ,deleted;
	
	if(fgets(word,1000,infid) == 0) return;
	/*After getting the line from input file program calls a function to handle the read line*/
	line = get_line(word,words_to_delete,number_of_words, outfid);
	/*At the end of every line program moves the next line in output file*/
	fprintf(outfid,"\n");
	/*This function calls itself until it reads all lines from input file*/
	delete_words (infid, outfid, words_to_delete, number_of_words);
}

/*This functions operates over all other clean file functions. We simply open our txt files and send pointers to recursive functions*/
void clean_file(char* infile, char * outfile, char* words_to_delete[WORDSIZE], int number_of_words)
{
	FILE* infid = fopen(infile, "r");
	FILE* outfid = fopen(outfile, "a");
	if(infid == NULL)
	{
		printf("Couldn't open the %s",infile);
		return;
	}
	if(outfid == NULL)
	{
		printf("Couldn't open the %s",outfile);
		return;
	}
	delete_words (infid, outfid, words_to_delete, number_of_words);
	printf("DONE\n");
	fclose(infid);
	fclose(outfid);
}

/*************************************** MAZE ***************************************/
/*This is the main faunction opeates over other maze related functions. This function takes movement inputs sends them to function, recieve an integer value, checks the value and halts the program.*/
int maze_move(cell_type maze[][8], cell_type player, move_type move)
{
	int locX = 0, locY = 0, reached;
	unsigned int togo;
	/*Printing maze and getting the location of the player*/
	print_maze(maze, 0, 0);
	current_location(maze, &locX, &locY, player);
	printf("Turn for %c. Reach T.\n", player);
	/*Getting the Direction to go for given player*/
	printf("Enter a direction to go (down:2, left:1, right:3, up:5):");
	scanf("%uud", &togo);
	printf("\n");

	/*Sending the movement way to the functions and controls the return value*/
	reached = move_player(togo,maze, locX, locY, player);
	/*value one indicates that target has reached and maze is solved*/
	if (reached == 1)
	{
		print_maze(maze, 0, 0);
		printf("\tPlayer %c won!\n", player);
		return 1;
	}
	/*Value 0 means the movement succefsul neither target reached nor the movement failed*/
	else if (reached == 0)
	{
		if (player == cell_p1)
			player = cell_p2;
		else player = cell_p1;
	}
	/*Value -1 means movement is unsuccesful*/
	else if (reached == -1) printf("\t==> Incorrect Move <==\n");

	return maze_move(maze, player, move);
}

/*Prints the maz until the end of the array maze[7][7] is reached*/
void print_maze(cell_type maze[][8], int locX, int locY)
{	
	if(locX + locY == 0) printf("\t");
	printf("%c ",maze[locX][locY]);
	if (locY != 7) locY++;
	/*Endo off the second index*/
	else if (locX != 7 && locY == 7)
	{
		printf("\n\t");
		locY = 0;
		locX++;
	}
	/*end of the both indexes*/
	else if (locX == 7 && locY == 7)
	{
		printf("\n\t");
		return;
	}
	/*Recalls itself again*/
	print_maze(maze, locX, locY);	
}

/*Finds The location of the current player*/
void current_location(cell_type maze[][8], int *locX, int* locY, cell_type player)
{
	if (maze[*locX][*locY] == player) return;
	/*If it's end of the index 1 then it inreases 1 then checks next index*/
	else if (*locX == 7 && *locY != 7)
	{
		(*locY)++;
		*locX = -1;
	}
	/*Increases until the end of the index*/
	(*locX)++;
	current_location(maze, locX, locY, player);
}

/*decides which function will be called after getting the movement and returns a value that indicates the operation result*/
int move_player(unsigned int togo, cell_type maze[][8], int locX, int locY, cell_type player)
{
	if(togo == move_down) 			return down(maze, locX, locY, player);
	else if(togo == move_up) 		return up(maze, locX, locY, player);
	else if(togo == move_left) 		return left(maze, locX, locY, player);
	else if(togo == move_right) 	return right(maze, locX, locY, player);		
	else if(togo != move_down && togo != move_up && togo != move_left && togo != move_right)
	{
		printf("\t==>Incorrect Direction<==\n");
		return 0;
	}	
}

/*Move player to the targeted way if that block is free or it's a target if it's not neither of them it will return -1*/
int up(cell_type maze[][8], int locX, int locY, cell_type player) 
{
	if(maze[locX-1][locY] == cell_free)
	{
		maze[locX-1][locY] = player;
		maze[locX][locY] = cell_free;
		return 0;
	}
	else if(maze[locX-1][locY] == cell_target)
	{
		maze[locX-1][locY] = player;
		maze[locX][locY] = cell_free;
		return 1;
	}
	else return -1;
}

/*Move player to the targeted way if that block is free or it's a target if it's not neither of them it will return -1*/
int down(cell_type maze[][8], int locX, int locY, cell_type player) 
{
	if(maze[locX+1][locY] == cell_free)
	{
		maze[locX+1][locY] = player;
		maze[locX][locY] = cell_free;
		return 0;
	}
	else if(maze[locX+1][locY] == cell_target)
	{
		maze[locX+1][locY] = player;
		maze[locX][locY] = cell_free;
		return 1;
	}
	else return -1;
}

/*Move player to the targeted way if that block is free or it's a target if it's not neither of them it will return -1*/
int left(cell_type maze[][8], int locX, int locY, cell_type player) 
{
	if(maze[locX][locY-1] == cell_free)
	{
		maze[locX][locY-1] = player;
		maze[locX][locY] = cell_free;
		return 0;
	}
	else if(maze[locX][locY-1] == cell_target)
	{
		maze[locX][locY-1] = player;
		maze[locX][locY] = cell_free;
		return 1;
	}
	else return -1;
}

/*Move player to the targeted way if that block is free or it's a target if it's not either of them it will return -1*/
int right(cell_type maze[][8], int locX, int locY, cell_type player) 
{
	if(maze[locX][locY+1] == cell_free)
	{
		maze[locX][locY+1] = player;
		maze[locX][locY] = cell_free;
		return 0;
	}
	else if(maze[locX][locY+1] == cell_target)
	{
		maze[locX][locY+1] = player;
		maze[locX][locY] = cell_free;
		return 1;
	}
	else return -1;
}

void towers_of_hanoi(char start_peg, char end_peg, char aux_peg, int n)
{
    printf("TO BE IMPLEMENTED\n");
}
