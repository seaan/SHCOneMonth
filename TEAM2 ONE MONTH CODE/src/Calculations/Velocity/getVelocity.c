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
float getVelocity(float current_alt, float previous_alt){
	float perms = 3.1249523; //number of cycles of the timer counter per millisecond
	float arr_alt[25]; //Creates an array of size 25 for altitude.
	float arr_vel[25]; //Velocity array.
	float arr_time[25]; //time array'

	float final_alt = getAltitude(getPressure(),getTemperature()); //sets final altitude for the loop to the current altitude.
	for(int i = 0; i < 25; i++){ //For each element in altTable
		TCF0.CNT = 0;
		while (TCF0.CNT != TCF0.PER); //wait until TCF0 overflows, which will take 10ms

		arr_alt[i] = getAltitude(getPressure(),getTemperature()) - final_alt ; //Set the current element to the delta altitude found with final altitude of the previous iteration subtracted by the current altitude.

		arr_time[i] = TCF0.CNT/perms + 10; //time element array is one ahead so we can record the change in time + 10ms for the timer counter.

		final_alt = getAltitude(getPressure(),getTemperature()); //Sets the final altitude for the iteration to the current altitude.

	}

	/* Simple differentiation
	for(int z = 1; z < 25; z++){
		arr_vel[z] = arr_alt[z]/arr_time[z]; //Approximated velocity from delta alt / delta t.
		printf("arr_vel: %.2f\n",arr_vel[z]);
	}*/

	/* numerical differentiation. */
	for(int z = 1; z < 25; z++){
		arr_vel[z] = (arr_alt[z+1] - arr_alt[z-1])/(2*arr_time[z]); //Approximated velocity using a centered difference scheme, reduces noise from taking the derivative.
		printf("arr_vel: %.2f\n",arr_vel[z]);
	}

	arr_vel[0] = arr_alt[0]/arr_time[0]; //Still need to get the first velocity, time was missing ~10ms.

	//Now we know the velocity for 25 different samples over a total of 250ms. We now need to exponentially smooth the data to reduce noise again.

	float a_smooth[25]; //Smoothed velocity table of size 25.
	a_smooth[0] = arr_vel[0]; //Sets the initial smoothed value to the first velocity taken.
	for(int j = 1; j < 25; j++){ //For our 24 elements of velocity..
		a_smooth[j] = exponentialSmoothing(a_smooth[j - 1], arr_vel[j]); //For the current element of smoothed velocity, set it equal to the value that our exponential smoothing function gives us. We input the previous value for the smoothed table as our forecast/second variable, then the element of velocity at the current position. 
		//printf("a_smooth: %.2f\n",a_smooth[j]);
	}
	float s_vel = 0; //Summed velocity so we can average it.
	for(int b = 1; b < 25; b++) //For our size 25 smoothed velocity table, we want to ignore the first value because it is a duplicate, then iterate through every other value.
		s_vel += a_smooth[b]; //Set the sum of velocity equal to itself plus the element at our current position. Sums up all of the values in our table.
	return s_vel/24; //The function finally ends with returning the average, or the sum of our 24 elements of velocity divided by 24.
 }

 /* Exponential smoothing to reduce noise. */
 float exponentialSmoothing(float p_smth_vel, float current_vel){ //We need the previous smoothed value as well as the current un-smoothed value, as indicated by the equation on this page: https://en.wikipedia.org/wiki/Exponential_smoothing#Basic_exponential_smoothing
	float smooth_factor = .5; //50% smoothing factor, needs to be tested.
	//printf("current_vel: %.2f\np_smth_vel:%.2f\n",current_vel,p_smth_vel);
	return (current_vel * smooth_factor) + (p_smth_vel * (1 - smooth_factor)); //returns the smoothed velocity for the current position of our table! That wasn't so hard, was it?
 }