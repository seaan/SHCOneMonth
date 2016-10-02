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
	//printf("cal_press\n");
	spi_select();
	 spi_write(0b10100010); //prom read ad1
	 c1 = ((uint16_t)spi_read())<<8;
	 c1 += ((uint16_t)spi_read());
	 spi_deselect();
	 //printf("c1: %u\n",c1);

	 spi_select();
	 spi_write(0b10100100); //prom read ad2
	 c2 = ((uint16_t)spi_read())<<8;
	 c2 += ((uint16_t)spi_read());
	 spi_deselect();
	 //printf("c2: %u\n",c2);

	 spi_select();
	 spi_write(0b10100110); //prom read ad3
	 c3 = ((uint16_t)spi_read())<<8;
	 c3 += ((uint16_t)spi_read());
	 spi_deselect();
	 //printf("c3: %u\n",c3);

	 spi_select();
	 spi_write(0b10101000); //prom read ad4
	 c4 = ((uint16_t)spi_read())<<8;
	 c4 += ((uint16_t)spi_read());
	 spi_deselect();
	 //printf("c4: %u\n",c4);

	 spi_select();
	 spi_write(0b10101010); //prom read ad5
	 c5 = ((uint16_t)spi_read())<<8;
	 c5 += ((uint16_t)spi_read());
	 spi_deselect();
	 //printf("c5: %u\n",c5);

	 spi_select();
	 spi_write(0b10101100); //prom read ad6
	 c6 = ((uint16_t)spi_read())<<8;
	 c6 += ((uint16_t)spi_read());
	 spi_deselect();
	 //printf("c6: %u\n",c6);
 }
 
 void SPI_init(void){
	 //printf("spi_init\n");
	 PORTC.DIR |= 0b10110000; //Set port to input.
	 PORTC.DIR &= 0b10111111;
	 SPIC.CTRL = 0b01010000; //Clock Double off, enable SPI, MSB DORD, master mode, SPI transfer mode set to 0, prescalar set to clkper/4 (has no effect in slave mode).
	 calibratePressure();
 }
 void spi_select(void){
	PORTC.OUT &= 0b11101111;
 }
 void spi_deselect(void){
	PORTC.OUT |= 0b00010000;
 }
 uint8_t spi_read(void){
	 //printf("spi_read\n");
	 SPIC.DATA = 0xFF; //Set the data to something we know.
	 while(!(SPIC.STATUS>>7)); //Wait until new data comes in by monitoring the interrupt flag.
	 return SPIC.DATA; //Return the data.
 }

 void spi_write(uint8_t data){
	 //printf("spi_write\n");
	 SPIC.DATA = data; //Take data from function argument and write it.
	 //printf("1\n");
	 while(!(SPIC.STATUS>>7)); //Wait until data has been sent.
	 //printf("2\n");
 }
 
 int32_t getPressure(void){
	//printf("get_press\n");
	spi_select();
	spi_write(0x48); //Ask for pressure value
	spi_deselect();
	delay_ms(10);

	spi_select();
	spi_write(0x00);
	uint32_t d1 = ((uint32_t)spi_read())<<16; //Typecast the 8 bit data to 32 bit, then move it 16 places to the left. Stores in temp variable.
	d1 += ((uint32_t)spi_read())<<8; //Assign data to data + the new data but in the lower 8 bits.
	d1 += ((uint32_t)spi_read());
	spi_deselect();
	//printf("d1: %li\n",d1);
	
	spi_select();
	spi_write(0x58); //Ask for temp value.
	spi_deselect();
	delay_ms(10);
	
	spi_select();
	spi_write(0x00);
	uint32_t d2 = ((uint32_t)spi_read())<<16;
	d2 += ((uint32_t)spi_read())<<8;
	d2 += ((uint32_t)spi_read());
	spi_deselect();
	//printf("d2: %li\n",d2);
	

	int32_t dT = (int64_t)d2 - (((int64_t)c5)*(256));
	int64_t offset = ((int64_t)c2)*(65536) + ((((int64_t)c4)*((int64_t)dT))/(128));
	int64_t sens = (((int64_t)c1)*(32768)) + ((((int64_t)c3)*((int64_t)dT))/256);
	int32_t pressure = ((((int64_t)d1) * (((uint64_t)sens)/2097152)) - (int64_t)offset)/32768;
	
	return pressure;
}