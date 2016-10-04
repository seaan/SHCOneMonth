/*
 * getTemperature.c
 *
 * Temperature class for NTC Thermistor 10k Bead.
 *
 * Created: 9/22/2016 11:45:21 AM
 *  Author: seanw
 */

 #include <asf.h>
 #include <math.h>
 #include "Drivers/Temperature/getTemperature.h"

  /*******************************************************************************************/
							/* getTemperature method */

/* Method that uses change in voltage to find resistance across thermistor and thus temperature w/ Steinhart-Hart equation. */
 float getTemperature(void){
	ADCA.CH0.CTRL |= 0b10000000; //Start the conversion..
	while(ADCA.CH0.INTFLAGS == 0); //Wait until conversion is done.
	uint16_t read_adc = ADCA.CH0.RES; // save reading

	float voltage = (0.000502512562*read_adc - 0.095979899); //converts the adc reading into an actual voltage based off of slope.
	//printf("%.2f\n", voltage);
	float resistance = (3.3*10000)/(3.3 - voltage) - 10000; //Need to convert our voltage reading into the resistance across the thermistor. To find this we use 3.3(R1)/(3.3-vadc) - R1
	//printf("%.2f\n",resistance);
	float a_const = 0.003354016; //A value for the NTCLE100E3103HT1 in the Steinhart-Hart equation (T = 1/(A + Bln(resistance) + Dln^3(resistance))
	float b_const = 0.000256985; //B value
	float d_const = 0.00000006383091; //D value.
	return 1/(a_const+ b_const*log(resistance/10000) + d_const*log(resistance/10000)*log(resistance/10000)*log(resistance/10000)); //Steinhart-Hart to find temperature then return it.
 }