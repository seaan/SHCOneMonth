/*
 * getPressure.c
 *
 * Created: 9/22/2016 11:45:21 AM
 *  Author: seanw
 */
 #include <asf.h>
 #include "Drivers/Pressure/getPressure.h"
 #include "Drivers/SPI/SPI_driver.h"
 uint16_t getPressure(void){
	uint16_t pressure; //Temporary data variable.
	spi_write(0x00); //Write the data to the SPI device.
	pressure = ((uint16_t)spi_read())<<8; //Typecast the 8 bit data to 16 bit, then move it 8 places to the left.
	pressure += ((uint16_t)spi_read()); //Assign data to data + the new data but in the lower 8 bits.

	return pressure * 100;
}