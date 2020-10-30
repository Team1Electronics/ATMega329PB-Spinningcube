/*
 * SSD1306.c
 *
 * Created: 07/11/2018 17:45:32
 *  Author: Petervg
 */ 

#include <avr/pgmspace.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../global.h"

#include "../HAL_PLC/HAL_GPIO.h"
#include "../HAL_AVR/I2C.h"
#include "../HAL_AVR/SPI.h"

#include "SSD1306.h"
#include "font.h"


// ------------------------------------------------------------------------------------------------------
// Copy framebuffer to display
// ------------------------------------------------------------------------------------------------------
struct _SSD1306FrameBuffer SSD1306FrameBuffer1;

void ProcessLCD(uint8_t l_Displaytype)
{
	if (SSD1306FrameBuffer1.IsDirty)
	{
		switch (l_Displaytype)
		{
			case OLEDSPI:
			SSD1306_UpdateLCDSPI(&SSD1306FrameBuffer1);
			break;
				
			case OLEDI2C:
			SSD1306_UpdateLCDI2C(&SSD1306FrameBuffer1, SSD1306_ADDRESS1);
			break;
			
			case OLEDBOTH:
			SSD1306_UpdateLCDSPI(&SSD1306FrameBuffer1);
			SSD1306_UpdateLCDI2C(&SSD1306FrameBuffer1, SSD1306_ADDRESS1);
			break;
				
			while(1);	// error
		}		
	}
}


// ------------------------------------------------------------------------------------------------------
// I2C low level communications
// ------------------------------------------------------------------------------------------------------

void SSD1306_InitI2C(uint8_t l_I2CAddress)
{	
	SSD1306_sendCommandI2C(SSD1306_DISPLAYOFF, l_I2CAddress);			// 0xAE
	SSD1306_sendCommandI2C(SSD1306_SETDISPLAYCLOCKDIV, l_I2CAddress);	// 0xD5
	SSD1306_sendCommandI2C(0x80, l_I2CAddress);							// 0x80
	SSD1306_sendCommandI2C(SSD1306_SETMULTIPLEX, l_I2CAddress);			// 0xA8
	SSD1306_sendCommandI2C(SSD1306_WIDTH-1, l_I2CAddress);				// 0x3F 
	SSD1306_sendCommandI2C(SSD1306_SETDISPLAYOFFSET, l_I2CAddress);		// 0xD3
	SSD1306_sendCommandI2C(0x00, l_I2CAddress);							// 0x00
	SSD1306_sendCommandI2C(SSD1306_SETSTARTLINE | 0x00, l_I2CAddress);	// 0x40
	SSD1306_sendCommandI2C(SSD1306_CHARGEPUMP, l_I2CAddress);			// 0x8D
	SSD1306_sendCommandI2C(0x14, l_I2CAddress);							// 0x14
	SSD1306_sendCommandI2C(SSD1306_MEMORYMODE, l_I2CAddress);			// 0x20
	SSD1306_sendCommandI2C(0x00, l_I2CAddress);							// 0x00
	SSD1306_sendCommandI2C(SSD1306_SEGREMAP | 0x1, l_I2CAddress);		// 0xA1
	SSD1306_sendCommandI2C(SSD1306_COMSCANDEC, l_I2CAddress);			// 0xC8
	SSD1306_sendCommandI2C(SSD1306_SETCOMPINS, l_I2CAddress);			// 0xDA
	SSD1306_sendCommandI2C(0x12, l_I2CAddress);							// 0x12
	SSD1306_sendCommandI2C(SSD1306_SETCONTRAST, l_I2CAddress);			// 0x81
	SSD1306_sendCommandI2C(0xCF, l_I2CAddress);							// 0xCF
	SSD1306_sendCommandI2C(SSD1306_SETPRECHARGE, l_I2CAddress);			// 0xD9
	SSD1306_sendCommandI2C(0xF1, l_I2CAddress);							// 0xF1
	SSD1306_sendCommandI2C(SSD1306_SETVCOMDETECT, l_I2CAddress);		// 0xDB
	SSD1306_sendCommandI2C(0x40, l_I2CAddress);							// 0x40
	SSD1306_sendCommandI2C(SSD1306_DISPLAYALLON_RESUME, l_I2CAddress);	// 0xA4
	SSD1306_sendCommandI2C(SSD1306_NORMALDISPLAY, l_I2CAddress);		// 0xA6
	SSD1306_sendCommandI2C(SSD1306_DISPLAYON, l_I2CAddress);			// 0xAF
	
	SSD1306FrameBuffer1.IsDirty = 1;
}


