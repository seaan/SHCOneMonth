/*
 * LED_driver.c
 *
 * Created: 9/25/2016 9:15:38 PM
 *  Author: seanw
 */ 
  /* LED method for two XPEBGR-L1-0000-00G01-SB01 LEDs */
  #include <asf.h>
  #include "Drivers/Outputs/LED/LED_driver.h"
  void LED(uint32_t period,uint8_t duty_cycle){
	  PORTD.OUT |= 0b00000010; //out, high = 1, low = 0.

	  TCD0.PER = period; //Set period of output.
	  TCD0.CCB = TCD0.PER - (TCD0.PER/duty_cycle); //Set duty cycle of output for Command Capture register A.
  }