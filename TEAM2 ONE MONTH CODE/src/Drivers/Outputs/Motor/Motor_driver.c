/*
 * Buzzer_Driver.c
 *
 * Class to hold buzzer methods, used to activate four motors.
 *
 *  Author: seanw
 */
 /* Includes */
 #include <asf.h>
 #include "Drivers/Outputs/Motor/Motor_driver.h"

 /* Global Variable Declarations */

  /*******************************************************************************************/
    					/* Simple Motor methods */

/* Motor method to start motor @ a certain period. */
void motor(uint16_t period){
	TCD0.CTRLA = 0b00000110; //prescalar 256
	PORTD.OUT |= 0b00000100; //out, high = 1, low = 0.

	TCD0.PER = period; //Set period of output.
	TCD0.CCC = TCD0.PER; //Set duty cycle of output for Command Capture register C.
}