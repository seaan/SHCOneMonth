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
uint8_t alt = 0;
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
	sysclk_enable_peripheral_clock(&TCD0);
	sysclk_enable_peripheral_clock(&TCC0);
	sysclk_enable_peripheral_clock(&SPIC); //Serial Port Interface initialization.
	
	/* Example, Timer Counter on PORTE */

	sysclk_enable_module(SYSCLK_PORT_E, SYSCLK_HIRES); //You must have this line for every timer counter due to a flaw in the design of the chip **************************
	sysclk_enable_module(SYSCLK_PORT_D, SYSCLK_HIRES);
	sysclk_enable_module(SYSCLK_PORT_C,PR_SPI_bm);
	
	/* Initializations */;
	UART_Comms_Init();
	SPI_init();
	//TCE0_init(12499,100);
	TCD0_init();
	ADC_init();
	PORTD.DIR = 0b11111111;
	PORTE.DIR = 0b11111111;
	PORTF.DIR = 0b00000011;

	/* Flight Code */

	/* Flight States */
	//flightStateZero();
	//flightStateOne();
	//flightStateTwo();
	// flightStateThree();
	LED(12499,10);
	//buzzer(124,100);
	while (1){
		//printf("Hello, World!");
		//delay_ms(50);
		//lightChase(50);
		test();
		/*eeProm test*//*
		if(rtc_get_time() - t > 15){
			t = rtc_get_time();
			nvm_eeprom_write_byte(EP_address,t);
			EP_address++;

			if(EP_address >= 2047)
				EP_address = 0;
		}
		else
			printf("eeProm at %i reads %i",EP_address,nvm_eeprom_read_byte(EP_address));
		*/
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
	nvm_eeprom_write_byte(EP_address,0); //indicates the flight state.

	 while(alt < 15){
		if(rtc_get_time() - t > 300000 ){ //Every 5 minutes, save data to eeprom
			nvm_eeprom_write_byte(EP_address,alt);
			EP_address++;

			t = rtc_get_time();
			nvm_eeprom_write_byte(EP_address,(uint8_t)(t*1000)); //Convert time to seconds, then typecast to 8 bit for easier storage.
			EP_address++; 

			if(EP_address >= 2047) //Loops back around if we run out of addresses.
				EP_address = 0;
			}
		alt = (uint8_t)getAltitude();
		}
 }

 /* ASCENT */
 void flightStateOne(void){
	 LED(24999,10);//Set LED to 5Hz, 10% DC.
	 nvm_eeprom_write_byte(EP_address,1); //indicates the flight state.

	 while(getAltitude() < 600){
		 if(rtc_get_time() - t > 25000 ){ //Every 25 seconds, save data to eeprom.
			 nvm_eeprom_write_byte(EP_address,alt);
			 EP_address++;
			 
			 t = rtc_get_time();
			 nvm_eeprom_write_byte(EP_address,(uint8_t)(t*1000)); //Convert time to seconds, then typecast to 8bit for easier storage.
			 EP_address++;

			 if(EP_address >= 2047) //Loops back around if we run out of addresses.
				EP_address = 0;
		 }
		alt = (uint8_t)getAltitude();
	 }
 }

 /* DESCENT */
 void flightStateTwo(void){
	 LED(12499,10);//Set LED to 10Hz, 10% DC.
	 
	 delay_ms(5000);
	 PORTF.OUT = 0b00000001; //Turn balloon-line hotwire (1) on.
	 delay_ms(5000);
	 PORTF.OUT = 0b00000000; //Turn hotwire off.
	 nvm_eeprom_write_byte(EP_address,2); //indicates the flight state.

	 //
	 while(getAltitude() > 10){
	  /* Emergency Parachute*/
		if (getVelocity() < -70) //rough estimate for what velocity we want to deploy the parachute at
			deployParachute();

		 if(rtc_get_time() - t > 25000 ){ //Every 25 seconds, save data to eeprom.
			 nvm_eeprom_write_byte(EP_address,alt);
			 EP_address++;
			 
			 t = rtc_get_time();
			 nvm_eeprom_write_byte(EP_address,(uint8_t)(t*1000)); //See above sections for explanation.
			 EP_address++;

			 if(EP_address >= 2047) //Loops back around if we run out of addresses.
				EP_address = 0;
		 }
		alt = (uint8_t)getAltitude();
	 }
 }

 /* TOUCHDOWN */
 void flightStateThree(void){
	 LED(124999,10); //Set LED to 1Hz, 10% DC.
	 buzzer(0,0);//Buzzer @ Hz, DC **********0,0 placeholder, need real values. 
	 nvm_eeprom_write_byte(EP_address,3); //indicates the flight state.

	 for(int i = 0; i < 3; i++){
		 if(rtc_get_time() - t > 30000 ){ //Every 
			nvm_eeprom_write_byte(EP_address,alt);
			EP_address++;
			 
			t = rtc_get_time();
			nvm_eeprom_write_byte(EP_address,(uint8_t)(t * 1000));
			EP_address++;

			if(EP_address >= 2047) //Loops back around if we run out of addresses.
				EP_address = 0;
		 }
	}
	while(1);
 }

/* EMERGENCY PARACHUTE DEPLOY */
 void deployParachute(void){
	 PORTF.OUT = 0b00000010; //Turn parachute hotwire (2) on.
	 delay_ms(5000);
	 PORTF.OUT = 0b00000000; //Hotwire (2) off.
 }