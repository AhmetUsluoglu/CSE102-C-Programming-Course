/*
** main.c:
**
** The test/driver program for the homework.
**
** Author: Yakup Genc. (c) 2018-2021
**
** Revision: 2021.04.02.23.55
** 
*/

/* I DID NOT WRITE COMMENTS FOR FUNCTIONS IN MAIN EXCEPT FOR THE 'FIRST ONE', BEACUSE I DID NOT CHANGE ANYTHING IN OTHER FUNCTIONS*/

#include <stdio.h>
#include <math.h>
#include "hw5_lib.h"

void test_operate_polynomials () 
{
	double a3 = 0 , a2 = 0, a1 = 0, a0 = 0, b3 = 0, b2 = 0, b1 = 0, b0 = 0, temp = 0;
	int x = 0, i = 0, error = 0;
	char op = '*';

	/*Getting coefficients and exponentials for 1. polynomial*/
	printf("Enter Coefficients as follows (x3, a), (x2, b), (x1, c), (x0, d) .\n\n\n");
	printf("Enter Coefficients for 1.st polynomial = ");
	for(i = 0; i <= 3; i++)
	{
		if (i < 3)  scanf("(%d, %lf), ", &x, &temp);
		else		scanf("(%d, %lf)", &x, &temp);
		
		if(x == 3)	a3 = temp;
		if(x == 2)	a2 = temp;
		if(x == 1)	a1 = temp;
		if(x == 0)	a0 = temp;
		if(x < 0 || x > 3) error = 1;
	}
	getchar();
	if (error == 1) printf("==>Invalid Exponential Value! Must be between 0-3<==\n");
	
	/*Getting coefficients and exponentials for 2. polynomial*/
	printf("Enter Coefficients for 2.nd polynomial = ");
	for(i = 0; i <= 3; i++)
	{
		if (i < 3)  scanf("(%d, %lf), ", &x, &temp);
		else		scanf("(%d, %lf)", &x, &temp);
		
		if(x == 3)	b3 = temp;
		if(x == 2)	b2 = temp;
		if(x == 1)	b1 = temp;
		if(x == 0)	b0 = temp;
		if(x < 0 || x > 3) error = 2;
	}
	getchar();
	if (error == 2) printf("==>Invalid Exponential Value! Must be between 0-3<==\n");
	
	printf("Select operation from '*', '+', '-' --> ");
	scanf(" %c",&op);
	/*Calling the function and calculating*/
	operate_polynomials(&a3,&a2,&a1,&a0,&b3,&b2,&b1,&b0,op);
	
	/*Printing the resulsts according to operation chosen*/
	printf("\nCoefficients = ");
	if (op == '*')
	{
		printf("(6, %.1f), (5, %.1f), (4, %.1f), (3, %.1f), (2, %.1f), (1, %.1f), (0, %.1f) \n\n", a3, a2, a1, a0, b3, b2, b1);
	}
	
	else if (op == '+' || op == '-')
	{
		printf("(3, %.1f), (2, %.1f), (1, %.1f), (0, %.1f) \n\n",a0, b3, b2, b1);
	}
	
	else
		printf("(6, %.1f), (5, %.1f), (4, %.1f), (3, %.1f), (2, %.1f), (1, %.1f), (0, %.1f) \n\n", a3, a2, a1, a0, b3, b2, b1);
}


void test_four_d_vectors ()
{
	double mean_a0=0.0, mean_a1=0.0, mean_a2=0.0, mean_a3=0.0, longest_distance=0.0;
	int N=5;
	four_d_vectors (&mean_a0, &mean_a1, &mean_a2, &mean_a3, &longest_distance, N);
	printf("Mean a0: %f\nMean a1: %f\nMean a2: %f\nMean a3: %f\nThe longest distance between two points: %f\n\n\n", mean_a0, mean_a1, mean_a2, mean_a3, longest_distance);
}


void test_dhondt_method ()
{
	int partyA=100000, partyB=80000, partyC=30000, partyD=20000, partyE=10000, numberOfSeats=550;
	dhondt_method (&partyA, &partyB, &partyC, &partyD, &partyE, numberOfSeats);
	printf("Party A: %d seat(s).\nParty B: %d seat(s).\nParty C: %d seat(s).\nParty D: %d seat(s).\nParty E: %d seat(s).\n\n\n", partyA, partyB, partyC, partyD, partyE);
}

void test_order_2d_points_cc ()
{
	double x1=97.0, y1=0.7, x2=-5.0, y2=7.0, x3=-7.0, y3=-3.0;
	order_2d_points_cc (&x1, &y1, &x2, &y2, &x3, &y3);
	printf("Counter-Clockwise Order: (%f,%f) - (%f,%f) - (%f,%f)\n\n\n", x1, y1, x2, y2, x3, y3);
}


void test_number_encrypt ()
{
	unsigned char number = 125;
	number_encrypt (&number);
	printf("Encrypted number: %d\n\n\n", number);
}


/*
** main function for testing the functions...
**
*/
int main(void) {
	test_operate_polynomials ();
	test_four_d_vectors ();
	test_dhondt_method ();
	test_order_2d_points_cc ();
	test_number_encrypt ();
	return (0);
} /* end main */
