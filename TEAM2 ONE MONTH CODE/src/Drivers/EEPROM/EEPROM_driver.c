/*
 * timer_counter_init.c
 *
 * Class to hold EEPROM methods.
 *
 *  Author: seanw
 */
  /* Includes */
  #include <asf.h>
  #include "Drivers/EEPROM/EEPROM_driver.h"
  /* Global Variable Declarations */
  uint16_t EP_address = 0;
  /*******************************************************************************************/
								/* Write and Read methods */
  /* EEPROM write method */
  void writeEEPROM(uint8_t data){
	nvm_eeprom_write_byte(EP_address, data);
	EP_address++;
	if(EP_address >= 2047) //Loops back around if we run out of addresses.
		EP_address = 0;
  }

  void readfullEEPROM(void){
	printf("Reading EEPROM....\n");
	delay_s(1);

	for(EP_address = 0; EP_address <= 2047; EP_address++){
		printf("EEPROM at %i reads %i\n",EP_address,nvm_eeprom_read_byte(EP_address));
		delay_ms(50);
	}
  }