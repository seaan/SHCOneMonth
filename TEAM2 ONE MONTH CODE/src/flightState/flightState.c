/*
 * flightState.c
 *
 * Created: 9/20/2016 11:06:37 PM
 *  Author: seanw
 */ 
 #include <asf.h>
 #include "flightState/flightState.h"
 #include "Calculations/Altitude/getAltitude.h"
 #include "Calculations/Velocity/getVelocity.h"


void deployParachute(void);

 //Pre-Launch
 void flightStateZero(void){
	//save data to eeprom.
	//Set LED to .5Hz, 5% DC.
	if(getAltitude() < 15) //0,0 is placeholder
		flightStateZero();
 }

 //Ascent
 void flightStateOne(void){
	//save data to eeprom.
	//Set LED to 5Hz, 10% DC.
	if(getAltitude() < 600) //0,0 is placeholder
		flightStateOne();
 }

 //Descent
 void flightStateTwo(void){
	//save data to eeprom.
	//Set LED to 10Hz, 10% DC.
	//velocity calculation
	if (getVelocity() < -70) //rough estimate for what velocity we want to deploy the parachute at
		deployParachute();
	if(getAltitude() > 10) //0,0 is placeholder
		flightStateTwo();
 }

 //Touchdown
 void flightStateThree(void){
	//save data to eeprom.
	//Set LED to 1Hz, 10% DC.
	//Buzzer
	flightStateThree();
 }

 void deployParachute(){
	

 }