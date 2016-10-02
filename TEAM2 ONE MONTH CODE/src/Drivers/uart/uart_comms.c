/*
 * uart_comms.c
 *
 * Class to hold functions that can be called during initialization of the MCU to start the uart communications
 *
 *  Author: abower
 */
 /* Includes */
#include "conf_usart_serial.h" //Includes the information in the config file
#include <asf.h>

/* Global Variables */

 /*******************************************************************************************/
								/* Initialization methods */
/* Method to initialize UART communications */
void UART_Comms_Init(void){
	//Struct that contains the settings from config file
	static usart_serial_options_t usart_options = {
		.baudrate = USART_SERIAL_BAUDRATE,
		.charlength = USART_SERIAL_CHAR_LENGTH,
		.paritytype = USART_SERIAL_PARITY,
		.stopbits = USART_SERIAL_STOP_BIT
	};

	//Must set TX pin as output
	PORTC.DIRSET=0b00001000;
	PORTC.OUTSET=0b00001000;
	PORTC.OUT=0xFF;

	stdio_serial_init(&USARTC0, &usart_options); //ASF function that initializes the UART peripheral
}
