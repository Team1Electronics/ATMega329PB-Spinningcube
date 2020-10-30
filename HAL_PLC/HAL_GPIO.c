/*
 * GPIO.c
 *
 * Created: 01/11/2018 22:34:09
 *  Author: Petervg
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>

#include "../global.h"
#include "HAL_GPIO.h"
#include "../HAL_LCD/SSD1306.h"
#include "../HAL_AVR/UART.h"


void init_GPIO()
{
	DDRC = 0b00001000;			// Set PC3 as output.
	DDRB = 0b00000111;			// PB0-2 as output
}


void GPIO_OledReset(uint8_t l_Interface, uint8_t l_State)
{
	switch (l_State)
	{
		case 0:
			if (l_Interface == RESETI2C)
			{
				PORTC = PORTC &~(1 << 3);	// I2C display	
			}
			if (l_Interface == RESETSPI)
			{
				PORTB = PORTB &~(1 << 0);	// SPI display	
			}			
		break;

		case 1:
			if (l_Interface == RESETI2C)
			{
				PORTC = PORTC | (1 << 3);	// I2C display
			}
			if (l_Interface == RESETSPI)
			{
				PORTB = PORTB | (1 << 0);	// SPI display
			}			
		break;

		default:
		while(1);	// Error
	}
}


void GPIO_SetOledDC(uint8_t l_State)
{
	switch (l_State)
	{
		case 0:
		PORTB = PORTB &~(1 << 1);
		break;

		case 1:
		PORTB = PORTB | (1 << 1);
		break;

		default:
		while(1);	// Error
	}
}


void GPIO_SetOledCS(uint8_t l_State)
{
	switch (l_State)
	{
		case 0:
		PORTB = PORTB &~(1 << 2);
		break;

		case 1:
		PORTB = PORTB | (1 << 2);
		break;

		default:
		while(1);	// Error
	}
}
