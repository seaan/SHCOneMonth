/*
 * timer_counter_init.c
 *
 * Created: 8/18/2015 5:19:23 PM
 *  Author: swidmier
 */ 

#include <asf.h>
#include "Drivers/timer_counter_init.h"

/* This driver can be used to initialize any of the timer counters. Note, only the instructions on 
   how to initialize the TCs is given, you must write the actual code */

void TCE0_init(uint16_t period,uint8_t dutyCycle){
	PORTE.DIR = 0b11111111;
	TCE0.CTRLA = 0b00000110; //0110 is prescalar 256.
	TCE0.CTRLB = 0b11110011; //All LED outputs and Single Slope.
	TCE0.PER = period; //THis will make the LEDs blink at 10Hz.
	TCE0.CCA = TCE0.PER - (TCE0.PER/dutyCycle); //90% Duty cycle equates to 10% on time for LEDs.
	TCE0.CCB = TCE0.PER - (TCE0.PER/dutyCycle);
	TCE0.CCC = TCE0.PER - (TCE0.PER/dutyCycle);
	TCE0.CCD = TCE0.PER - (TCE0.PER/dutyCycle);
}
