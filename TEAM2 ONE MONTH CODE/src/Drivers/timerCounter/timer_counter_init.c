/*
 * timer_counter_init.c
 *
 * Class to hold initializations for various timer counters used across the project.
 *
 *  Author: seanw
 */

 /* Includes */
#include <asf.h>
#include "Drivers/timerCounter/timer_counter_init.h"

/* Global Variable Declaration */

  /*******************************************************************************************/
								 /* Initialization methods */

/* Initialize Timer Counter for port E, Board LEDs */
void TCE0_init(uint16_t period,uint8_t duty_cycle){
	PORTE.OUT = 0b00000000; //Sets all of the pins voltage levels to 0V, which is logic 0 in programming.
	TCE0.CTRLA = 0b00000110; //0110 is prescalar 256.
	TCE0.CTRLB = 0b11110011; //All LED outputs and Single Slope.
	TCE0.PER = period; //THis will make the LEDs blink at 10Hz.
	TCE0.CCA = TCE0.PER - (TCE0.PER/duty_cycle); //90% Duty cycle equates to 10% on time for LEDs.
	TCE0.CCB = TCE0.PER - (TCE0.PER/duty_cycle);
	TCE0.CCC = TCE0.PER - (TCE0.PER/duty_cycle);
	TCE0.CCD = TCE0.PER - (TCE0.PER/duty_cycle);
}

/* Initialize Timer Counter for port D, GPIOs (LEDs, Buzzer, Motor) */
void TCD0_init(void){
	PORTD.DIR = 0b11111111;
	PORTD.DIR = 0b11111111; //sets to output.
	TCD0.CTRLA = 0b00000110; //0110 is prescalar 256.
	TCD0.CTRLB = 0b10110011; //OCn output pin (currently set at 0) and single slope PWM.
}

/* Initialize Timer Counter for port F, keeps time for velocity calculations. */
void TCF0_init(void){
	PORTF.DIR = 0b00000011;
	TCF0.CTRLA = 0b00000111; //presc 1024
	TCF0.CTRLB = 0b00000000;
	TCF0.CNT = 0;
	TCF0.PER = 312; //period of 312 = ~100Hz = ~10ms before overflow
}
