/*
 * altitudeMeasurement.c
 *
 * Created: 9/20/2016 11:17:10 PM
 *  Author: seanw
 */ 
 #include <asf.h>
 #include <math.h>
 #include "Calculations/Altitude/getAltitude.h"
 #include "Drivers/Pressure/getPressure.h"
 #include "Drivers/Temperature/getTemperature.h"
 float getAltitude(void){

	float temperature = getTemperature();
	float pressure = (float)getPressure();
	float R = 287;
	float g = 9.80665;
	float pconst = 101325;

	float alt1 = (R*temperature)/g;
	
	float alt2 = log(pconst/pressure);

	//printf("pressure: %f\ntemperature: %f\nalt1:%f\nalt2:%f\n",pressure,temperature,alt1,alt2);

	return ((R * temperature)/g)*log(pconst/pressure);  //Equation found here: https://en.wikipedia.org/wiki/Hypsometric_equation 
 }