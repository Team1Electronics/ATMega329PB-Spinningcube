/*
 * FrameWork.c
 *
 * Created: 01/11/2018 19:02:40
 * Author : Petervg
 * 
 * I2C connections:
 * Connect OLED D0 (SCL) to the CPU PC5
 * Connect OLED D1 (SDA) to the CPU PC4
 * Connect OLED RES (reset) to the CPU PC3
 * Connect OLED DC (data/command) to GND
 * Connect OLED CS (Chip select) to GND
 *
 * SPI connections:
 * Connect OLED D0 (CLK) to the CPU PB5
 * Connect OLED D1 (MOSI) to the CPU PCB3
 * Connect OLED RES (reset) to the CPU PB0
 * Connect OLED DC (data/command) to the CPU PB1
 * Connect OLED CS (Chip select) to the CPU PB2
 * 
 * 
 */ 

#include <stdint.h>
#include <avr/io.h>

#include "global.h"

#include "main.h"
#include "util.h"
#include "buffer.h"
#include "cube.h"

#include "HAL_PLC/HAL_Init.h"
#include "HAL_PLC/HAL_GPIO.h"
#include "HAL_AVR/UART.h"
#include "HAL_LCD/SSD1306.h"


int main(void)
{
	uint16_t l_rotateX;
	uint16_t l_rotateY;
	uint16_t l_rotateZ;
	uint8_t l_CubeSize = 20;
	
	ResizeCube(l_CubeSize);
	
	l_rotateX = 180;
	l_rotateY = 180;
	l_rotateZ = 180;	
	
	HAL_Init();
	
	SSD1306_clear(&SSD1306FrameBuffer1);
	SSD1306_puts(0, 0, (char*)"Thomas more", FONTLARGE, &SSD1306FrameBuffer1);
	SSD1306_puts(0, 2, (char*)"Spinning cube demo", FONTMEDIUM, &SSD1306FrameBuffer1);
	ProcessLCD(OLEDI2C);
		
	// not using the tick timer to get display updates as fast as possible
    while (1)
    {
		if (l_rotateX++ > 359) l_rotateX = 0;
		if (l_rotateY++ > 359) l_rotateY = 0;
		if (l_rotateZ++ > 359) l_rotateZ = 0;
		
		SSD1306_clear(&SSD1306FrameBuffer1);
		DrawCubeRotated(&SSD1306FrameBuffer1, SSD1306_WIDTH/2, SSD1306_HEIGHT/2, l_rotateX, l_rotateY, l_rotateZ);
		ProcessLCD(OLEDSPI);	
    }
}







