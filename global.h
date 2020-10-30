/*
 * global.h
 *
 * Created: 06/11/2018 21:04:22
 *  Author: Petervg
 */ 


#ifndef GLOBAL_H_
#define GLOBAL_H_

#define F_CPU 16000000UL

// Globally available variables.
extern volatile uint16_t g_tick;
extern volatile uint8_t g_LcdUpdateTimer;
extern volatile uint16_t g_DemoTimer;
extern struct _SSD1306FrameBuffer SSD1306FrameBuffer1;

// Globally available structs
extern struct _RxBuffer UART_RxBuffer;

// Various defines
#define ON	1
#define OFF	0

#define HIGH	1
#define LOW		0

#define LOWERCASE 0
#define UPPERCASE 1

#define FONTSMALL	3
#define FONTMEDIUM	5
#define FONTLARGE	7

#define FALSE	0
#define TRUE	1


#endif /* GLOBAL_H_ */