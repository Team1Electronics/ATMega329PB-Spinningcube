/*
 * UART.h
 *
 * Created: 01/11/2018 19:29:52
 *  Author: Petervg
 */ 


#ifndef UART_H_
#define UART_H_



void init_uart(uint16_t l_UBRR, uint8_t l_doublespeed);
void uart_putc(uint8_t c);
void uart_puts(const uint8_t* s);
void uart_putvar(uint8_t* s);




#endif /* UART_H_ */