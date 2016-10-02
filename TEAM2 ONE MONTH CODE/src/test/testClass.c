/*
 * testClass.c
 *
 * Created: 9/18/2016 6:30:09 PM
 *  Author: seanw
 */ 
 #include "test/testClass.h"
 #include "Drivers/Pressure/getPressure.h"
 #include "Drivers/Temperature/getTemperature.h"
 #include "Calculations/Altitude/getAltitude.h"
 #include "Calculations/Velocity/getVelocity.h"
 #include "Drivers/Outputs/LED/LED_driver.h"
 #include "Drivers/Outputs/Buzzer/Buzzer_Driver.h"
 
 uint8_t lightNumber;
 float voltage;
 uint16_t read_adc;

 void lightChase(uint8_t delay){ //A function written to make a rectangular light chase on the MCU board, where the light that is on cycles from 0-3, then to 7 and down to 4. msDelay defines the amount of time the function waits between each shift.
			lightNumber = 0b00000001; //All pins on except pin 0.
			for(int i = 0; i < 4; i++){ //Runs 4 times to cycle through pins 0-3.
				PORTE.OUT = ~lightNumber; //Takes inverse of lightChase, meaning all pins are off except pin [i].
				lightNumber <<= 1; //Shifts the pin off for lightChase to the left, meaning that when ~lightChase is next taken, pin [i + 1] is on. 
				delay_ms(delay); //Delay given time.
			}

			lightNumber = 0b10000000; //All pins on except pin 7.
			for(int i = 7; i > 3; i--){ //Cycles through pins 4-7, backwards.
				PORTE.OUT = ~lightNumber;
				lightNumber >>= 1; //Shifts the pin off for lightChase to the right, meaning that when ~lightChase is next taken, pin [i - 1] is on.
				delay_ms(delay);
			}
 }
 int ADC_test(uint8_t delay){ //The ADC function we wrote in the lab using the MCU workshop PDF.
	ADCA.CH0.CTRL |= 0b10000000; //Start the conversion.. FOR THOMAS: THIS IS A BITWISE OPERATOR THAT KEEPS 1's and MODIFIES ZEROES.
	
	while(ADCA.CH0.INTFLAGS == 0); //Wait until conversion is done.
	
	read_adc = ADCA.CH0.RES; //Save the result into variable called adcReading.
	
	voltage = (0.0005*read_adc) - 0.0941; //This converts adcReading into an actual voltage based off of slope.
	
	return (uint16_t)(voltage*1000); //voltage*1000 converts it into millivolts.
	//If we hadn't multiplied by 1000, the typecast would've truncated the voltage reading to just the one's place.

	delay_ms(delay);
}

void test(void){
	lightChase(100);
	printf("Test:\n");
	printf("Pressure is:%li\n",getPressure());
	printf("Temperature is:%.2f\n",getTemperature());
	printf("Altitude is:%.2f\n",getAltitude());
	printf("and velocity is:%.2f\n",getVelocity());
	//printf("For time:%i\nPressure is:%i\nTemperature is:%f\nAltitude is: %f\nAnd velocity is:%f\n",rtc_get_time(),getPressure(),getTemperature(),getAltitude(),getVelocity());
}