/*
 * getTemperature.c
 *
 * Created: 9/22/2016 11:48:26 AM
 *  Author: seanw
 */ 

 #include <asf.h>
 #include <math.h>
 #include "Drivers/Temperature/getTemperature.h"

 /* getTemperature method for NTC Thermistor 10k Bead */

 float getTemperature(void){
	ADCA.CH0.CTRL |= 0b10000000; //Start the conversion.. FOR THOMAS: THIS IS A BITWISE OPERATOR THAT KEEPS 1's and MODIFIES ZEROES.
		
	while(ADCA.CH0.INTFLAGS == 0); //Wait until conversion is done.
		
	uint16_t read_adc = ADCA.CH0.RES; //Save the result into variable called adcReading.
	printf("read adc: %i\n",read_adc);
	float voltage = (0.0005*read_adc - 0.0941); //Find 
	printf("volt: %i\n",voltage);  
																/* 
																* This converts adcReading into an actual voltage based off of slope. NEED TO TEST AND FIND THE SLOPE! voltage*1000 converts it into millivolts.
																* If we hadn't multiplied by 1000, the typecast would've truncated the voltage reading to just the one's place.
															    */

	float resistance = (3.3*10000)/(voltage - 10000); //Need to convert our voltage reading into the resistance across the thermistor. To find this we use Rb(Vin - Vout) / Vout
	printf("res: %i\n",resistance);
	
	float a = 0.003354016; //A value for the NTCLE100E3103HT1 in the Steinhart-Hart equation (T = 1/(A + Bln(resistance) + Dln^3(resistance))
	float b = 0.000256985; //B value.
	float c = 0.000002620131; //C value.
	float d = 0.00000006383091; //D value.
	return -273.15 + 1/(a+ b*log(resistance/10000) + c*(log(resistance/10000)*log(resistance/10000)) + d*(log(resistance/10000)*log(resistance/10000)*log(resistance/10000))); //Steinhart-Hart to find temperature then return it as celsius.
 }