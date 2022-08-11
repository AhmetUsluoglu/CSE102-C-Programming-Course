/*
** hw4_lib.c:
**
** The source file implementing library functions.
**
** Author: Yakup Genc. (c) 2018-2021
**
** Revision: 2021.03.03.20.16
** 
*/

#include <stdio.h>
#include "hw1_lib.h"


double integral3(double a0, double a1, double a2, double a3, double xs, double xe, double delta)
{
/* Using Rinmann's Calculation method to find aproximate total area (integral).'*/
	double f, result = 0.0;
	/* Staring from xs + delta/2 as first input and increases by delta for each step*/
    for(double i = xs + delta/2 ; i < xe; i = i + delta)
    {
    	f = a3*i*i*i+a2*i*i+a1*i+a0;
    	result = result + f * delta;
    }
    
    return result;
}


double integral4(double a0, double a1, double a2, double a3, double a4, double xs, double xe, double delta)
{
/* Same as the function above.*/
    double f, result = 0.0;
    for(double i = xs + delta/2 ; i < xe; i = i + delta)
    {
    	f = a4*i*i*i*i + a3*i*i*i + a2*i*i + a1*i + a0;
    	result = result + f * delta;
    }
    
    return result;
}


double root3(double a0, double a1, double a2, double a3, double xs, double xe)
{
	/* Using Newton's Root Calculation method to find aproximate value of roots. Starting initial guess from middle of the range*/
    double xn = (xs + xe) / 2;
    
    for (int i = 1; i <= 10; i++)
    {
        xn = xn - (a3*xn*xn*xn + a2*xn*xn + a1*xn + a0) / (3*a3*xn*xn + 2*a2*xn + a1);
    }
    return xn;
}


double root4(double a0, double a1, double a2, double a3, double a4, double xs, double xe)
{
	/* Same as the function above.*/
    double xn = (xs + xe) / 2;
    for (int i = 1; i <= 10; i++)
    {
        xn = xn - (a4*xn*xn*xn*xn + a3*xn*xn*xn + a2*xn*xn + a1*xn + a0) / (4*a4*xn*xn*xn + 3*a3*xn*xn + 2*a2*xn + a1);
    }
    return xn;
}
