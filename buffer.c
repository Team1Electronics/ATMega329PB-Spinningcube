/*
 * buffer.c
 *
 * Created: 10/11/2018 13:43:19
 *  Author: Petervg
 */ 
#include <stdint.h>
#include <avr/io.h>

#include "global.h"

#include "HAL_AVR/UART.h"
#include "buffer.h"

struct _RxBuffer UART_RxBuffer;

signed int FindString(const uint8_t *l_String, struct _RxBuffer *l_UART_RxBuffer)
{
	unsigned char l_BufferIndex = l_UART_RxBuffer->ReadIndex;
	unsigned char l_StringIndex = 1;
	unsigned char l_MatchFound = 0;
	
	while (l_BufferIndex <= l_UART_RxBuffer->WriteIndex)
	{
		if (*l_String == l_UART_RxBuffer->Buffer[l_BufferIndex])
		{
			l_MatchFound = 1;			// found a first match. Now loop through the string until you reach the 0 of the string
			l_StringIndex = 1;

			while(*(l_String + l_StringIndex))
			{
				if (*(l_String + l_StringIndex) != l_UART_RxBuffer->Buffer[l_BufferIndex + l_StringIndex])
				{
					l_MatchFound = 0;
				}
				l_StringIndex++;
			}
			
			if (l_MatchFound)
			{
				return l_BufferIndex;
			}
		}		
		l_BufferIndex++;
	}
	return -1;
}


void ClearBuffer(struct _RxBuffer *l_UART_RxBuffer)
{
	unsigned int l_index=0;
	
	for(l_index = 0; l_index < RXBUFFERSIZE; l_index++)
	{
		l_UART_RxBuffer->Buffer[l_index] = 0;
	}
	
	l_UART_RxBuffer->WriteIndex = 0;
	l_UART_RxBuffer->BufferOverflow = 0;
	l_UART_RxBuffer->IsDirty = 0;
	l_UART_RxBuffer->ReadIndex=0;
}