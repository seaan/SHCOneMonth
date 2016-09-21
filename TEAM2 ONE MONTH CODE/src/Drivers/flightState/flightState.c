/*
 * flightState.c
 *
 * Created: 9/20/2016 11:06:37 PM
 *  Author: seanw
 */ 
 #include "Drivers/flightState/flightState.h"

 //Pre-Launch
 int flightStateZero(void){
	//Set LED to .5Hz, 5% DC.
	while (altitude < 10){
		altitude = getAltitude(); //Records current altitude to initial altitude for the loop.
	}
	//save data to eeprom.
	return 1;
 }

 //Ascent
 int flightStateOne(void){
	//Set LED to 5Hz, 10% DC.
	while (altitude < 600)){
		altitude = getAltitude(); //Records current altitude.

	}
	//save data to eeprom.
	return 2;
 }

 //Descent
 int flightStateTwo(void){
	 uint8_t initAlt; //Altitude to be recorded at the beginning of every loop.
	 uint8_t initTime; //Time to be recorded at beginning of every loop.
	 int8_t deltaAlt; //Change in altitude
	 uint8_t deltaTime = 0.005; //Change in time in seconds.
	 int8_t velocity;

	//Set LED to 10Hz, 10% DC.
	while (initAlt > 10){
		altitude = getAltitude(); //Records current altitude.
		deltaAlt = altitude - initAlt; //Sets change in altitude to the current altitude subtracted by the altitude 50ms ago.
		velocity = getVelocity(deltaAlt,deltaTime);

		initAlt = getAltitude(); //Records current altitude to compare to flight state altitude, as well as act as initial altitude in velocity computation.
		delay_ms(50);
	}
	//save data to eeprom.
	return 3;
 }

 //Touchdown
 int flightStateThree(void){
	//Set LED to 1Hz, 10% DC.
	while (1){
		altitude = getAltitude(); //Records current altitude.

	}
	//save data to eeprom.
	return 0;
 }