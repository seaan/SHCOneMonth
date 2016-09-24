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
	return (getTemperature()/-0.0065)*(powf(getPressure()/101325,0.190263236508483566887195756610229817256134454173664313568)-1);  //Equation found here: http://www.mide.com/pages/air-pressure-at-altitude-calculator, PRESSURE IN PASCALS, TEMP IN KELVIN.
 }