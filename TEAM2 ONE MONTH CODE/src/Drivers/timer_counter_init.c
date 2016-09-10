/*
 * timer_counter_init.c
 *
 * Created: 8/18/2015 5:19:23 PM
 *  Author: abower
 */ 

#include <asf.h>
#include "Drivers/timer_counter_init.h"

/* This driver can be used to initialize any of the timer counters. Note, only the instructions on 
   how to initialize the TCs is given, you must write the actual code */

/* Steps to initialize a timer counter and for PWM
	1) If using PWM, make sure the pins for PWM are set as outputs on the register level
	2) Setup control register A, CTRLA
	3) Setup control register B, CTRLB
	4) Set the timer counter period. Note, max value is 16 bit or 65535.
	5) Set the CCx for each pin being used as PWM. CCx/PER = Duty Cycle %
*/
