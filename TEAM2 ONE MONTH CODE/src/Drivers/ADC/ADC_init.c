/*
 * timer_counter_init.c
 *
 * Class to hold initializations for various analog to digital converters used across the project.
 *
 *  Author: seanw
 */

 /* Includes */
#include <asf.h>
#include "Drivers/ADC/ADC_init.h"
/* Global Variable Declarations */

/*******************************************************************************************/
								/* Initialization methods */
/* Initialize ADC for port A, thermistor*/
void ADC_init(void){
	ADCA.CTRLA = 0b00000001; //Enables the ADC.
	ADCA.CTRLB = 0b00000000; //Unsigned 12 bit mode.
	ADCA.REFCTRL = 0b00010000; //Voltage reference of Vcc/1.6V
	ADCA.PRESCALER = 0b00000101; //This is automatic prescalar of 128 on the clock.
	ADCA.CAL = adc_get_calibration_data(ADC_CAL_ADCA); //Retrieve stored calibration data about the ADC.

	ADCA.CH0.CTRL = 0b00000001; //Single ended input
	ADCA.CH0.MUXCTRL = 0b00000000; //Reading ADCA pin 0.
}