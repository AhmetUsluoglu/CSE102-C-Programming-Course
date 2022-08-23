#include <stdio.h>

void calculate_fibonacci_sequence();
void decide_perfect_harmonic_number();
void difference_max_min ();
void bmi_calculation ();

int main (void)
{	/*Calling Functions*/
	calculate_fibonacci_sequence();
	decide_perfect_harmonic_number();
	difference_max_min ();
	bmi_calculation ();
}

void calculate_fibonacci_sequence()
{	/*Assigning initial values for numbers to calculate fibonacci series*/
	int input,i, num1 = 0, num2 = 1, num3 = 1;
	char check = '\n';
	/*Check is a character which takes \n as input when numbers entered by user. If the user enters * then program terminates*/
	while(!(check == '*'))
	{	/*Getting a char with number at the same time to find id user entered * as input*/
		printf("Please enter term(s) number:");
		scanf("%d",&input);
		check = getchar();
		
		/*Conditions that we do not wnat  user to enter as input.*/
		while((input <= 0) || (check != '\n' && check != '*'))
		{
			if (input <= 0 && check == '\n')
				printf("Please enter 'positive' term(s) number:");
			
			else if (check != '\n' && check != '*')
				printf("Please enter 'numeric' term(s) number:");
			/*Getting new input in loop to able to leave infinite loop*/
			input = 0, check = '\n';
			scanf("%d",&input);
			check = getchar();
		}
		/*In case the user inputs * as first, we do not want program to run anything else, just terminate the program*/
		if (!(check == '*'))
		{
			printf("Fibonacci Sequence:\n");
			/*Calculating fibonnacci series via adding orevious numbers to current numbers and moving new current numbers to previous numbers*/
			for(i = 0; i < input; i++)
		    {    
				printf("%d\n", num3);  
				num3 = num1 + num2;
				num1 = num2;    
				num2 = num3;    
		    }
		    /*Resetting all numbers to initial values because program still active and we assigned them out of the loop before*/
		    num1 = 0, num2 = 1, num3 = 0;  
		}
	}
}

void decide_perfect_harmonic_number()
{	/*Assigning values for calculations. */
	int input = 1, i, divisors = 1, total_div = 0;
	char check;
	double harmonic = 0.0, mean = 0.0;
	/*Check is a character which takes \n as input when numbers entered by user. If the user enters * then program terminates*/
	while(!(check == '*'))
	{	/*Getting a char with number at the same time to find id user entered * as input*/
		printf("\nPlease enter input number:");
		scanf("%d",&input);
		check = getchar();
		/*Conditions that we do not wnat  user to enter as input.*/
		while(input <= 0)
		{
			printf("Please enter 'positive' input number:");
			/*Getting new input in loop and checking the terminator character*/
			scanf("%d",&input);
			check = getchar();
			if (check == '*') return;
		}
		/*In case the user inputs * as first, we do not want program to run anything else, just terminate the program*/
		if (!(check == '*'))
		{
			printf("\nNatural Number Divisors:");
			/*To calculate natural divisors we use module for every integer until input/2 because input can not be divided by a number lower than 2 except 1 as integer*/
			for (i = 1; i <= input/2; i++)
			{
				if (input % i == 0)
				{	/*finding the sum of divisors, number of divisors and the harmonic which is 'bolunen' part of the 'bolen' part in harmonic divisor formula.
					Harmonic divisor formula is number of divisors divided by all the divisors result from dividing 1.*/				
					printf("%d,",i);
					total_div += i;
					harmonic += input/i;
					divisors++;
				}
			}
			/*Last divisor is itself*/
			printf("%d\n",input);
			harmonic++;
			/*Harmonic Divisor Formula*/
			mean = divisors/(harmonic/input);
			
			/*If the Total of all the divisors of number except itself equal to itself then it is a Perfect number*/
			printf("\nIs Perfect Number:");
			if (total_div == input) printf("Yes\n");
			else printf("No\n");
			
			/*Finding if the result of Harmonic Divisor formula is integer by casting and substracting the result from itself.*/
			printf("\nIs Harmonic Divisor Number:");
			if (mean - (int)mean == 0) printf("Yes\n\n");
			else printf("No\n\n");
		}
		/*Resetting all numbers to initial values because program still active and we assigned them out of the loop before*/
		divisors = 1, total_div = 0, harmonic = 0.0, mean = 0.0;
	}
}

void difference_max_min ()
{
	float num1, num2, num3, num4, num5, min, max;
	
	printf("\nPlease Enter 5 Numbers\n");
	scanf("%f%f%f%f%f", &num1,&num2,&num3,&num4,&num5);
	
	/*Finding max values by checking every combination*/
	if(num1 >= num2 && num1 >= num3 && num1 >= num4 && num1 >= num5) max = num1;
	if(num2 >= num1 && num2 >= num3 && num2 >= num4 && num2 >= num5) max = num2;
	if(num3 >= num1 && num3 >= num2 && num3 >= num4 && num3 >= num5) max = num3;
	if(num4 >= num1 && num4 >= num2 && num4 >= num3 && num4 >= num5) max = num4;
	if(num5 >= num1 && num5 >= num2 && num5 >= num3 && num5 >= num4) max = num5;
	
	/*Finding min values by checking every combination*/
	if(num1 <= num2 && num1 <= num3 && num1 <= num4 && num1 <= num5) min = num1;
	if(num2 <= num1 && num2 <= num3 && num2 <= num4 && num2 <= num5) min = num2;
	if(num3 <= num1 && num3 <= num2 && num3 <= num4 && num3 <= num5) min = num3;
	if(num4 <= num1 && num4 <= num2 && num4 <= num3 && num4 <= num5) min = num4;
	if(num5 <= num1 && num5 <= num2 && num5 <= num3 && num5 <= num4) min = num5;
	
	/*Calculatin and printing the results*/
	printf("\nMaximum number is %.2f\n",max);
	printf("Minumum number is %.2f\n",min);		
	printf("Difference between maximum and minimum %.2f\n", max-min);
}

void bmi_calculation ()
{
	float weight, height, bmi;
	
	printf ("\n\nPlease Enter Your Weight (kg)\n");
	scanf ("%f",&weight);
	printf ("Please Enter Your Height (m)\n");
	scanf ("%f",&height);
	/*Formula of Body Mass Index*/
	bmi = weight/(height*height);
	printf ("Your Category:");
	
	/*Pre determined cases for BMI results*/
	if (bmi < 16.0) 					printf ("Severly Underweight\n");
	    
	else if (bmi >= 16.0 && bmi < 18.5) printf ("Underweight\n");
	    
	else if (bmi >= 18.5 && bmi < 25.0) printf ("Normal\n");
	    
	else if (bmi >= 25.0 && bmi < 30.0) printf ("Overweight\n");
	    
	else if (bmi >= 30.0) 				printf ("Obese\n");
}

