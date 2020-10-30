/*
 * UART.c
 *
 * Created: 01/11/2018 19:29:44
 *  Author: Petervg
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include "../global.h"
#include "UART.h"
#include "../buffer.h"



void init_uart(uint16_t l_UBRR, uint8_t l_doublespeed)
{
	if (l_doublespeed)
	{
		UBRR0H = l_UBRR >> 8;
		UBRR0L = l_UBRR;	
		UCSR0A |= (1<<U2X0);								// set double speed bit
		UCSR0B |= (1<<TXEN0) | (1<<RXEN0) | (1<<RXCIE0);	// Enable RX, TX and RX interrupt
		UCSR0C |= (0<<USBS0) | (3<<UCSZ00);					// 8N1
	}
	else
	{
		UBRR0H = l_UBRR >> 8;
		UBRR0L = l_UBRR;	
		UCSR0B |= (1<<TXEN0) | (1<<RXEN0) | (1<<RXCIE0);	// Enable RX, TX and RX interrupt
		UCSR0C |= (0<<USBS0) | (3<<UCSZ00);					// 8N1
	}
}

void uart_putc(uint8_t c)
{
	// THIS IS TEMPORARY CODE. IMPLEMENT PROPER RINGBUFFER
	while(!(UCSR0A & (1<<UDRE0)));	// wait until sending is possible
	UDR0 = c;						// output character saved in c
}

void uart_puts(const uint8_t* s)
{
	while(*s)
	{
		uart_putc(*s);
		s++;
	}
}

void uart_putvar(uint8_t* s)
{
	while(*s)
	{
		uart_putc(*s);
		s++;
	}
}

ISR (USART0_RX_vect)
{
	UART_RxBuffer.Buffer[UART_RxBuffer.WriteIndex++] = UDR0;
	if (UART_RxBuffer.WriteIndex == RXBUFFERSIZE) 
	{
		UART_RxBuffer.WriteIndex = 0;
		UART_RxBuffer.BufferOverflow = 1;
	}
	UART_RxBuffer.IsDirty=1;
}


