#include <stdio.h>
#include <stdlib.h>

#define N 4
typedef enum direction{noway,left,right,up,down}direction;

int move(int puzzle[][N], int locX, int locY, int togo);
void fill_puzzle(int puzzle[][N]);
int random(int numbers[]);
void print_puzzle(int puzzle[][N]);
int puzzle_check(int puzzle[][N]);
direction toRight(int puzzle[][N], int locX, int locY);
direction toLeft(int puzzle[][N], int locX, int locY);
direction toUp(int puzzle[][N], int locX, int locY);
direction toDown(int puzzle[][N], int locX, int locY);

int main()
{	
	direction togo;
	int puzzle[N][N];
	int locX, locY;
	/*Setting up the game*/
	fill_puzzle(puzzle);
	print_puzzle(puzzle);
	if(N <= 2) printf("\n\n==>This Puzzle Is Unsolveable. N should be bigger than 2<==\n\n");
	if(N < 2) return 0;
	
	
	/*Getting the user inputs(location and direction) then calling the respective functions after checking the values*/
	while(1)
	{
		printf("Enter The Staring Point`s Row & Column.(Starts from 1)\n");
		scanf("%d %d", &locX, &locY);
		locX--,locY--;
		if(locX < 0 || locX > N || locY < 0 || locY > N) printf("\n\n==>Invalid Row & Column<==\n\n");
		printf("Enter The Direction.(left = 1 , right = 3 , up = 5, down = 2 , quit = 0)\n");
		scanf("%u", &togo);
		
		/*Sending the location and direction to function and if it returns 0 then program halts.*/
		if(move(puzzle, locX, locY, togo) == 0) return 0;
		
		/*Re-printing the puzzle after each movement then checking the puzzle if it`s solved or not*/
		print_puzzle(puzzle);
		if(puzzle_check(puzzle) == 1)
		{
			printf("\n\n==>CONGRATULATIONS<==\n\n");
			return 0;		
		}
	}
	return 0;
}
int move(int puzzle[][N], int locX, int locY, int togo)
{
	switch(togo)
		{
			case 0:
				printf("\n\nGoodbye\n\n");
				return 0;
				break;
			case 1:
				if (toLeft(puzzle, locX, locY) == noway) printf("\n==>There is NO WAY TO GO<==\n");
				break;
			case 3:
				if (toRight(puzzle, locX, locY) == noway) printf("\n==>There is NO WAY TO GO<==\n");
				break;
			case 5:
				if (toUp(puzzle, locX, locY) == noway) printf("\n==>There is NO WAY TO GO<==\n");
				break;
			case 2:
				if (toDown(puzzle, locX, locY) == noway) printf("\n==>There is NO WAY TO GO<==\n");
				break;
			default:
				printf("\n==>Invalid Direction<==\n");
				return 0;
		}
	return 1;	
}