void SSD1306_sendCommandI2C(uint8_t command, uint8_t l_I2CAddress) 
{
	i2c_start(l_I2CAddress);
	i2c_write( (0 << SSD1306_CO) | (0 << SSD1306_DC));
	i2c_write(command);
	i2c_stop();
}


void SSD1306_UpdateLCDI2C(struct _SSD1306FrameBuffer* l_FrameBuffer, uint8_t l_I2CAddress) 
{
	unsigned int packet;
	unsigned int packet_byte;
	
	SSD1306_sendCommandI2C(SSD1306_COLUMNADDR, l_I2CAddress);			// 0x21 (33)
	SSD1306_sendCommandI2C(0, l_I2CAddress);
	SSD1306_sendCommandI2C(SSD1306_WIDTH-1, l_I2CAddress);
	
	SSD1306_sendCommandI2C(SSD1306_PAGEADDR, l_I2CAddress);				// 0x22 (34)
	SSD1306_sendCommandI2C(0, l_I2CAddress);
	#if SSD1306_HEIGHT == 64
		SSD1306_sendCommandI2C(7, l_I2CAddress);
	#endif
	#if SSD1306_HEIGHT == 32
		SSD1306_sendCommandI2C(3, l_I2CAddress);
	#endif
	#if SSD1306_HEIGHT == 16
		SSD1306_sendCommandI2C(1, l_I2CAddress);
	#endif
	
	
	// We have to send the buffer as 16 bytes packets
	for (packet = 0; packet < (SSD1306_HEIGHT *SSD1306_WIDTH)/128 ; packet++) 
	{
		i2c_start(l_I2CAddress);
		i2c_write( (0 << SSD1306_CO) | (1 << SSD1306_DC));			// 0x40
		
		for (packet_byte = 0; packet_byte < 16; ++packet_byte) 
		{
			i2c_write(l_FrameBuffer->Framebuffer[packet*16+packet_byte]);
		}
		
		i2c_stop();	
	}
	l_FrameBuffer->IsDirty=0;		
}


// ------------------------------------------------------------------------------------------------------
// SPI low level communications
// ------------------------------------------------------------------------------------------------------

void SSD1306_InitSPI()
{
	GPIO_SetOledDC(1);						// Set D/nC high
	GPIO_SetOledCS(1);						// Set nCS high

	GPIO_SetOledCS(0);						// Set nCS low
	GPIO_SetOledDC(0);						// Set D/nC low
	
	spi_send(SSD1306_DISPLAYOFF);			// 0xAE
	spi_send(SSD1306_SETDISPLAYCLOCKDIV);	// 0xD5
	spi_send(0x80);							// 0x80
	spi_send(SSD1306_SETMULTIPLEX);			// 0xA8
	spi_send(SSD1306_WIDTH-1);				// 0x3F
	spi_send(SSD1306_SETDISPLAYOFFSET);		// 0xD3
	spi_send(0x00);							// 0x00
	spi_send(SSD1306_SETSTARTLINE | 0x00);	// 0x40
	spi_send(SSD1306_CHARGEPUMP);			// 0x8D
	spi_send(0x14);							// 0x14
	spi_send(SSD1306_MEMORYMODE);			// 0x20
	spi_send(0x00);							// 0x00
	spi_send(SSD1306_SEGREMAP | 0x1);		// 0xA1
	spi_send(SSD1306_COMSCANDEC);			// 0xC8
	spi_send(SSD1306_SETCOMPINS);			// 0xDA
	spi_send(0x12);							// 0x12
	spi_send(SSD1306_SETCONTRAST);			// 0x81
	spi_send(0xCF);							// 0xCF
	spi_send(SSD1306_SETPRECHARGE);			// 0xD9
	spi_send(0xF1);							// 0xF1
	spi_send(SSD1306_SETVCOMDETECT);		// 0xDB
	spi_send(0x40);							// 0x40
	spi_send(SSD1306_DISPLAYALLON_RESUME);	// 0xA4
	spi_send(SSD1306_NORMALDISPLAY);		// 0xA6
	spi_send(SSD1306_DISPLAYON);			// 0xAF
	
	GPIO_SetOledDC(1);						// Set D/nC high
	GPIO_SetOledCS(1);						// Set nCS high
	
	SSD1306FrameBuffer1.IsDirty = 1;
}


