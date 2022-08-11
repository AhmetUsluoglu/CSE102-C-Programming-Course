/*
** main.c:
**
** The test/driver program for the homework.
**
** Author: Yakup Genc. (c) 2018-2021
**
** Revision: 2021.03.03.20.16
** 
*/


#include <stdio.h>
#include "hw1_lib.h"
#include "hw1_io.h"



/*
** Testing root finding..
**
*/
void test_root_finder3() 
{
	double root_known = -0.6;
	double root_found;
	root_found = root3(1.0, 2.0, 3.0, 4.0, root_known - 1.0, root_known + 1.0);
	printf("Ground truth Root 3: %f vs found root: %f\n", root_known, root_found);
}

void test_root_finder4() 
{
	double root_known = -0.9;
	double root_found;
	root_found = root4(-1.0, 2.0, 3.0, 4.0, 5.0, root_known - 1.0, root_known + 1.0);
	printf("Ground truth Root 4: %f vs found root: %f\n", root_known, root_found);
}

void test_integral3() 
{
	double root_found;
	root_found = integral3(1.0, 2.0, 3.0, 4.0, -1.6, 0.4, 0.1);
	printf("Integral 3 is %f\n", root_found);
}

void test_integral4() 
{
	double root_found;
	root_found = integral4(1.0, 2.0, 3.0, 4.0, 5.0, -1.0, +1.0, 0.1);
	printf("Integral 4 is %f\n", root_found);
}
/*
** main function for testing the functions...
**
*/
int main(void) {
	write_polynomial3(1.0,2.0,-7.0,-3.0);
	write_polynomial4(1.0,0.0,-5.0,0.0,1.2);
	test_root_finder3();
	test_root_finder4();
	test_integral3();
	test_integral4();
	return (0);
} /* end main */
