/*
 * altitudeMeasurement.c
 *
 * Created: 9/20/2016 11:17:10 PM
 *  Author: seanw
 */ 
 #include <asf.h>
 #include "Calculations/Altitude/getAltitude.h"
 #include "Drivers/Pressure/getPressure.h"
 #include "Drivers/Temperature/getTemperature.h"
 float getAltitude(void){
	return (getTemperature() - 123)/(-.00649);
 }