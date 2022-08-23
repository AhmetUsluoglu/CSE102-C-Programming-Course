/*
** hw5_lib.c:
**
** The source file implementing library functions.
**
** Author: Yakup Genc. (c) 2018-2021
**
** Revision: 2021.04.02.23.55
** 
*/

#include <stdio.h>
#include <math.h>
#include "hw5_lib.h"
/* Helper Functions */
int find_biggest(int pA, int pB, int pC, int pD, int pE);

void sort_3_numbers(double *high, double *middle, double *low, double num1, double num2, double num3);
void find_location (double x, double y, double *p, double *val);
void give_points(double high, double middle, double low, double num1, double num2, double num3, double *val1, double *val2, double *val3, double a, double b, double c);

void operate_polynomials  (double* a3, double* a2, double* a1, double* a0, double* b3, double* b2, double* b1, double* b0, char op)
{
	/*Variables for every possible exponential element*/
	double x6 = 0.0, x5 = 0.0, x4 = 0.0, x3 = 0.0, x2 = 0.0, x1 = 0.0, x0 = 0.0;
	
	if(op == '*')
	{
		/*Calculating the values of coeffients*/
		x6 = *a3 * *b3;
		x5 = *a3 * *b2 + *a2 * *b3;
		x4 = *a3 * *b1 + *a2 * *b2 + *a1 * *b3;
		x3 = *a3 * *b0 + *a2 * *b1 + *a1 * *b2 + *a0 * *b3;
		x2 = *a2 * *b0 + *a1 * *b1 + *a0 * *b2;
		x1 = *a1 * *b0 + *a0 * *b1;
		x0 = *a0 * *b0;
	}
	
	else if (op == '+')
	{
		/*Calculating the values of coeffients*/
		x3 = *a3 + *b3;
		x2 = *a2 + *b2;
		x1 = *a1 + *b1;
		x0 = *a0 + *b0;
	}
	
	else if (op == '-')
	{
		/*Calculating the values of coeffients*/
		x3 = *a3 - *b3;
		x2 = *a2 - *b2;
		x1 = *a1 - *b1;
		x0 = *a0 - *b0;
	}
	else
		printf("\nInvalid Opertaion!\n");
		
	/*Assigning coeffiecent values to pointers so we can print them in main*/
	*a3 = x6;
	*a2 = x5;
	*a1 = x4;
	*a0 = x3;
	*b3 = x2;
	*b2 = x1;
	*b1 = x0;
}

void four_d_vectors (double* mean_a0, double* mean_a1, double* mean_a2, double* mean_a3, double* longest_distance, int N)
{
	double d0, d1, d2, d3, euclidian_distance, counter = 0.0, i, temp0 = 0.0, temp1 = 0.0, temp2 = 0.0, temp3 = 0.0, max_distance = 0.0;
	
	/*Until the N element*/
	for(i = 1; i <= N; i++)
	{
		printf("\nEnter Coordinates for %.0f. 4D Vector = ",i);
		scanf("%lf %lf %lf %lf", &d0, &d1, &d2, &d3);
		/*If before Nth elemt user inputs -1 -1 -1 -1 , terminate program (-1 -1 -1 -1 IS INCLUDED TO AVERAGE, IT WAS VAGUE AND I DECIDED TO ADD IT TO AVERAGE SINCE N ALREADY INDICATES THE END.)*/
		if(d0 == -1.0 && d1 == -1.0 && d2 == -1.0 && d3 == -1.0) 
		{
			counter++;
			*mean_a0 += d0, *mean_a1 += d1, *mean_a2 += d2, *mean_a3 += d3;
			*mean_a0 /= counter, *mean_a1 /= counter, *mean_a2 /= counter, *mean_a3 /= counter, *longest_distance = max_distance;
			return;
		}
		*mean_a0 += d0, *mean_a1 += d1, *mean_a2 += d2, *mean_a3 += d3;
		
		/*After the first entry , program takes second inputs , gets differences between first and second inputs, send them to calaculate distance, checking the max distance.
		At the end program increases the order by 1 via assigning the last values to 'temps' then getting new values in next loop*/
		if (i > 1)
		{	/*Getting the differneces and sending it to calculate distance*/
			temp0 -= d0, temp1 -= d1, temp2 -= d2, temp3 -= d3;
			distance_between_4d_points (temp0, temp1, temp2, temp3, &euclidian_distance);
			printf("\nDistance between 2 consecutive vectors = %.2f \n\n", euclidian_distance);
			
			if(euclidian_distance > max_distance)
				max_distance = euclidian_distance;
		}
		/*Assigning current values to leave a space for the next input. Current input will be used to calculate distance in next loop*/
		temp0 = d0, temp1 = d1, temp2 = d2, temp3 = d3;
		counter++;
	}
	/*In every loop we summed the means and increased counter. Now avreages are send to main.*/
	*mean_a0 /= counter, *mean_a1 /= counter, *mean_a2 /= counter, *mean_a3 /= counter, *longest_distance = max_distance;
}

