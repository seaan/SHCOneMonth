/*
 * SPI_Init.h
 *
 * Created: 9/24/2016 9:59:19 AM
 *  Author: seanw
 */ 
#include <asf.h>

#ifndef SPI_INIT_H_
#define SPI_INIT_H_

void SPI_init(void);
uint8_t spi_read(void);
void spi_write(uint8_t data);


#endif /* SPI_INIT_H_ */