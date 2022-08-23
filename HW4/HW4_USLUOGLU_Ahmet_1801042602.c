#include <stdio.h>
#include <math.h>

void part1(void);
void pretty_polynomial(int Nth, float coef, int counter);

void part2(void);
void number_check(int num);

void part3(void);
int  sum_of_primes(int num1, int num2);


int main()
{
	part1();
	part2();
	part3();
	return 0;
}

void part1(void)
{
	/*Variables for functions*/
	int Nth , i;
	float coefficient;
	
	/*Getting coefficients for polynomial function and printing the result at the same time*/
	printf("Enter Your Polynomial [N then a_n to a_0] \n");
	scanf("%d",&Nth);
	printf("p(x) = ");
	for (i = 0; i <= Nth; i++)
	{
		scanf("%f",&coefficient);
		pretty_polynomial(Nth, coefficient, i);
	}
}

void part2(void)
{
	/*Variables for function*/
	int num;
	
	do
	{
	/*Getting a number for armstrong and palindrome check*/
	printf("\nPlease enter an integer number:");
	scanf("%d",&num);
	number_check(num);
	}while(num < 0);

}

void part3(void)
{
	/*Variables for function*/
	int frs_int, sec_int;
	do
	{
	/*Getting numbers to find sum of the prime numbers between them*/
	printf("\nPlease enter first integer number : ");
	scanf("%d",&frs_int);
	printf("Please enter second integer number : ");
	scanf("%d",&sec_int);
	printf("Sum of prime numbers between %d and %d : %d \n",frs_int ,sec_int,sum_of_primes(frs_int, sec_int));
	}while (frs_int < 0 || sec_int < 0);
}


void pretty_polynomial(int Nth, float coef, int counter)
{
	/*For every case we also check for 1,-1 values to pretty print the polinom correctly*/
	/*Finding the exponent of the current x value*/
	int power = Nth - counter;
	
	/*Case for biggest exponent*/
	if (power == Nth)
	{
		if(coef == 0)
			printf("Incorrect polynomial degree!\n");
		if (coef != 1 && coef != -1)
			printf("%.1fx^%d ",coef ,power);
		if (coef == 1 && Nth != 1)
			printf("x^%d ",power);
		if (coef == -1 && Nth != 1)
			printf("-x^%d ",power);
		if (coef == 1 && Nth == 1)
			printf("x ",power);
		if (coef == -1 && Nth == 1)
			printf("-x ",power);
	}
	
	if (power > 1 && power != Nth && coef != 0)
	{
		if (coef != 1 && coef != -1)
			printf("%+.1fx^%d ",coef ,power);
		if (coef == 1)
			printf("+x^%d ",power);
		if (coef == -1)
			printf("-x^%d ",power);
	}
	/*Case for x^1 value*/
	if (power == 1 && power != Nth && coef != 0)
	{
		if (coef != 1 && coef != -1)
			printf("%+.1fx ",coef);
		if (coef == 1)
			printf("+x ");
		if (coef == -1)
			printf("-x ");
	}
	/*Case for x^0 constant*/
	if (power == 0 && coef != 0)
		printf("%+.1f \n",coef);
}

void number_check(int num)
{
	int i,total = 0, digit, num_arm = num, num_pal = num, counter = 0, count_digit = 0
	,total_1 =0, total_2 =0, check_pal = 0, check_arm = 0;
	
	/*Printing an error message but still running the code*/
	if(num <= 0)
		printf("Please Enter Positive Integer!\n");

	/*Checking armstrong number in loop for every digit till number is lower than 1*/
	while (num_arm >= 1)
	{	
	/*Taking a digit, getting 3rd power of it and keeping the sum in total, then dividing number and going for next digit*/
		digit = num_arm % 10;
		total += pow(digit,3);
		num_arm /= 10;
	}
	if (num == total)
		check_arm = 1;
	
	/*First finding the number of digits*/
	while (num_pal >=1)
	{
		num_pal /= 10;
		counter++;
	}
	num_pal = num;
	
	/*Checking palindrom number in loop for every digit till number is lower than 1*/
	while (num_pal >= 1)
	{
		/*Keeping sum of the first half of the digits at total1*/
		if (count_digit < counter/2)
		{
			digit = num_pal % 10;
			total_1 += digit;
			num_pal /= 10;
		}
		/*When the number of digit is odd number then skip the middle number*/
		if (counter % 2 != 0 && count_digit == counter/2)
		{
			num_pal /= 10;
		}
		/*Keeping sum of the first half of the digits at total2*/
		if (count_digit > counter/2)
		{
			digit = num_pal % 10;
			total_2 += digit;
			num_pal /= 10;
		}
		count_digit++;
	}

	if (total_1 == total_2)
		check_pal = 1;
	
	if (check_arm == 1 && check_pal == 1)
		printf("This number is both Palindrome and Armstrong number.\n");
	else if(check_arm == 1 && check_pal == 0)
		printf("This number is only Armstrong number.\n");
	else if(check_arm == 0 && check_pal == 1)
		printf("This number is only Palindrome number.\n");	
	else if(check_arm == 0 && check_pal == 0)
		printf("This number does not satisfy any special cases.\n");	
}

int  sum_of_primes(int num1, int num2)
{
	int total = 0, number=0 ,i ,j ,counter = 0,temp = 0;
	
	if(num1 < 0 || num2 < 0)
		printf("Please Enter Positive integers\n");
	
	/*num2 must be bigger*/
	if(num1 > num2)
	{
		temp = num2;
		num2 = num1;
		num1 = temp;
	}
	/*Checking  every number from num1 to num2 and if they are prime keeping the total sum in 'total'*/
	for (j = num1; num1 <= num2; num1++)
	{
		for(i = 1; i <= num1; i++)
		{
			if (num1 % i == 0)
			{
				counter++;
			}
		}
		if(counter == 2)
		{
			total += num1;
		}
		counter = 0;
	}
	return total;
}














