void distance_between_4d_points (double d0, double d1, double d2, double d3, double* euclidian_distance)
{	
	/*Using pythagorian theorem to calculate the distance*/
    *euclidian_distance = sqrt(pow(d0,2) + pow(d1,2) + pow(d2,2) + pow(d3,2));
}

void dhondt_method (int* partyA, int* partyB, int* partyC, int* partyD, int* partyE, int numberOfSeats)
{	
	/*Keeping counts, seats and initial votes separately*/
	int countA = 1, countB = 1, countC = 1, countD = 1, countE = 1;
	int seatA = 0, seatB = 0, seatC = 0, seatD = 0, seatE = 0, total_seats = 0;
	int voteA = *partyA, voteB = *partyB, voteC = *partyC, voteD = *partyD, voteE = *partyE;
	
	/*Until all seats are taken*/
	while(total_seats != numberOfSeats)
	{	
		/*Finding the current biggest vote among participants and returning a value related to them*/
		/*In every case program increases seat by 1, calculates new number of votes, and increases divider for next turn*/
		switch(find_biggest(voteA, voteB, voteC, voteD, voteE))
		{
			case 1:
				seatA++;
				voteA = *partyA / countA++;
				break;
			
			case 2:
				seatB++;
				voteB = *partyB / countB++;
				break;
			
			case 3:
				seatC++;
				voteC = *partyC / countC++;
				break;
			
			case 4:
				seatD++;
				voteD = *partyD / countD++;
				break;
			
			case 5:
				seatE++;
				voteE = *partyE / countE++;
				break;
		}
		total_seats = seatA + seatB + seatC + seatD + seatE;
	}
	/*Assigning seats to popinters to print them in main*/
	*partyA = seatA;
	*partyB = seatB;
	*partyC = seatC;
	*partyD = seatD;
	*partyE = seatE;
}
/*Function finds the party which has the highest current vote count*/
int find_biggest(int pA, int pB, int pC, int pD, int pE)
{
	if(pA >= pB && pA >= pC && pA >= pD && pA >= pE)	return 1;
	if(pB >= pA && pB >= pC && pB >= pD && pB >= pE)	return 2;
	if(pC >= pA && pC >= pB && pC >= pD && pC >= pE)	return 3;
	if(pD >= pA && pD >= pB && pD >= pC && pD >= pE)	return 4;
	if(pE >= pB && pE >= pC && pE >= pD && pE >= pA)	return 5;
}

/*This function gets 3 (x,y) in coordinates and sort them counter-clockwise*/

