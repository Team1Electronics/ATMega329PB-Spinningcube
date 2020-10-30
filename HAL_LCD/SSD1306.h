/*
 * SSD1306.h
 *
 * Created: 07/11/2018 17:45:40
 *  Author: Petervg
 * 
 * 
 * 7-bit I2C address 0x3C (0x78/0x79) or 0x3D (0x7A/0x7B)
 * 
 *
 *
 * Write mode for I2C:
 * 
 * 1) The master device initiates the data communication by a start condition. The definition of the start
 * condition is shown in Figure 8-8. The start condition is established by pulling the SDA from HIGH to
 * LOW while the SCL stays HIGH.
 * 
 * 2) The slave address is following the start condition for recognition use. For the SSD1306, the slave
 * address is either “b0111100” or “b0111101” by changing the SA0 to LOW or HIGH (D/C pin acts as
 * SA0).
 * 
 * 3) The write mode is established by setting the R/W# bit to logic “0”.
 * 
 * 4) An acknowledgement signal will be generated after receiving one byte of data, including the slave
 * address and the R/W# bit. Please refer to the Figure 8-9 for the graphical representation of the
 * acknowledge signal. The acknowledge bit is defined as the SDA line is pulled down during the HIGH
 * period of the acknowledgement related clock pulse.
 * 
 * 5) After the transmission of the slave address, either the control byte or the data byte may be sent across
 * the SDA. A control byte mainly consists of Co and D/C# bits following by six “0” ‘s.
 * a. If the Co bit is set as logic “0”, the transmission of the following information will contain
 * data bytes only.
 * b. The D/C# bit determines the next data byte is acted as a command or a data. If the D/C# bit is
 * set to logic “0”, it defines the following data byte as a command. If the D/C# bit is set to
 * logic “1”, it defines the following data byte as a data which will be stored at the GDDRAM.
 * The GDDRAM column address pointer will be increased by one automatically after each
 * data write.
 * 
 * 6) Acknowledge bit will be generated after receiving each control byte or data byte.
 * 
 * 7) The write mode will be finished when a stop condition is applied. The stop condition is also defined
 * in Figure 8-8. The stop condition is established by pulling the “SDA in” from LOW to HIGH while
 * the “SCL” stays HIGH
* 
 */
 


#ifndef SSD1306_H_
#define SSD1306_H_

#define SSD1306_ADDRESS1			0x78
#define SSD1306_ADDRESS2			0x7A
#define SSD1306_SETCONTRAST			0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON		0xA5
#define SSD1306_NORMALDISPLAY		0xA6
#define SSD1306_INVERTDISPLAY		0xA7
#define SSD1306_DISPLAYOFF			0xAE
#define SSD1306_DISPLAYON			0xAF
#define SSD1306_SETDISPLAYOFFSET	0xD3
#define SSD1306_SETCOMPINS			0xDA
#define SSD1306_SETVCOMDETECT		0xDB
#define SSD1306_SETDISPLAYCLOCKDIV	0xD5
#define SSD1306_SETPRECHARGE		0xD9
#define SSD1306_SETMULTIPLEX		0xA8
#define SSD1306_SETLOWCOLUMN		0x00
#define SSD1306_SETHIGHCOLUMN		0x10
#define SSD1306_SETSTARTLINE		0x40
#define SSD1306_MEMORYMODE			0x20
#define SSD1306_COLUMNADDR			0x21
#define SSD1306_PAGEADDR			0x22
#define SSD1306_COMSCANINC			0xC0
#define SSD1306_COMSCANDEC			0xC8
#define SSD1306_SEGREMAP			0xA0
#define SSD1306_CHARGEPUMP			0x8D
#define SSD1306_SWITCHCAPVCC		0x2
#define SSD1306_NOP					0xE3

#define SSD1306_CO	7			// Continuation bit
#define SSD1306_DC	6			// Data/command

#define RESETI2C	0
#define RESETSPI	1

#define OLEDSPI		0
#define OLEDI2C		1
#define OLEDBOTH	2

//#define SSD1306_WIDTH 64
//#define SSD1306_HEIGHT 48

#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64

#define SSD1306_BUFFERSIZE (SSD1306_WIDTH*SSD1306_HEIGHT)/8

struct _SSD1306FrameBuffer
{
	unsigned char IsDirty;
	unsigned char Framebuffer[SSD1306_BUFFERSIZE];
};

// Copy framebuffer to display
void ProcessLCD(uint8_t l_Displaytype);

// Init the SSD1306 driver
void SSD1306_InitI2C(uint8_t l_I2CAddress); 
void SSD1306_InitSPI();

// Copy the framebuffer to the SSD1306 driver
void SSD1306_UpdateLCDI2C(struct _SSD1306FrameBuffer* l_FrameBuffer, uint8_t l_I2CAddress);
void SSD1306_UpdateLCDSPI(struct _SSD1306FrameBuffer* l_FrameBuffer);

// Send a command to the SSD1306 driver
void SSD1306_sendCommandI2C(uint8_t command, uint8_t l_I2CAddress);

// Clear the framebuffer
void SSD1306_clear(struct _SSD1306FrameBuffer* l_FrameBuffer);

// High level text functions
void SSD1306_putc(uint8_t X, uint8_t Y, uint8_t Character, uint8_t FontSize, struct _SSD1306FrameBuffer* l_FrameBuffer);
void SSD1306_puts(uint8_t X, uint8_t Y, const char Text[], uint8_t FontSize, struct _SSD1306FrameBuffer* l_FrameBuffer);

// High level graphics functions
void SSD1306_drawPixel(uint8_t pos_x, uint8_t pos_y, uint8_t pixel_status, struct _SSD1306FrameBuffer* l_FrameBuffer);
void SSD1306_drawVLine(uint8_t x, uint8_t y, uint8_t length, struct _SSD1306FrameBuffer* l_FrameBuffer);
void SSD1306_drawHLine(uint8_t pos_y, uint8_t start, uint8_t length, struct _SSD1306FrameBuffer* l_FrameBuffer);
void SSD1306_drawRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t fill, struct _SSD1306FrameBuffer* l_FrameBuffer);
void ssd1306_drawline(struct _SSD1306FrameBuffer* l_FrameBuffer, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);

//void ssd1306_drawline(struct _SSD1306FrameBuffer* l_FrameBuffer, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

#endif /* SSD1306_H_ */