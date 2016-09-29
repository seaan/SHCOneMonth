/*
 * Motor_driver.c
 *
 * Created: 9/25/2016 9:16:21 PM
 *  Author: seanw
 */ 
 #include <asf.h>
 #include "Drivers/Outputs/Motor/Motor_driver.h"

/* Motor method for four [part no.] motors */
void motor(uint32_t period,uint8_t duty_cycle){
	PORTD.OUT |= 0b00000001; //out, high = 1, low = 0.

	TCD0.PER = period; //Set period of output.
	TCD0.CCA = TCD0.PER - (TCD0.PER/duty_cycle); //Set duty cycle of output for Command Capture register A.
}