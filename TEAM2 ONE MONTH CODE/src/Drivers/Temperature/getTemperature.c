/*
 * getTemperature.c
 *
 * Created: 9/22/2016 11:48:26 AM
 *  Author: seanw
 */ 

 #include <adc.h>

 //getTemperature from NTC Thermistor 10k Bead

 float getTemperature(){
	ADCA.CH0.CTRL |= 0b10000000; //Start the conversion.. FOR THOMAS: THIS IS A BITWISE OPERATOR THAT KEEPS 1's and MODIFIES ZEROES.
		
	while(ADCA.CH0.INTFLAGS == 0); //Wait until conversion is done.
		
	uint16_t adcReading = ADCA.CH0.RES; //Save the result into variable called adcReading.
		
	uint16_t voltage = (uint16_t)((0.0005*adcReading) - 0.0941); //This converts adcReading into an actual voltage based off of slope. NEED TO TEST AND FIND THE SLOPE! voltage*1000 converts it into millivolts.
	//If we hadn't multiplied by 1000, the typecast would've truncated the voltage reading to just the one's place.

	float resistance = 10000(voltage - 3.3) /3.3; //Need to convert our voltage reading into the resistance across the thermistor. To find this we use Rb(Vin - Vout) / Vout
	
	float a = 0.003354016; //A value for the NTCLE100E3103HT1 in the Steinhart-Hart equation (T = 1/(A + Bln(resistance) + Dln^3(resistance))
	float b = 0.000256985; //B value
	float d = 0.00000006383091; //D value.
	return 1/(a+ b*log(resistance) + d*log(resistance)*log(resistance)*log(resistance)); //Steinhart-Hart to find temperature then return it.
 }