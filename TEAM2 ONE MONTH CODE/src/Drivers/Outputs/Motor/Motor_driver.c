/*
 * Motor_driver.c
 *
 * Created: 9/25/2016 9:16:21 PM
 *  Author: seanw
 */ 
 #include <asf.h>
 #include "Drivers/Outputs/Motor/Motor_driver.h"

/* Motor method for four [part no.] motors */
void motor(uint16_t period){
	TCD0.CTRLA = 0b00000110; //prescalar 256
	PORTD.OUT |= 0b00000100; //out, high = 1, low = 0.

	TCD0.PER = period; //Set period of output.
	TCD0.CCC = TCD0.PER; //Set duty cycle of output for Command Capture register A.
}