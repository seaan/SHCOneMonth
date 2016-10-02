/*
 * getPressure.c
 *
 * Pressure and SPI class for MS5611 pressure sensor.
 *
 * Created: 9/22/2016 11:45:21 AM
 *  Author: seanw
 */
 #include <asf.h>
 #include <math.h>
 #include "Drivers/Pressure/getPressure.h"
 
 /* Global Variable Declaration */
 uint16_t cal_pres_c1;
 uint16_t cal_pres_c2;
 uint16_t cal_pres_c3;
 uint16_t cal_pres_c4;
 uint16_t cal_pres_c5;
 uint16_t cal_pres_c6;
 
 /*******************************************************************************************/
              /* Initialization and Calibration for SPI and Pressure Sensor */

 /* Configuring SPI module.*/
 void SPI_init(void){
	 //printf("spi_init\n");
	 PORTC.DIR |= 0b10110000; //Set port to input.
	 PORTC.DIR &= 0b10111111;
	 SPIC.CTRL = 0b01010000; //Clock Double off, enable SPI, MSB DORD, master mode, SPI transfer mode set to 0, prescalar set to clkper/4 (has no effect in slave mode).
	 calibratePressure();
 }
 /* Calibrate pressure from factory settings on the MS5611 */
 void calibratePressure(void){
	/* Calibrate value c1 */
	 spi_select();
	 spi_write(0b10100010); //prom read ad1
	 cal_pres_c1 = ((uint16_t)spi_read())<<8; //bitshift
	 cal_pres_c1 += ((uint16_t)spi_read());
	 spi_deselect();

	/* Calibrate value c2 */
	 spi_select();
	 spi_write(0b10100100); //prom read ad2
	 cal_pres_c2 = ((uint16_t)spi_read())<<8;
	 cal_pres_c2 += ((uint16_t)spi_read());
	 spi_deselect();

	/* Calibrate value c3 */
	 spi_select();
	 spi_write(0b10100110); //prom read ad3
	 cal_pres_c3 = ((uint16_t)spi_read())<<8;
	 cal_pres_c3 += ((uint16_t)spi_read());
	 spi_deselect();

	/* Calibrate value c4 */
	 spi_select();
	 spi_write(0b10101000); //prom read ad4
	 cal_pres_c4 = ((uint16_t)spi_read())<<8;
	 cal_pres_c4 += ((uint16_t)spi_read());
	 spi_deselect();

	/* Calibrate value c5 */
	 spi_select();
	 spi_write(0b10101010); //prom read ad5
	 cal_pres_c5 = ((uint16_t)spi_read())<<8;
	 cal_pres_c5 += ((uint16_t)spi_read());
	 spi_deselect();

	/* Calibrate value c6 */
	 spi_select();
	 spi_write(0b10101100); //prom read ad6
	 cal_pres_c6 = ((uint16_t)spi_read())<<8;
	 cal_pres_c6 += ((uint16_t)spi_read());
	 spi_deselect();
 }

  /*******************************************************************************************/
								/* SPI Usability Methods */
 void spi_select(void){
	PORTC.OUT &= 0b11101111;
 }
 void spi_deselect(void){
	PORTC.OUT |= 0b00010000;
 }
 uint8_t spi_read(void){
	 SPIC.DATA = 0xFF; //Set the data to something we know.
	 while(!(SPIC.STATUS>>7)); //Wait until new data comes in by monitoring the interrupt flag.
	 return SPIC.DATA; //Return the data.
 }

 void spi_write(uint8_t data){
	 SPIC.DATA = data;
	 while(!(SPIC.STATUS>>7));
 }
 
  /*******************************************************************************************/
						/* Get Current Pressure value method */

 int32_t getPressure(void){
	/* Request current pressure value */
	spi_select();
	spi_write(0x48);
	spi_deselect();
	delay_ms(10);

	/* Read current pressure value */
	spi_select();
	spi_write(0x00);
	uint32_t uncal_pressure = ((uint32_t)spi_read())<<16; //Typecast the 8 bit data to 32 bit, then move it 16 places to the left. Stores in temp variable.
	uncal_pressure += ((uint32_t)spi_read())<<8; //Assign data to data + the new data but in the lower 8 bits.
	uncal_pressure += ((uint32_t)spi_read());
	spi_deselect();
	

	/* Request current temperature value */
	spi_select();
	spi_write(0x58); //Ask for temp value.
	spi_deselect();
	delay_ms(10);
	
	/* Read current temperature value */
	spi_select();
	spi_write(0x00);
	uint32_t d2 = ((uint32_t)spi_read())<<16;
	d2 += ((uint32_t)spi_read())<<8;
	d2 += ((uint32_t)spi_read());
	spi_deselect();
	
	/* Calculate temperature-adjusted pressure */
	int32_t dT = (int64_t)d2 - (((int64_t)cal_pres_c5)*(256));
	int64_t offset = ((int64_t)cal_pres_c2)*(65536) + ((((int64_t)cal_pres_c4)*((int64_t)dT))/(128));
	int64_t sens = (((int64_t)cal_pres_c1)*(32768)) + ((((int64_t)cal_pres_c3)*((int64_t)dT))/256);
	int32_t pressure = ((((int64_t)uncal_pressure) * (((uint64_t)sens)/2097152)) - (int64_t)offset)/32768;
	
	
	return pressure; //pressure given in pascals.
}