/*Fills puzzle array with random numbers*/
void fill_puzzle(int puzzle[][N])
{
	int i, j,num = 1,x = 1;
	int numbers[N*N-1];
	/*Creating 1d control array which contains the numbers that will be used*/
	for(i = 0; i < N*N-1; i++)
	{	
		numbers[i] = i+1;
	}

	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
			/*At each index of the puzzle to ensure random numbers to not equal each other, program calls random function repedeatly if the return value is -1*/
			puzzle[i][j] = random(numbers);
			for(x = 0; x < N*N*2; x++)
			if(puzzle[i][j] == -1) puzzle[i][j] = random(numbers);
		}
	}
	/*Assigning zero (space) randomly to the puzzle*/
	puzzle[N-1][N-1] = 0;
	i = rand() % N, j = rand() % N;
	puzzle[N-1][N-1] = puzzle[i][j];
	puzzle[i][j] = 0;
}
/*Helps fill_puzzle to randomly assign numbers but not be equal to each other*/
int random(int numbers[])
{	
	int i, num = 1 + rand() % ((N*N)-1);
	/*If the resulting rand value is equal to one of the values in numbers array which contains all possible numbers, program resets the value in numbers array to 0 then return the number, generated from rand function*/
	for(i = 0; i < N*N-1; i++)
	{
		if(numbers[i] == num)
		{
			numbers[i] = 0;
			return num;
		}
	}
	/*Indicates failure to assign different random number, in this case this function will be called again*/
	return -1;
}
/*Printf puzzle correctly for every value of N form 1 to 10.*/
void print_puzzle(int puzzle[][N])
{	
	int i, j,x;
	printf("\n%dX%d Puzzle Game\n", N, N);
	printf("\n");
	
	for(i = 0; i < N; i++)
	{	/*Prints upper bounds of numbers*/
		for(x = 0; x < N; x++) printf("*****");
		printf("*\n");
		
		for(j = 0; j < N; j++)
		{
			if(puzzle[i][j] == 0) printf("*    ");
			else printf("* %-2d ",puzzle[i][j]);
		}
		printf("*\n");
	}
	/*Prints lowest bound of puzzle*/
	for(x = 0; x < N; x++) printf("*****");
	printf("*\n");
}
/*If every value of puzzle sorted from 1 to last , left to right, top to bottom correctly, if it's correct then function returns 1 ,else 0.(logical values)*/
int puzzle_check(int puzzle[][N])
{
	int i, j, x = 1, y = 1;
	/*X always equals to numbers in puzzle if the puzzle solved correctly except the last box which is 0 (space), y is the number of boxes that needs to be filled*/
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
			if (puzzle[i][j] != 0)
			{
				if (puzzle[i][j] == x) y++;
			}
			else x--;
			x++;
		}
	}
	/*Returns 1 if the numbers sorted correctly , space can be anywhere*/
	if (x == y) return 1;
	else return 0;
}
/*Moves numbers to right if there is space to the right else function return noway and does nothing*/
direction toRight(int puzzle[][N], int locX, int locY)
{
	int i, a,target,check = 0,temp;
	/*Finding the empty spot to the right of the location user inputs*/
	for(i = N - 1 ; i > locY && check == 0; i--)
	{
		if(puzzle[locX][i] == 0)
		{
			target = i;
			check = 1;
		}
	}
	/*When there is no empty space*/
	if (check == 0) return noway;
	/*Moves every number to 1 box right to itself.*/
	for(i = target - 1; i >= locY; i--)
	{
		puzzle[locX][i + 1]= puzzle[locX][i];
	}
	/*Moves empty space to the location user inputs*/
	puzzle[locX][locY] = 0;
	return right;
}
/*Moves numbers to left if there is space to the left, else function return noway and does nothing*/
direction toLeft(int puzzle[][N], int locX, int locY)
{	
	int i, a,target,check = 0,temp;
	/*Finding the empty spot to the left of the location user inputs*/
	for(i = 0 ; i < locY && check == 0; i++)
	{
		if(puzzle[locX][i] == 0)
		{
			target = i;
			check = 1;
		}
	}
	/*When there is no empty space*/
	if (check == 0) return noway;
	/*Moves every number to 1 box left to itself.*/
	for(i = target + 1; i <= locY; i++)
	{
		puzzle[locX][i - 1]= puzzle[locX][i];
	}
	/*Moves empty space to the location user inputs*/
	puzzle[locX][locY] = 0;
	return left;
}
/*Moves numbers to down if there is space to the down, else function return noway and does nothing*/
direction toDown(int puzzle[][N], int locX, int locY)
{	
	int i, a,target,check = 0,temp;
	/*Finding the empty spot to the left of the location user inputs*/
	for(i = N - 1 ; i > locX && check == 0; i--)
	{
		if(puzzle[i][locY] == 0)
		{
			target = i;
			check = 1;
		}
	}
	/*When there is no empty space*/
	if (check == 0) return noway;
	/*Moves every number to 1 box down to itself.*/
	for(i = target - 1; i >= locX; i--)
	{
		puzzle[i + 1][locY]= puzzle[i][locY];
	}
	/*Moves empty space to the location user inputs*/
	puzzle[locX][locY] = 0;
	return down;
}
/*Moves numbers to up if there is space to the up, else function return noway and does nothing*/
direction toUp(int puzzle[][N], int locX, int locY)
{	
	int i, a,target,check = 0,temp;
	
	/*Finding the empty spot to the left of the location user inputs*/
	for(i = 0 ; i < locX && check == 0; i++)
	{
		if(puzzle[i][locY] == 0)
		{
			target = i;
			check = 1;
		}
	}
	/*When there is no empty space*/
	if (check == 0) return noway;
	/*Moves every number to 1 box down to itself.*/
	for(i = target + 1; i <= locX; i++)
	{
		puzzle[i - 1][locY]= puzzle[i][locY];
	}
	/*Moves empty space to the location user inputs*/
	puzzle[locX][locY] = 0;
	return up;
}
