/*
 * SPI_Init.c
 *
 * Created: 9/24/2016 9:59:32 AM
 *  Author: seanw
 */ 
 #include "Drivers/SPI/SPI_driver.h"
 #include <asf.h>

 void SPI_init(void){
	SPIC.CTRL = 0b01000000; //Clock Double off, enable SPI, MSB DORD, slave mode, SPI transfer mode set to 0, prescalar set to clkper/4 (has no effect in slave mode).
	SPIC.INTCTRL = 0b00000010; //Medium level interrupt.
 }

 uint8_t spi_read(void){
	SPIC.DATA = 0xFF; //Set the data to something we know.
	while(!(SPIC.STATUS>>7)); //Wait until new data comes in by monitoring the interrupt flag.
	return SPIC.DATA; //Return the data.
 }

 void spi_write(uint8_t data){
	SPIC.DATA = data; //Take data from function argument and write it.
	while(!(SPIC.STATUS>>7)); //Wait until data has been sent.
 }
 void getReferencePressure(){
	
 }