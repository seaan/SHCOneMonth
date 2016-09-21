/*
 * flightState.c
 *
 * Created: 9/20/2016 11:06:37 PM
 *  Author: seanw
 */ 
 #include <asf.h>
 #include "Drivers/flightState/flightState.h"
 #include "Drivers/Altitude/getAltitude.h"
 #include "Drivers/Velocity/getVelocity.h"
 #include "Drivers/descentControl/descentControl.h"

uint8_t altitude;

 //Pre-Launch
 int flightStateZero(void){
	//Set LED to .5Hz, 5% DC.
	while (altitude < 10){
		altitude = getAltitude(0,0); //Records current altitude to initial altitude for the loop. 0,0 is placeholder.
	}
	//save data to eeprom.
	return 1;
 }

 //Ascent
 int flightStateOne(void){
	//Set LED to 5Hz, 10% DC.
	while (altitude < 600){
		altitude = getAltitude(0,0); //Records current altitude. 0,0 is placeholder.

	}
	//save data to eeprom.
	return 2;
 }

 //Descent
 int flightStateTwo(void){
	 uint8_t initAlt = 0; //Altitude to be used as inital in velocity equation, as well as for controlling flight state.
	 int8_t deltaAlt; //Change in altitude
	 uint8_t deltaTime = 0.005; //Change in time in seconds.
	 int8_t velocity;

	//Set LED to 10Hz, 10% DC.
	while (initAlt > 10){
		altitude = getAltitude(0,0); //Records current altitude. 0,0 is placeholder.
		deltaAlt = altitude - initAlt; //Sets change in altitude to the current altitude subtracted by the altitude 50ms ago.
		
		velocity = getVelocity(deltaAlt,deltaTime);
		
		descentControl(velocity);
		
		initAlt = getAltitude(0,0); //Records current altitude to compare to flight state altitude, as well as act as initial altitude in velocity computation. 0,0 is placeholder.
		delay_ms(50);
	}
	//save data to eeprom.
	return 3;
 }

 //Touchdown
 int flightStateThree(void){
	//Set LED to 1Hz, 10% DC.
	while (1){
		altitude = getAltitude(0,0); //Records current altitude. 0,0 is placeholder.

	}
	//save data to eeprom.
	return 0;
 }