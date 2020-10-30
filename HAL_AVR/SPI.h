/*
 * SPI.h
 *
 * Created: 01/11/2018 19:17:06
 *  Author: Petervg
 */ 


#ifndef SPI_H_
#define SPI_H_

#include <inttypes.h>
#include <avr/io.h>

#define MODE0	0
#define MODE1	1
#define MODE2	2
#define MODE3	3

#define MSBFIRST	0
#define LSBFIRST	1

#define SPISLAVE	0
#define SPIMASTER	1


//initialize the SPI bus
//  uint8_t lsbfirst - if 0: most significant bit is transmitted first
//  uint8_t master - if 1: use master mode, if 0: slave mode is used
//  uint8_t mode - sets the transfer mode:
//                 mode   leading clock edge   trailing clock edge
//                 -----------------------------------------------
//                 0      sample (rising)      setup  (falling)
//                 1      setup  (rising)      sample (falling)
//                 2      sample (falling)     setup  (rising)
//                 3      setup  (falling)     sample (rising)
//  uint8_t clkrate - spi bus clock rate, valid speeds are 0-3
//                    rate   speed
//                    ------------
//                    0      CPUCLK/4
//                    1      CPUCLK/16
//                    2      CPUCLK/64
//                    3      CPUCLK/128
//  uint8_t dblclk - if 1: doubles the SPI clock rate in master mode
//  EXAMPLE: spi_init(0, 1, 0, 3, 0)
void spi_init(uint8_t lsbfirst, uint8_t master, uint8_t mode, uint8_t clkrate, uint8_t dblclk);

//shifts out 8 bits of data
//  uint8_t data - the data to be shifted out
//  returns uint8_t - the data received during sending
uint8_t spi_send(uint8_t value);



#endif /* SPI_H_ */