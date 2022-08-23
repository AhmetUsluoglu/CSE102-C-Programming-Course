/*
** hw2_lib.c:
**
** The source file implementing library functions.
**
*/

#include <stdio.h>
#include "hw2_lib.h"
#include <math.h>
#include <stdlib.h>

int find_total_days(int day, int month, int year)
{	
	/* starting from date 01/01/0001 saturday*/
	int total_days = 0, rmn_days = 0, i = 0;
	/*Recording every month as number in an array*/
	int months[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};


	/*Before Starting We check the date is exist or not and  if it does not exist we return -1.*/
	if ((month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) && day > 31) return -1;
	if ((month == 2 || month == 4 || month == 6 || month == 9 || month == 11 ) && day > 30 || month > 12) return -1;			
  	if ((month == 2 && day == 29) && ((year/4 - year/100 + year/400) == ((year-1)/4 - (year-1)/100 + (year-1)/400))) return -1;
  	if (year <= 0 || month <= 0 || day <= 0) return -1;
  	
  	/*Calculating leap days for that year and conditioning the 29th february because it's already added in calculation'*/
  	rmn_days = year/4 - year/100 + year/400;
  	if ((month <= 2 && day <= 29) && ((year/4 - year/100 + year/400) != ((year-1)/4 - (year-1)/100 + (year-1)/400))) rmn_days--;
  	/*printf("Artik gun sayisi %d \n",rmn_days);*/

	/*Calculating total days in months to given date.*/
	for (i = 0; i<month-1; i++)
		total_days += months[i];
	/*printf("Ay bazinda toplam gun %d \n",total_days);*/
	
	total_days += (year - 1)*365 + rmn_days + day;
	/*printf("Toplam gun %d \n",total_days);*/
	
	return total_days;
}

int find_weekday_of_data(int day, int month, int year)
{	
	int today = (find_total_days(day, month, year)) % 7;   
	if (today == 0) today = 7;
    return today;
}

int count_day_between_dates(int start_day, int start_month, int start_year, int end_day, int end_month, int end_year)
{	/*Sending each date to total days function and calculating the differnece.*/
    int gap = find_total_days(end_day, end_month, end_year) - find_total_days(start_day, start_month, start_year);
    return gap;
}

double find_angle(double a, double b, double c)
{
	double b2, c2, Vb, Vc, tetha;
	
	/*Using median theorem of 2*Vb*Vb = a*a + c*c - b*b/2 to find medians which is part of the little triangle we look for*/
    Vb = sqrt((a*a + c*c - (b*b)/2)/2); 
    b2 = 2*Vb/3;
    Vc = sqrt((a*a + b*b - (c*c)/2)/2);
    c2 = 2*Vc/3;
    
	/*Using cosinus theorem (a*a = b*b + c*c - 2*b*c*cos(tetha)) in acos function to return directly the angle as radian */
    tetha = acos((b2*b2 + c2*c2 - a*a) / (2*b2*c2));
    return tetha;
}

void print_tabulated(unsigned int r11, double r12, int r13, 
                     unsigned int r21, double r22, int r23, 
                     unsigned int r31, double r32, int r33, char border)
{	/*Chart type selection*/
	printf("Select '*' or '-':");
	scanf(" %c",&border);
	/*Getting rid of digits after .xx decimals for doubles. */
	r12 = (int)(100*r12)/100.00;
	r22 = (int)(100*r22)/100.00;
	r32 = (int)(100*r32)/100.00;
	
	/*Determining a chart type and printing values.*/
	if (border == '*')
	{
		printf("*************************************************************************\n");
		printf("*\t\t\t*\t\t\t*\t\t\t*\n*\tRow 101 \t* \tRow ABCDEFG \t*\tRow XYZ123\t*\n*\t\t\t*\t\t\t*\t\t\t*\n");
		printf("*************************************************************************\n");
		
		printf("*\t\t\t*\t\t\t*\t\t\t*\n*\t  %-5d \t* %-7.7g \t\t* %-+7i \t\t*\n*\t\t\t*\t\t\t*\t\t\t*\n",r11,r12,r13);
		printf("*************************************************************************\n");
		printf("*\t\t\t*\t\t\t*\t\t\t*\n*\t  %-5d \t* %-7.7g \t\t* %-+7i \t\t*\n*\t\t\t*\t\t\t*\t\t\t*\n",r21,r22,r23);
		printf("*************************************************************************\n");
		printf("*\t\t\t*\t\t\t*\t\t\t*\n*\t  %-5d \t* %-7.7g \t\t* %-+7i \t\t*\n*\t\t\t*\t\t\t*\t\t\t*\n",r31,r32,r33);
		printf("*************************************************************************\n");
	}
	/*I did not use corner type lines becasuse it was giving errors*/
	else if (border == '-')
	{
		printf("*-----------------------------------------------------------------------*\n");
		printf("|\t\t\t|\t\t\t|\t\t\t|\n|\tRow 101 \t| \tRow ABCDEFG \t|\tRow XYZ123\t|\n|\t\t\t|\t\t\t|\t\t\t|\n");
		printf("|-----------------------------------------------------------------------|\n");
		
		printf("|\t\t\t|\t\t\t|\t\t\t|\n|\t  %-5d \t| %-7.7g \t\t| %-+7i \t\t|\n|\t\t\t|\t\t\t|\t\t\t|\n",r11,r12,r13);
		printf("|-----------------------------------------------------------------------|\n");
		printf("|\t\t\t|\t\t\t|\t\t\t|\n|\t  %-5d \t| %-7.7g \t\t| %-+7i \t\t|\n|\t\t\t|\t\t\t|\t\t\t|\n",r21,r22,r23);
		printf("|-----------------------------------------------------------------------|\n");
		printf("|\t\t\t|\t\t\t|\t\t\t|\n|\t  %-5d \t| %-7.7g \t\t| %-+7i \t\t|\n|\t\t\t|\t\t\t|\t\t\t|\n",r31,r32,r33);
		printf("*-----------------------------------------------------------------------*\n");
	}
    printf("Invalid Selection!\n");
}

