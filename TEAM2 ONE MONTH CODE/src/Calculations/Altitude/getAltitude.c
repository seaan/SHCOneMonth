/*
 * getAltitude.c
 *
 * Class to hold methods needed for Altitude calculations.
 *
 * Created: 9/22/2016 11:45:21 AM
 *  Author: seanw
 */

 /* Includes */
 #include <asf.h>
 #include <math.h>
 #include "Calculations/Altitude/getAltitude.h"

 /* Global Variable Declarations */

 /*******************************************************************************************/
								 /* getAltitude method */
/* Method that uses temperature and pressure w/ Hypsometric Equation. */
 float getAltitude(float temperature, uint32_t inputpressure){
	float pressure = (float)inputpressure;
	float R = 287;
	float g = 9.80665;
	float pconst = 101325;
	//printf("Temperature: %.2f\nPressure: %.2f\n",temperature,pressure);
	return (((R * temperature)/g)*log(pconst/pressure)) * 3.28084; //return altitude in feet
 }