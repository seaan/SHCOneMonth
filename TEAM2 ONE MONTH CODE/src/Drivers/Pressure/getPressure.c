/*
 * getPressure.c
 *
 * Created: 9/22/2016 11:45:21 AM
 *  Author: seanw
 */
 #include <asf.h>
 #include <math.h>
 #include "Drivers/Pressure/getPressure.h"
 
 uint16_t c1;
 uint16_t c2;
 uint16_t c3;
 uint16_t c4;
 uint16_t c5;
 uint16_t c6;
 
 void calibratePressure(void){
	 spi_write(0b10100010); //prom read ad1
	 c1 = ((uint16_t)spi_read())<<8;
	 c1 += ((uint16_t)spi_read());
	 
	 spi_write(0b10100100); //prom read ad2
	 c2 = ((uint16_t)spi_read())<<8;
	 c2 += ((uint16_t)spi_read());
	 
	 spi_write(0b10100110); //prom read ad3
	 c3 = ((uint16_t)spi_read())<<8;
	 c3 += ((uint16_t)spi_read());
	 
	 spi_write(0b10101000); //prom read ad4
	 c4 = ((uint16_t)spi_read())<<8;
	 c4 += ((uint16_t)spi_read());
	 
	 spi_write(0b10101010); //prom read ad5
	 c5 = ((uint16_t)spi_read())<<8;
	 c5 += ((uint16_t)spi_read());
	 
	 spi_write(0b10101100); //prom read ad6
	 c6 = ((uint16_t)spi_read())<<8;
	 c6 += ((uint16_t)spi_read());
 }
 
 void SPI_init(void){
	 PORTC.DIR |= 0b00000000; //Set port to input.
	 PORTC.OUT |= 0b00000001; 
	 SPIC.CTRL = 0b01000000; //Clock Double off, enable SPI, MSB DORD, slave mode, SPI transfer mode set to 0, prescalar set to clkper/4 (has no effect in slave mode).
	 SPIC.INTCTRL = 0b00000011; //High level interrupt.
	 PMIC.CTRL = 0b00000100; //Enable high-priority interrupts.
	 sei(); //Enable global interrupts.
	 calibratePressure();
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
 
 uint16_t getPressure(void){
	spi_write(0x48); //Ask for pressure value
	uint16_t d1 = ((uint16_t)spi_read())<<8; //Typecast the 8 bit data to 16 bit, then move it 8 places to the left. Stores in temp variable.
	d1 += ((uint16_t)spi_read()); //Assign data to data + the new data but in the lower 8 bits.
	
	spi_write(0x58); //Ask for temp value.
	uint16_t d2 = ((uint16_t)spi_read())<<8;
	d2 += ((uint16_t)spi_read());
	
	uint32_t dT = d2 - c5*pow(2,8);
	uint64_t offset = c2*pow(2,16) + (c4*dT)/pow(2,7);
	uint64_t sens = c1*pow(2,15) + (c3*dT)/pow(2,8);
	uint32_t pressure = (d1 * sens/pow(2,21) - offset)/pow(2,15);
	
	return pressure * 100;
}