void SSD1306_UpdateLCDSPI(struct _SSD1306FrameBuffer* l_FrameBuffer)
{
	unsigned int packet;
	unsigned int packet_byte;
	
	GPIO_SetOledCS(0);		// Set nCS low
	GPIO_SetOledDC(0);		// Set D/nC low
	
	spi_send(SSD1306_COLUMNADDR);				// 0x21 (33)
	spi_send(0);
	spi_send(SSD1306_WIDTH-1);
	
	spi_send(SSD1306_PAGEADDR);				// 0x22 (34)
	spi_send(0);
	#if SSD1306_HEIGHT == 64
	spi_send(7);
	#endif
	#if SSD1306_HEIGHT == 32
	spi_send(3);
	#endif
	#if SSD1306_HEIGHT == 16
	spi_send(1);
	#endif
	
	GPIO_SetOledDC(1);		// Set D/nC high
		
	// We have to send the buffer as 16 bytes packets
	for (packet = 0; packet < (SSD1306_HEIGHT *SSD1306_WIDTH)/128 ; packet++)
	{		
		for (packet_byte = 0; packet_byte < 16; ++packet_byte)
		{
			spi_send(l_FrameBuffer->Framebuffer[packet*16+packet_byte]);
		}
	}
	
	GPIO_SetOledCS(1);		// Set nCS high
	
	l_FrameBuffer->IsDirty=0;
}


// ------------------------------------------------------------------------------------------------------
// Framebuffer
// ------------------------------------------------------------------------------------------------------

void SSD1306_drawPixel(uint8_t pos_x, uint8_t pos_y, uint8_t pixel_status, struct _SSD1306FrameBuffer* l_FrameBuffer) 
{
	if (pos_x >= SSD1306_WIDTH || pos_y >= SSD1306_HEIGHT) 
	{
		return;
	}

	if (pixel_status) 
	{
		l_FrameBuffer->Framebuffer[pos_x+(pos_y/8)*SSD1306_WIDTH] |= (1 << (pos_y&7));
	} 
	else 
	{
		l_FrameBuffer->Framebuffer[pos_x+(pos_y/8)*SSD1306_WIDTH] &= ~(1 << (pos_y&7));
	}
	
	l_FrameBuffer->IsDirty=1;
}


void SSD1306_drawVLine(uint8_t x, uint8_t y, uint8_t length, struct _SSD1306FrameBuffer* l_FrameBuffer) 
{
	for (uint8_t i = 0; i < length; ++i) 
	{
		SSD1306_drawPixel(x,i+y, ON, l_FrameBuffer);
	}
}


void SSD1306_drawHLine(uint8_t x, uint8_t y, uint8_t length, struct _SSD1306FrameBuffer* l_FrameBuffer) 
{
	for (uint8_t i = 0; i < length; ++i) 
	{
		SSD1306_drawPixel(i+x,y, ON, l_FrameBuffer);
	}
}


void SSD1306_drawRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t fill, struct _SSD1306FrameBuffer* l_FrameBuffer) 
{
	uint8_t length = x2 - x1 + 1;
	uint8_t height = y2 - y1;
			
	if (!fill) 
	{
		SSD1306_drawHLine(x1,y1,length, l_FrameBuffer);
		SSD1306_drawHLine(x1,y2,length, l_FrameBuffer);
		SSD1306_drawVLine(x1,y1,height, l_FrameBuffer);
		SSD1306_drawVLine(x2,y1,height, l_FrameBuffer);
	} 
	else 
	{
		for (int16_t x = 0; x < length; ++x) 
		{
			for (int16_t y = 0; y <= height; ++y) 
			{
				SSD1306_drawPixel(x1+x,y+y1, ON, l_FrameBuffer);
			}
		}
	}
}


void SSD1306_clear(struct _SSD1306FrameBuffer* l_FrameBuffer) 
{
	for (uint16_t buffer_location = 0; buffer_location < SSD1306_BUFFERSIZE; buffer_location++) 
	{
		l_FrameBuffer->Framebuffer[buffer_location] = 0x00;
	}
	
	l_FrameBuffer->IsDirty=1;
}


void SSD1306_putc(uint8_t X, uint8_t Y, uint8_t Character, uint8_t FontSize, struct _SSD1306FrameBuffer* l_FrameBuffer)
{
	// X goes from 0 to 64 (for an 48x64 display). Y goes from 0 to 5 (6 lines)
	uint8_t l_Counter=0;
	
	switch (FontSize)
	{
		case FONTSMALL:
		for (l_Counter=0;l_Counter<3;l_Counter++)
		{
			l_FrameBuffer->Framebuffer[X + l_Counter + Y*SSD1306_WIDTH] = pgm_read_byte (&(g_Font3[ (3 * (Character - 0x20)) + l_Counter]));
		}
		break;
		
		case FONTMEDIUM:
		for (l_Counter=0;l_Counter<5;l_Counter++)
		{
			l_FrameBuffer->Framebuffer[X + l_Counter + Y*SSD1306_WIDTH] = pgm_read_byte (&(g_Font5[ (5 * (Character - 0x20)) + l_Counter]));
		}
		break;
		
		case FONTLARGE:
		for (l_Counter=0;l_Counter<7;l_Counter++)
		{
			l_FrameBuffer->Framebuffer[X + l_Counter + Y*SSD1306_WIDTH] = pgm_read_byte (&(g_Font7[ (7 * (Character - 0x20)) + l_Counter]));
		}
		break;
		
		default:
			while(1);	// error
		break;
	}
	
	l_FrameBuffer->IsDirty=1;
}


