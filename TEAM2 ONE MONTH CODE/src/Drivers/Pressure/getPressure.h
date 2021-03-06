/*
 * getPressure.h
 *
 * Created: 9/22/2016 11:45:14 AM
 *  Author: seanw
 */ 
#ifndef GETPRESSURE_H_
#define GETPRESSURE_H_

int32_t getPressure(void);
void spi_write(uint8_t data);
uint8_t spi_read(void);
void SPI_init(void);
uint8_t spi_read(void);
void spi_write(uint8_t data);
void calibratePressure(void);
void spi_select(void);
void spi_deselect(void);

#endif /* GETPRESSURE_H_ */