/*Program finds the location of the (x.y) and gives them points (most important variable-40 to 10 points),
calculates the tangent value which is also the slope then gives points (important variable- 3 to 1 points),
finds the distance to the origin and gives them points (less important variable- 0.3 to 0.1 points)*/
void order_2d_points_cc (double* x1, double* y1, double* x2, double* y2, double* x3, double* y3)
{
	double x_1 = *x1, y_1 = *y1, x_2 = *x2, y_2 = *y2, x_3 = *x3, y_3 = *y3;
	double tan1 = *x1 / *y1, tan2 = *x2 / *y2, tan3 = *x3 / *y3, dis1 = *x1 * *x1 + *y1 * *y1 , dis2 = *x2 * *x2 + *y2 * *y2, dis3 = *x3 * *x3 + *y3 * *y3;
	double p1 = 0, p2 = 0, p3 = 0, val1 = 0, val2 = 0, val3 = 0, low, middle, high; 
	
	/*Program finds the location of the (x.y) and gives them points (most important variable-40 to 10 points),*/
	find_location (x_1, y_1, &p1, &val1);
	find_location (x_2, y_2, &p2, &val2);
	find_location (x_3, y_3, &p3, &val3);

	/*Program calculates the tangent value which is also the slope, then gives points (important variable- 3 to 1 points),*/
	sort_3_numbers(&high, &middle, &low, tan1, tan2, tan3);
	give_points(high, middle, low, tan1, tan2, tan3, &val1, &val2, &val3, 1.0, 2.0 ,3.0);
	
	/*finds the distance to the origin and gives them points (less important variable- 0.3 to 0.1 points)*/
	sort_3_numbers(&high, &middle, &low, dis1, dis2, dis3);
	give_points(high, middle, low, dis1, dis2, dis3, &val1, &val2, &val3, 0.1, 0.2, 0.3);

	/*Program sorts scores given to the (x,y) points, checking the order and assigning them to initail pointers to print them in main*/
	sort_3_numbers(&high, &middle, &low, val1, val2, val3);

	if (high == val1) *x1 = x_1, *y1 = y_1;
	if (high == val2) *x1 = x_2, *y1 = y_2;
	if (high == val3) *x1 = x_3, *y1 = y_3;
	
	if (middle == val1) *x2 = x_1, *y2 = y_1;
	if (middle == val2) *x2 = x_2, *y2 = y_2;
	if (middle == val3) *x2 = x_3, *y2 = y_3;
	
	if (low == val1) *x3 = x_1, *y3 = y_1;
	if (low == val2) *x3 = x_2, *y3 = y_2;
	if (low == val3) *x3 = x_3, *y3 = y_3;	
}
/*Finds the location of the point and giving them points to indicate the importance.*/
void find_location (double x, double y, double *p, double *val)
{
	if (x > 0 && y > 0) 	*p = 1, *val += 40.0;
	if (x < 0 && y > 0) 	*p = 2, *val += 30.0;
	if (x < 0 && y < 0) 	*p = 3, *val += 20.0;
	if (x > 0 && y < 0) 	*p = 4, *val += 10.0;
	
	if (x > 0 && y == 0) 	*p = 1, *val += 40.0;
	if (x < 0 && y == 0) 	*p = 2, *val += 30.0;
	if (x == 0 && y > 0) 	*p = 1, *val += 40.0;
	if (x == 0 && y < 0) 	*p = 4, *val += 10.0;
}
/*Gets high, low, middle values and 3 numbers to sort, compare them to each other and sort the inputs, Returns high mid low*/
void sort_3_numbers(double *high, double *middle, double *low, double num1, double num2, double num3)
{
	if (num1>num2) 
	{   
	  *middle=num1;
	  *low=num2;   
	} 
	else 
	{
	  *middle=num2;  
	  *low=num1;  
	}
	 
	if (*middle>num3) 
	{ 
		*high=*middle;    

		if(*low>num3)
		{         
			*middle=*low;                
			*low=num3;
		}
		else *middle=num3;      
	}
	else *high=num3; 
}
/*Gets high, low, middle values, compare them to each other, gives them to points determined in function cal according to the highest order , assigning the points to pointers*/
void give_points(double high, double middle, double low, double num1, double num2, double num3, double *val1, double *val2, double *val3, double a, double b, double c)
{
	if (high == num1) *val1 += a;
	if (high == num2) *val2 += a;
	if (high == num3) *val3 += a;
	
	if (middle == num1) *val1 += b;
	if (middle == num2) *val2 += b;
	if (middle == num3) *val3 += b;
	
	if (low == num1) *val1 += c;
	if (low == num2) *val2 += c;
	if (low == num3) *val3 += c;
}

void number_encrypt (unsigned char* number)
{
	char b7='0', b6='0', b5='0', b4='0', b3='0', b2='0', b1='0', b0='0';
	get_number_components (*number, &b7, &b6, &b5, &b4, &b3, &b2, &b1, &b0);
	reconstruct_components (number, b7, b6, b5, b4, b3, b2, b1, b0);
}
/*Diving the first value by 2 until the end while assigning every remainer to the variables which meant to hold the binary numbers (at most 8 digits in binary - 255 in decimal)*/
void get_number_components (unsigned char number, char* b7, char* b6, char* b5, char* b4, char* b3, char* b2, char* b1, char* b0)
{
	 *b0 = number %2;
	 number /= 2;
	 *b1 = number %2;
	 number /= 2;
	 *b2 = number %2;
	 number /= 2;
	 *b3 = number %2;
	 number /= 2;
	 *b4 = number %2;
	 number /= 2;
	 *b5 = number %2;
	 number /= 2;
	 *b6 = number %2;
	 number /= 2;
	 *b7 = number %2;
	 number /= 2;
}
/*Relocating the binary values according to instructions and calculating the decimal value by multplying them with orders of 2 according to the digit number.(0. digit 2^0, 5. digit 2^5)*/
void reconstruct_components (unsigned char* number, char b7, char b6, char b5, char b4, char b3, char b2, char b1, char b0)
{
	char temp;
	int i;
	
	temp = b7;
	b7 = b2;
	b2 = temp;
	
	temp = b6;
	b6 = b3;
	b3 = temp; 
	
	temp = b5;
	b5 = b0;
	b0 = temp; 
	
	temp = b4;
	b4 = b1;
	b1 = temp;

	for(i = 0; i < 2; i++)
	{
		temp = b7;
		b7 = b6;
		b6 = b5;
		b5 = b4;
		b4 = b3;
		b3 = b2;
		b2 = b1;
		b1 = b0;
		b0 = temp;
	} /*Reassigning the calculated value to pointer to print it in main*/ 
	*number = b7*pow(2,7) + b6*pow(2,6) + b5*pow(2,5) + b4*pow(2,4) + b3*pow(2,3) + b2*pow(2,2) + b1*pow(2,1) + b0*pow(2,0);
}

