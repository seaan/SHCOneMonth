/*
 * main.c
 *
 * Main class for Team 2 One Month project Sep-Oct 2016 |RPBees|.
 *
 * Author: seanw
 */

/* Includes */
#include <asf.h>
#include "Drivers/timerCounter/timer_counter_init.h"
#include "Drivers/ADC/ADC_init.h"
#include "test/testClass.h"
#include "Calculations/Altitude/getAltitude.h"
#include "Calculations/Velocity/getVelocity.h"
#include "conf_usart_serial.h"
#include "Drivers/Pressure/getPressure.h"
#include "Drivers/Outputs/Buzzer/Buzzer_Driver.h"
#include "Drivers/Outputs/LED/LED_driver.h"
#include "Drivers/Outputs/Motor/Motor_driver.h"

/* Defines */

 /* Global Variable Declarations */
uint8_t EP_address;
float alt;
float initAlt;
float t;
uint8_t deployedParachute;

/* Function Prototypes */
void flightStateOne(void);
void flightStateTwo(void);
void flightStateThree(void);
void flightStateZero(void);
void deployParachute(void);

/*******************************************************************************************/
									/* Main method */
int main (void)
{
	sysclk_init(); //Initialize the system clock, 32MHz, this also turns off all peripheral clocks.

	/* Peripheral clock initializations*/
	sysclk_enable_peripheral_clock(&USARTC0); //UART communications
	sysclk_enable_peripheral_clock(&ADCA); //Analog to Digital Converter
	sysclk_enable_peripheral_clock(&TCE0); //Timer Counter E
	sysclk_enable_peripheral_clock(&TCD0); //Timer Counter D
	sysclk_enable_peripheral_clock(&TCC0); //Timer Counter C
	sysclk_enable_peripheral_clock(&TCF0); //Timer Counter F
	sysclk_enable_peripheral_clock(&SPIC); //Serial Port Interface initialization.

	sysclk_enable_module(SYSCLK_PORT_E, SYSCLK_HIRES); //Must have this line for every timer counter due to a flaw in the design of the chip.
	sysclk_enable_module(SYSCLK_PORT_D, SYSCLK_HIRES);
	sysclk_enable_module(SYSCLK_PORT_F, SYSCLK_HIRES);
	sysclk_enable_module(SYSCLK_PORT_C,PR_SPI_bm);
	
	/* Input/Output Initializations */
	UART_Comms_Init();
	SPI_init();
	TCD0_init();
	TCF0_init();
	ADC_init();

	/* Variable Initializations */
	EP_address = 0;
	alt = 0;
	t = 0;
	initAlt = 0;
	deployedParachute = 0;

	/* Turn on Board lights */
	PORTE.DIR = 0b11111111;
	PORTE.OUT = 0b00000000;
	
	/* Delay for 5 seconds, then get our initial altitude */
	delay_s(5);
	initAlt = (float)getAltitude(getPressure(),getTemperature()); //typecast to float cause we don't want to mess stuff up.

	/* Code to read saved eeProm data */
	/*	
	for(int i = 0; i < 2047; i++){
		printf("eeprom at %i reads %i\n",EP_address,nvm_eeprom_read_byte(EP_address));
		EP_address++;
		delay_ms(250);
	}
*/	

	/* FLIGHT CODE */
	flightStateZero(); //Main will cycle through flight states, each flight state completes when target altitude is reached.
	flightStateOne();
	flightStateTwo();
	flightStateThree(); //Flight State 3 has an infinite loop.
	
}

/*******************************************************************************************/
						/* Flight State methods */

