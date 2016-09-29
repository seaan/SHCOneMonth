/*
 * Buzzer_driver.c
 *
 * Created: 9/25/2016 9:15:45 PM
 *  Author: seanw
 */ 
 #include <asf.h>
 #include "Drivers/Outputs/Buzzer/Buzzer_Driver.h"

 /* Buzzer method for 668-1468-ND Buzzer */
 void buzzer(uint32_t period,uint8_t duty_cycle){
	 PORTD.OUT |= 0b00001000; //out, high = 1, low = 0.

	 TCD0.PER = period; //Set period of output.
	 TCD0.CCD = TCD0.PER - (TCD0.PER/duty_cycle); //Set duty cycle of output for Command Capture register A.
 }