/*
 * util.c
 *
 * Created: 03/11/2018 13:55:42
 *  Author: Petervg
 */ 

#include <stdint.h>
#include <avr/io.h>

#include "global.h"
#include "util.h"

unsigned char * UTIL_ConvertINT2CHAR(unsigned int A)
{
	unsigned char digit = 0;
	unsigned char charnr = 0;
	static unsigned char pstring[6] = {0,0,0,0,0,0};

	for (charnr = 4; charnr < 255 ; charnr--)
	{
		digit = A - (A/10)*10;			// Filter out digit
		pstring[charnr] = digit+48;		// Convert digit to an ascii character
		A = A/(10);						// Next decimal
	}
	
	return pstring;
}


unsigned char * UTIL_ConvertBYTE2CHAR(unsigned char A)
{
	unsigned char digit = 0;
	unsigned char charnr = 0;
	static unsigned char pstring[4] = {0,0,0,0};

	for (charnr = 2; charnr < 255 ; charnr--)
	{
		digit = A - (A/10)*10;			// Filter out digit
		pstring[charnr] = digit+48;		// Convert digit to an ascii character
		A = A/(10);						// Next decimal
	}
	
	return pstring;
}


unsigned char UTIL_ConvertASCII2BYTE(unsigned char *A, unsigned char l_length)
{
	unsigned char l_Value = 0;
	unsigned char l_digits[] =	{0,0,0};
	unsigned char l_digit = 0;
	
	while (l_length)
	{
		l_digits[l_digit++] = (*(A + l_length-1)-48);
		l_Value = l_digits[0] + l_digits[1] * 10 + l_digits[2] * 100;
		l_length--;
	}
	
	return l_Value;
}

unsigned char UTIL_ConvertHEXASCII2BYTE(unsigned char *A, unsigned char l_length, unsigned char l_case)
{
	unsigned char l_Value = 0;
	unsigned char l_digits[] =	{0,0};
	unsigned char l_digit = 0;
	
	while (l_length)
	{
		l_digits[l_digit] = (*(A + l_length-1)-48);			// Subtract 48 to get nr value. Is wrong when digit is a, b, c, d, e or f
		if (l_digits[l_digit] > 9) 
		{
			if (l_case == LOWERCASE)
			{
				l_digits[l_digit] = l_digits[l_digit]-39;		// If the value is larger than 9, we wrongfully assumed 0..9. So subtract 39 more to get nr value.	
			}
			if (l_case == UPPERCASE)
			{
				l_digits[l_digit] = l_digits[l_digit]-17;		// If the value is larger than 9, we wrongfully assumed 0..9. So subtract 17 more to get nr value.	
			}
			
		}
		l_Value = l_digits[0] + l_digits[1] * 16;
		l_digit++;
		l_length--;
	}
	
	return l_Value;
}