/* PRE-LAUNCH (FS0) */
 void flightStateZero(void){
	TCD0.CTRLA = 0b00000111; //prescalar 1024
	LED(62500,5);//Set LED to .5Hz, 5% DC.

	nvm_eeprom_write_byte(EP_address,0); //indicates the flight state.
	EP_address++;

	alt = (float)getAltitude(getPressure(),getTemperature()) - initAlt;; //find our altitude before we enter the while loop
	while(alt < 20){ //until we reach target altitude
		delay_ms(50); //delay 50ms so we don't trip over our own shoelaces.
		alt = (float)getAltitude(getPressure(),getTemperature()) - initAlt;; //find our altitude
	}
		nvm_eeprom_write_byte(EP_address,(uint8_t)(alt/3.28084)); //write the altitude (in meters) that caused us to exit flight state 0.
		EP_address++;
 }

 /* ASCENT (FS1) */
 void flightStateOne(void){
	TCD0.CTRLA = 0b00000111; //prescalar 1024
	LED(24999,10);//Set LED to 5Hz, 10% DC.

	nvm_eeprom_write_byte(EP_address,1); //indicates the flight state.

	alt = (float)getAltitude(getPressure(),getTemperature()) - initAlt;; //find our altitude before we enter the while loop
	while(alt < 600){
		nvm_eeprom_write_byte(EP_address,(uint8_t)(alt/3.28084)); //save our altitude (in meters)
		EP_address++;
		if(EP_address >= 2047) //Loops back around if we run out of addresses.
			EP_address = 0;

		delay_ms(250); //Find & save data every quarter sec.			
		alt = (float)getAltitude(getPressure(),getTemperature()) - initAlt;; //find our altitude
	 }
	 nvm_eeprom_write_byte(EP_address,(uint8_t)(alt/3.28084)); //save the altitude (in meters) that caused us to exit flight state 1.
	 EP_address++;
 }

 /* DESCENT (FS2) */
 void flightStateTwo(void){
	TCD0.CTRLA = 0b00000110; //prescalar 256
	LED(12499,10);//Set LED to 10Hz, 10% DC.
	motor(12499); //turn motor on

	delay_s(2); //wait a couple seconds
	PORTF.OUT = 0b00000010; //Turn balloon-line hotwire (1) on.
	delay_ms(1000); //hotwire is on 1 second
	PORTF.OUT = 0b00000000; //hotwire off.

	nvm_eeprom_write_byte(EP_address,2); //indicates the flight state.

	alt = (float)getAltitude(getPressure(),getTemperature()) - initAlt;; //find our altitude before we enter the while loop
	while(alt < 15){
		nvm_eeprom_write_byte(EP_address,(uint8_t)(alt/3.28084)); //save our altitude (in meters)
		EP_address++;
		if(EP_address >= 2047) //Loops back around if we run out of addresses.
			EP_address = 0;

		delay_ms(250); //Find & save data every quarter sec.
		if (getVelocity(getAltitude(getPressure(), getTemperature()), alt) < -24.384) //rough estimate for what velocity we want to deploy the parachute at
			deployParachute();
		alt = (float)getAltitude(getPressure(),getTemperature()) - initAlt;; //find our altitude
	}
	//turn motor off
	nvm_eeprom_write_byte(EP_address,(uint8_t)(alt/3.28084)); //save the altitude (in meters) that caused us to exit flight state 2.
	EP_address++;
 }

 /* TOUCHDOWN (FS3) */
 void flightStateThree(void){
	TCD0.CTRLA = 0b00000111; //prescalar 1024
	LED(31250,10); //Set LED to 1Hz, 10% DC.
	buzzer(124);//buzzer @ period of 124

	nvm_eeprom_write_byte(EP_address,3); //indicates the flight state.

	alt = (float)getAltitude(getPressure(),getTemperature()) - initAlt;; //find our altitude before we enter the for loop
	for(int i = 0; i < 3; i++){ //save data only three times, this flight state is kinda boring.
		nvm_eeprom_write_byte(EP_address,(uint8_t)(alt/3.28084));
		EP_address++;

		if(EP_address >= 2047) //Loops back around if we run out of addresses.
			EP_address = 0;
		delay_ms(300000); //Save data every 5 minutes.
		alt = (float)getAltitude(getPressure(),getTemperature()) - initAlt;; //find our altitude
	}
	while(1);
 }

/* EMERGENCY PARACHUTE DEPLOY */
 void deployParachute(void){
	 if(deployedParachute){
		 PORTF.OUT = 0b00000001; //Turn parachute hotwire (2) on.
		 delay_ms(2000);
		 PORTF.OUT = 0b00000000; //Hotwire (2) off.
		 deployedParachute = 0;
	 }
 }