void SSD1306_puts(uint8_t X, uint8_t Y, const char Text[], uint8_t FontSize, struct _SSD1306FrameBuffer* l_FrameBuffer)
{
	// X goes from 0 to 64 (for an 48x64 display). Y goes from 0 to 5 (6 lines)
	uint8_t l_Index = 0;
	
	while (Text[l_Index]!=0)
	{
		SSD1306_putc (X + (l_Index * (FontSize + 1)), Y ,Text[l_Index], FontSize, l_FrameBuffer);	// Fontsize+1 is to add a space between the letters
		l_Index++;
	}
}


void ssd1306_drawline(struct _SSD1306FrameBuffer* l_FrameBuffer, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
	int16_t p;
	uint8_t dx, dy, octant;
	
	// swap Y coordinates if needed to always approach calculations from lower point to higher point
	if (y0 > y1)
	{
		uint8_t tmp = y0;
		y0 = y1;
		y1 = tmp;
		tmp = x0;
		x0 = x1;
		x1 = tmp;
	}
	
	// Calculate the initial delta constants
	if (x0>x1)
	{
		dx = x0-x1;
	}
	else
	{
		dx = x1-x0;
	}
	
	if (y0>y1)
	{
		dy = y0-y1;
	}
	else
	{
		dy = y1-y0;
	}
	
	     if ( (x0 >= x1) && (dx >= dy) ) octant = 4;	// also valid for octant 0 but since the Y direction is always lower to higher, octant 0 is not used.
	else if ( (x0 >= x1) && (dx <= dy) ) octant = 5;	// also valid for octant 1 but since the Y direction is always lower to higher, octant 1 is not used.
	else if ( (x0 <= x1) && (dx <= dy) ) octant = 6;	// also valid for octant 2 but since the Y direction is always lower to higher, octant 2 is not used.
	else if ( (x0 <= x1) && (dx >= dy) ) octant = 7;	// Also valid for octant 3 but since the Y direction is always lower to higher, octant 3 is not used.
	
	switch (octant)
	{
		// --------------------------------------------------------------------
		case 4:
		
		p = 2*(int16_t)dy-(int16_t)dx;
				
		while(x0 >= x1 && x0 <= SSD1306_WIDTH)
		{
			SSD1306_drawPixel(x0, y0, 1, l_FrameBuffer);
			x0--;
		
			if (p < 0)
			{
				p = p + 2*(int16_t)dy;
			}
			else
			{
				p = p + 2*(int16_t)dy - 2*(int16_t)dx;
				y0++;
			}
		}
		
		break;
		
		
		// --------------------------------------------------------------------
		case 5:
		
		p = 2*(int16_t)dx-(int16_t)dy;
				
		while(y0 <= y1)
		{
			SSD1306_drawPixel(x0, y0, 1, l_FrameBuffer);
			y0++;
		
			if (p < 0)
			{
				p = p + 2*(int16_t)dx;
			}
			else
			{
				p = p + 2*(int16_t)dx - 2*(int16_t)dy;
				x0--;
			}
		}
		
		break;
		
		// --------------------------------------------------------------------
		case 6:
		
		p = 2*(int16_t)dx-(int16_t)dy;
				
		while(y0 <= y1)
		{
			SSD1306_drawPixel(x0, y0, 1, l_FrameBuffer);
			y0++;
		
			if (p < 0)
			{
				p = p + 2*(int16_t)dx;
			}
			else
			{
				p = p + 2*(int16_t)dx - 2*(int16_t)dy;
				x0++;
			}
		}
		
		break;
		// --------------------------------------------------------------------
		case 7:
		
		p = 2*(int16_t)dy-(int16_t)dx;
				
		while(x0 <= x1)
		{
			SSD1306_drawPixel(x0, y0, 1, l_FrameBuffer);
			x0++;
		
			if (p < 0)
			{
				p = p + 2*(int16_t)dy;
			}
			else
			{
				p = p + 2*(int16_t)dy - 2*(int16_t)dx;
				y0++;
			}
		}
		
		break;
		// --------------------------------------------------------------------
	
	}
	
}
