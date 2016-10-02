/*
 * getAltitude.c
 *
 * Class to hold methods used for testing various functions used in the project.
 *
 * Created: 9/22/2016 11:45:21 AM
 *  Author: seanw
 */

 /* Includes */
 #include "test/testClass.h"
 #include "Drivers/Pressure/getPressure.h"
 #include "Drivers/Temperature/getTemperature.h"
 #include "Calculations/Altitude/getAltitude.h"
 #include "Calculations/Velocity/getVelocity.h"
 #include "Drivers/Outputs/LED/LED_driver.h"
 #include "Drivers/Outputs/Buzzer/Buzzer_Driver.h"
 
 /* Global Variable Declarations */
 uint8_t lightNumber;
 float voltage;
 uint16_t read_adc;

/*******************************************************************************************/
									/* Test methods */
/* 
 * A method written to make a rectangular light chase on the MCU board, where the light that is on cycles from 0-3, then to 7 and down to 4.
 * delay input defines the amount of time the function waits between each shift.
 */
 void lightChase(uint8_t delay){
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

 /* Testing method to print out various sensor inputs and calculations */
void test(void){
	//TCE0_init(12499,100);
	while(1){
		lightChase(100);
		printf("Test:\n");
		//printf("Pressure is:%li\n",getPressure());
		//printf("Temperature is:%.2f\n",getTemperature());
		//printf("Altitude is:%.2f\n",getAltitude(getPressure(),getTemperature())*3.28084);
		//printf("and velocity is:%.2f\n",getVelocity());
	}
}