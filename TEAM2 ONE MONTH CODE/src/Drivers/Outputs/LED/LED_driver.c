/*
 * Buzzer_Driver.c
 *
 * Class to hold LED methods, used to activate two XPEBGR-L1-0000-00G01-SB01 LEDs.
 *
 *  Author: seanw
 */
 
 /* Includes */
  #include <asf.h>
  #include "Drivers/Outputs/LED/LED_driver.h"

  /* Global Variables */

   /*******************************************************************************************/
   						/* Simple LED methods */

/* LED method to start LED @ a certain period and duty cycle. */
  void LED(uint16_t period,uint8_t duty_cycle){
	  PORTD.OUT |= 0b00000010; //out, high = 1, low = 0.

	  TCD0.PER = period; //Set period of output.
	  TCD0.CCB = TCD0.PER - (TCD0.PER/duty_cycle); //Set duty cycle of output for Command Capture register A.
  }