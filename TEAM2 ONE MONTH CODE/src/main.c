/* ONE MONTH TEAM 2 MAIN */

/* Begin #include section */
//This is where you will need to include the header files that you have written code in to use the code
#include <asf.h> //This will mainly include behind the scene code and all header files within the config folder
#include "Drivers/timer_counter_init.h"

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
uint8_t lightChase;
/* End Global Variable Section */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Begin Function Prototype Section */
//This is where you make function prototypes

void Example(void); //function called Example with no return or argument input, define below main
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
	sysclk_enable_peripheral_clock(&USARTC0); //For every peripheral, you must enable the clock like shown here. Ex. Timer counters, SPI, ADCs
	
	/* Example, Timer Counter on PORTE */
	sysclk_enable_peripheral_clock(&TCE0);
	sysclk_enable_module(SYSCLK_PORT_E, SYSCLK_HIRES); //You must have this line for every timer counter due to a flaw in the design of the chip
	
	/* Initializations */;
	UART_Comms_Init();
	TCE0_init(12499,10);
	/* Flight Code */
	
	sysclk_enable_peripheral_clock(&ADCA);
	
	PORTE.DIR = 0b11111111; //Sets all the pins on PORTE as an output.
	PORTE.OUT = 0b00000000; //Sets all of the pins voltage levels to 0V, which is logic 0 in programming.
	
	while (1){
		/*
		lightChase = 0b00000001;
		for(int i = 0; i < 4; i++){
			PORTE.OUT = ~lightChase;
			lightChase >>= 1;
			delay_ms(100);
		}
		lightChase = 0b1000000;
		for(int i = 0; i < 4; i++){
			PORTE.OUT = ~lightChase;
			lightChase <<= 1;
			delay_ms(100);
		}
		*/
		
	}
	
}