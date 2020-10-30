/*
 * HAL_Init.c
 *
 * Created: 01/11/2018 22:31:35
 *  Author: Petervg
 */ 

#include <avr/io.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include "../global.h"

#include "../HAL_AVR/SPI.h"
#include "../HAL_AVR/I2C.h"
#include "../HAL_AVR/UART.h"
#include "../HAL_AVR/TC1.h"
#include "../HAL_LCD/SSD1306.h"
#include "HAL_Init.h"		// Hardware init
#include "HAL_GPIO.h"		// Native GPIO


void HAL_Init()
{
	init_GPIO();									// Make sure to init GPIO before SPI to ensure master mode canbe enabled.
	init_TC1(20000);								// Start the tick timer (20.000 for 10mS tick timer with a 16Mhz crystal)
	init_uart(103, 0);								// init the UART 
	i2c_init();										// init I2C
	spi_init (MSBFIRST, SPIMASTER, MODE3, 0, 1);	// SSD1306: SDIN is shifted into an 8-bit shift register on every rising edge of SCLK in the order of D7, D6, ... D0
	
	GPIO_OledReset(RESETI2C, 0);					// Reset the I2C OLED
	GPIO_OledReset(RESETI2C, 1);					// Restart the I2C OLED
	SSD1306_InitI2C(SSD1306_ADDRESS1);				// Init the SSD1306 driver
	
	GPIO_OledReset(RESETSPI, 0);					// Reset the SPI OLED
	GPIO_OledReset(RESETSPI, 1);					// Restart the SPI OLED
	SSD1306_InitSPI();								// Init the SSD1306 driver	
}




