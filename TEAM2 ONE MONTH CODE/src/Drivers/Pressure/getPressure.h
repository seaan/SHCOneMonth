/*
 * getPressure.h
 *
 * Created: 9/22/2016 11:45:14 AM
 *  Author: seanw
 */ 

//TEMPLATE FROM GITHUB, PROTOTYPING..
#ifndef GETPRESSURE_H_
#define GETPRESSURE_H_

uint16_t getPressure(void);
void spi_write(uint8_t data);
uint8_t spi_read(void);


#endif /* GETPRESSURE_H_ */