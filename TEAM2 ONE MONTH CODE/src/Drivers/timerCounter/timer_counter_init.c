/*
 * timer_counter_init.c
 *
 * Created: 8/18/2015 5:19:23 PM
 *  Author: swidmier
 */ 

#include <asf.h>
#include "Drivers/timerCounter/timer_counter_init.h"

/* This driver can be used to initialize any of the timer counters. Note, only the instructions on 
   how to initialize the TCs is given, you must write the actual code */

void TCE0_init(uint16_t period,uint8_t duty_cycle){
	PORTE.DIR = 0b11111111;
	PORTE.OUT = 0b00000000; //Sets all of the pins voltage levels to 0V, which is logic 0 in programming.
	TCE0.CTRLA = 0b00000110; //0110 is prescalar 256.
	TCE0.CTRLB = 0b11110011; //All LED outputs and Single Slope.
	TCE0.PER = period; //THis will make the LEDs blink at 10Hz.
	TCE0.CCA = TCE0.PER - (TCE0.PER/duty_cycle); //90% Duty cycle equates to 10% on time for LEDs.
	TCE0.CCB = TCE0.PER - (TCE0.PER/duty_cycle);
	TCE0.CCC = TCE0.PER - (TCE0.PER/duty_cycle);
	TCE0.CCD = TCE0.PER - (TCE0.PER/duty_cycle);
}
void TCD0_init(void){
	PORTD.DIR = 0b11111111; //sets to output.
	TCD0.CTRLA = 0b00000110; //0110 is prescalar 256.
	TCD0.CTRLB = 0b10110011; //OCn output pin (currently set at 0) and single slope PWM.
}

void TCF0_init(void){
	TCF0.CTRLA = 0b00000111; //presc 1024
	TCF0.CTRLB = 0b00000000;
	TCF0.CNT = 0;
	TCF0.PER = 312; //period of 312 = ~100Hz = ~10ms before overflow

	//TCF0.CTRLB = 0b00000000; //Command capture @ CCDEN
	//while ((TCF0.INTFLAGS<<7)!=0b10000000); //wait until TCF0 overflows
}
