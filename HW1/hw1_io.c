/*
** hw1_io.c:
**
** The source file implementing output functions.
**
** Author: Yakup Genc. (c) 2018-2021
**
** Revision: 2021.03.03.20.16
** 
*/


#include <stdio.h>
#include "hw1_io.h"

void write_polynomial3(double a0, double a1, double a2, double a3)
{
	/* Assigning every value in !d array for easier use, then staring a for loop to check all elements in !D array*/
	double a[4] = {a0, a1, a2, a3};
    for ( int i = 3; i >= 0; i--)
    {
    /* There are two general cases which is if the values bigger or lower than '0';*/
    	if(a[i] > 0) 
    	{   
    	/* In every case initial x value is important so we shape the function to fit all cases respectively but mainly for initial x^3 value. */
    		if (a[i] == 1)
    		{
    			if (i == 3) printf("x^3 ");
    			else if (i == 2) printf("+x^2 ");
    			else if (i == 1) printf("+x ");
    			else if (i == 0) printf("+1 ");
    		}
    		else if (a[i] != 1)
    		{
    			if (i == 3) 	 printf("%.1lfx^3 ", a[i]);
    			else if (i == 2) printf("+%.1lfx^2 ",a[i]);
    			else if (i == 1) printf("+%.1lfx ",a[i]);
    			else if (i == 0) printf("+%.1lf ", a[i]);
    		}

    	}
    	else if(a[i] < 0)
    	{   
    	/* Same as the case above.*/
    		if (a[i] == -1)
    		{
    			if (i == 3) 	 printf("-x^3 ");
    			else if (i == 2) printf("-x^2 ");
    			else if (i == 1) printf("-x ");
    			else if (i == 0) printf("-1 ");
    		}
    		else if (a[i] != -1)
    		{
    			if (i == 3) 	 printf("%.1lfx^3 ",a[i]);
    			else if (i == 2) printf("%.1lfx^2 ",a[i]);
    			else if (i == 1) printf("%.1lfx ",a[i]);
    			else if (i == 0) printf("%.1lf ", a[i]);
    		}
    	}
    }
    printf("\n");
}

void write_polynomial4(double a0, double a1, double a2, double a3, double a4)
{
	/* In this function all we do is simply write special cases for initial value and calling the prevous function to finish the process*/
	if (a4 > 0)
	{
		if (a4 == 1) printf("x^4 ");
		else printf("%.1lfx^4 ", a4);
	}
	else if (a4 < 0)
	{
		if (a4 == -1) printf("-x^4 ");
		else printf("%.1lfx^4 ", a4);
	}
	
	if (a3 > 0) printf("+ ");
	write_polynomial3 (a0, a1, a2, a3);
}















