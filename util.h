/*
 * util.h
 *
 * Created: 03/11/2018 13:55:56
 *  Author: Petervg
 */ 


#ifndef UTIL_H_
#define UTIL_H_

// Convert a 16-bit integer to an array of bytes. Pointer to array is returned.
unsigned char * UTIL_ConvertINT2CHAR(unsigned int A);

// Convert a byte value to an array of bytes. Pointer to array is returned.
unsigned char * UTIL_ConvertBYTE2CHAR(unsigned char A);

// Convert an array of Ascii DEC encoded bytes to a Char. Value of the char is returned
unsigned char UTIL_ConvertASCII2BYTE(unsigned char *A, unsigned char length);

// Convert an array of Ascii HEX encoded bytes to a Char. Value of the char is returned. case must be 1 when uppercase, 0 when lowercase
unsigned char UTIL_ConvertHEXASCII2BYTE(unsigned char *A, unsigned char l_length, unsigned char l_case);

#endif /* UTIL_H_ */