/*
 * getAltitude.c
 *
 * Class to hold methods needed for Velocity calculations.
 *
 * Created: 9/22/2016 11:45:21 AM
 *  Author: seanw
 */

 /* Includes */
 #include <asf.h>
 #include "Calculations/Velocity/getVelocity.h"
 #include "Calculations/Altitude/getAltitude.h"
 #include "Drivers/Pressure/getPressure.h"
 #include "Drivers/Temperature/getTemperature.h"

 /* Global Variable Declarations */

/*******************************************************************************************/
								/* Velocity Calculation methods */
/* Find velocity using change in altitude and time, utilizing numerical differentiation and exponential smooth to reduce noise.*/
float getVelocity(void){
	float perms = 3.1249523; //number of cycles of the timer counter per millisecond
	float arr_alt[25]; //Creates an array of size 25 for altitude.
	float arr_time[25]; //time array

	arr_alt[0] = getAltitude(getPressure(),getTemperature()); //our first altitude recording.
	for(int i = 1; i < 25; i++){ //For each element in altTable
		TCF0.CNT = 0;
		while (TCF0.CNT != TCF0.PER); //wait until TCF0 overflows, which will take 10ms

		arr_alt[i] = getAltitude(getPressure(),getTemperature()); //Set the current element to the current altitude.

		arr_time[i] = TCF0.CNT/perms + 10; //time element array is one ahead so we can record the change in time + 10ms for the timer counter.
	}


	/* Altitude Data Smoothing */
	float arr_smoothAlt[25];
	arr_smoothAlt[0] = arr_alt[0];
	for(int i = 1; i < 25; i++){ //For our 25 elements of altitude..
		arr_smoothAlt[i] = exponentialSmoothing(arr_smoothAlt[i - 1], arr_alt[i]); //For the current element of smoothed altitude, set it equal to the value that our exponential smoothing function gives us. We input the previous value for the smoothed table as our forecast/second variable, then the element of alt at the current position.
		//printf("arr_smoothAlt: %.2f\n",arr_smoothAlt[i]);
	}

	/* Numerical differentiation. */
	float arr_vel[23]; //Velocity array. Has to be two smaller than altitude table cause we need to find the change in altitude, and the first and second data points don't have anything to compare to before or beyond them.
	for(int i = 1; i < 24; i++){ //for each value in our velocity array
		arr_vel[i-1] = (arr_smoothAlt[i+1] - arr_smoothAlt[i-1])/(2*arr_time[i]); //Approximated velocity using a centered difference scheme, reduces noise from taking the derivative.
		printf("arr_vel: %.2f\n",arr_vel[i-1]);
	}

	/* Velocity Data Smoothing */
	float arr_smoothVel[23]; //Smoothed velocity table of size 23, same as velocity table.
	arr_smoothVel[0] = arr_vel[0]; //Sets the initial smoothed value to the first velocity taken.
	for(int i = 1; i < 24; i++){ //For our 23 elements of smoothed velocity..
		arr_smoothVel[i] = exponentialSmoothing(arr_smoothVel[i - 1], arr_vel[i]); //For the current element of smoothed velocity, set it equal to the value that our exponential smoothing function gives us. We input the previous value for the smoothed table as our forecast/second variable, then the element of velocity at the current position. 
		//printf("arr_smoothVel: %.2f\n",arr_smoothVel[i]);
	}
	return arr_smoothVel[22]; //The most smoothed, most recent value.
}
 /* Exponential smoothing to reduce noise. */
 float exponentialSmoothing(float previous_value, float current_value){ //We need the previous smoothed value as well as the current un-smoothed value, as indicated by the equation on this page: https://en.wikipedia.org/wiki/Exponential_smoothing#Basic_exponential_smoothing
	float smooth_factor = .5; //50% smoothing factor, needs to be tested.
	//printf("current_value: %.2f\np_smth_vel:%.2f\n",current_value,previous_value);
	return (current_value * smooth_factor) + (previous_value * (1 - smooth_factor)); //returns the smoothed velocity for the current position of our table! That wasn't so hard, was it?
 }