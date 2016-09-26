/* ONE MONTH TEAM 2 MAIN */

/* Begin #include section */
//This is where you will need to include the header files that you have written code in to use the code
#include <asf.h> //This will mainly include behind the scene code and all header files within the config folder
#include "Drivers/timerCounter/timer_counter_init.h"
#include "Drivers/ADC/ADC_init.h"
#include "test/testClass.h"
#include "Calculations/Altitude/getAltitude.h"
#include "Calculations/Velocity/getVelocity.h"
#include "conf_usart_serial.h"
#include "Drivers/SPI/SPI_driver.h"
#include "Drivers/Outputs/Buzzer/Buzzer_Driver.h"
#include "Drivers/Outputs/LED/LED_driver.h"
#include "Drivers/Outputs/Motor/Motor_driver.h"

/* End #include Section */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Begin #define Section */
//This is where you can define constants, assign names to numbers, etc

#define EXAMPLE		(0xFF) //Now you can type EXAMPLE and the compiler will understand hexadecimal FF, or decimal 255
/* End #define Section */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Begin Global Variable Section */
//This is where you declare global variables, do this sparingly. It's okay to only have a few or no global variables

uint8_t example = 0; //variable example, unsigned 8 bit, starts at 0
uint8_t EP_address = 0;
uint16_t alt = 0;
uint32_t t = 0;
/* End Global Variable Section */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Begin Function Prototype Section */
//This is where you make function prototypes
void flightStateOne(void);
void flightStateTwo(void);
void flightStateThree(void);
void flightStateZero(void);
void deployParachute(void);
/* End Function Prototype Section */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Begin Interrupt Service Routine Section */
//This is where you make ISRs when using interrupts, only some teams will use interrupts as they can be complicated
/* End Interrupt Service Routine Section */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main (void)
{
	/* Initialize the system clock, 32MHz, this also turns off all peripheral clocks */
	sysclk_init();
	rtc_init();

	/* Peripheral clock inits */
	sysclk_enable_peripheral_clock(&USARTC0); //For every peripheral, you must enable the clock like shown here. Ex. Timer counters, SPI, ADCs
	sysclk_enable_peripheral_clock(&ADCA); //Analog to Digital Converter clock initialization.
	sysclk_enable_peripheral_clock(&TCE0); //Timer Counter clock initialization
	sysclk_enable_peripheral_clock(&SPIC); //Serial Port Interface initialization.
	
	/* Example, Timer Counter on PORTE */

	sysclk_enable_module(SYSCLK_PORT_E, SYSCLK_HIRES); //You must have this line for every timer counter due to a flaw in the design of the chip **************************
	sysclk_enable_module(SYSCLK_PORT_C,PR_SPI_bm);
	
	/* Initializations */;
	UART_Comms_Init();
	SPI_init();
	//TCE0_init(12499,100);
	ADC_init();

	/* Flight Code */
	PORTE.DIR = 0b11111111; //Sets all the pins on PORTE as an output.
	PORTE.OUT = 0b00000000; //Sets all of the pins voltage levels to 0V, which is logic 0 in programming.

	while (1){
		//printf("%i\n",rtc_get_time());
		/*
		if(rtc_get_time() - t > 15){
			t = rtc_get_time();
			//nvm_eeprom_write_byte(EP_address,time);
			//EP_address++;
			if(EP_address >= 2047)
				EP_address = 0;
		}
		else
			printf(nvm_eeprom_read_byte(EP_address);

		*/

		/* Flight States */
		//flightStateZero();
		//flightStateOne();
		//flightStateTwo();
		// flightStateThree();
		
		/*Already written TEST cases*/
		//printf("%i\n", ADC_test(250)); //Print the value that ADC_test returns, with a 250ms delay before the print.
		//lightChase(100); //Runs function lightChase with a 50ms delay before switching each light.

	}
	
}
/*********************************************************************/
/*
*					FLIGHT STATE METHODS
* Each completes a certain objective within a certain altitude, then
*  throws to next flight state in the main while loop.
*/

 /* PRE-LAUNCH */
 void flightStateZero(void){
	 LED(249999,5);//Set LED to .5Hz, 5% DC.

	 if(getAltitude() < 15){
		if(rtc_get_time() - time > 300 ){ //Every 5 minutes, save data to eeprom
			nvm_eeprom_write_byte(EP_address,alt); //FIX PLS, CAN ONLY SAVE 1 BYTE AT A TIME LULZ
			EP_address++;
			
			time = rtc_get_time();
			nvm_eeprom_write_byte(EP_address,time);
			EP_address++; 

			if(EP_address >= 2047) //Loops back around if we run out of addresses.
				EP_address = 0;
			}
		flightStateZero();
		}
 }

 /* ASCENT */
 void flightStateOne(void){
	 LED(24999,10);//Set LED to 5Hz, 10% DC.

	 if(getAltitude() < 600){
		 if(rtc_get_time() - time > 0.25 ){ //Every 25 seconds, save data to eeprom.
			 nvm_eeprom_write_byte(EP_address,alt); //FIX PLS, CAN ONLY SAVE 1 BYTE AT A TIME LULZ
			 EP_address++;
			 
			 time = rtc_get_time();
			 nvm_eeprom_write_byte(EP_address,time);
			 EP_address++;

			 if(EP_address >= 2047) //Loops back around if we run out of addresses.
				EP_address = 0;
		 }

		 flightStateOne();
	 }
 }

 /* DESCENT */
 void flightStateTwo(void){
	 LED(12499,10);//Set LED to 10Hz, 10% DC.

	 /* Emergency Parachute*/
	 if (getVelocity() < -70) //rough estimate for what velocity we want to deploy the parachute at
		deployParachute();
	 //
	 if(getAltitude() > 10){
		 if(rtc_get_time() - time > 25 ){ //Every 25 seconds, save data to eeprom.
			 nvm_eeprom_write_byte(EP_address,alt); //FIX PLS, CAN ONLY SAVE 1 BYTE AT A TIME LULZ
			 EP_address++;
			 
			 time = rtc_get_time();
			 nvm_eeprom_write_byte(EP_address,time);
			 EP_address++;

			 if(EP_address >= 2047) //Loops back around if we run out of addresses.
				EP_address = 0;
		 }

		 flightStateOne();
	 }
 }

 /* TOUCHDOWN */
 void flightStateThree(void){
	 LED(124999,10)); //Set LED to 1Hz, 10% DC.
	 buzzer(0,0)//Buzzer @ Hz, DC **********0,0 placeholder, need real values. 

	 for(int i = 0; i < 3; i++){
		 if(rtc_get_time() - time > 300 ){ //Every 
			nvm_eeprom_write_byte(EP_address,alt); //FIX PLS, CAN ONLY SAVE 1 BYTE AT A TIME LULZ
			EP_address++;
			 
			time = rtc_get_time();
			nvm_eeprom_write_byte(EP_address,time);
			EP_address++;

			if(EP_address >= 2047) //Loops back around if we run out of addresses.
				EP_address = 0;
		 }
	}
	while(1);
 }

/* EMERGENCY PARACHUTE DEPLOY */
 void deployParachute(void){
	 

 }