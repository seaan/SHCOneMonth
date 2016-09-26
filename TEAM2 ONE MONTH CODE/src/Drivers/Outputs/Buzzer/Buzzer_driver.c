/*
 * Buzzer_driver.c
 *
 * Created: 9/25/2016 9:15:45 PM
 *  Author: seanw
 */ 

 /* Buzzer method for 668-1468-ND Buzzer */
 void buzzer(uint16_t period,uint8_t duty_cycle){
	 PORTE.DIR = 0b11111111; //MODIFY PORT PLS, sets to output.

	 TCE0.CTRLA = 0b00000110; //0110 is prescalar 256.
	 TCE0.CTRLB = 0b10000011; //OCn output pin (currently set at 1) and single slope PWM.

	 TCE0.PER = period; //Set period of output.

	 TCE0.CCA = TCE0.PER - (TCE0.PER/duty_cycle); //Set duty cycle of output for Command Capture register A.
	 TCE0.CCB = TCE0.PER - (TCE0.PER/duty_cycle);
	 TCE0.CCC = TCE0.PER - (TCE0.PER/duty_cycle);
	 TCE0.CCD = TCE0.PER - (TCE0.PER/duty_cycle);
 }