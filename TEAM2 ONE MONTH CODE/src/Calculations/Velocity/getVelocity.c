/*
 * getVelocity.c
 *
 * Created: 9/20/2016 11:47:58 PM
 *  Author: seanw
 */ 
 #include <asf.h>
 #include "Calculations/Velocity/getVelocity.h"
 #include "Calculations/Altitude/getAltitude.h"

 float exponentialSmoothing(float p_smth_vel, float current_vel);

 //Velocity function
 float getVelocity(void){
	float arr_alt[25]; //Creates an array of size 25 for altitude.
	float arr_vel[25]; //Velocity array.
	float arr_time[25]; //Time array
	float final_alt = getAltitude(); //sets final altitude for the loop to the current altitude.
	uint32_t t = 0;
	for(int i = 0; i < 25; i++){ //For each element in altTable
		delay_ms(10); //Delay for 10ms, creates a sample rate for velocity of 100Hz. 
		arr_alt[i] = final_alt - getAltitude(); //Set the current element to the delta altitude found with final altitude of the previous iteration subtracted by the current altitude.
		arr_time[i] = (rtc_get_time() - t)*1000;
		final_alt = getAltitude(); //Sets the final altitude for the iteration to the current altitude.
		t = rtc_get_time();
	}

	//Attempt at numerical differentiation.
	for(int z = 1; z < 25; z++){
		arr_vel[z] = (arr_alt[z+1] - arr_alt[z-1])/arr_time[z]; //Approximated velocity using a centered difference scheme, reduces noise from taking the derivative.
	}

	arr_vel[0] = arr_alt[0]/.1; //Still need to get the first velocity.

	//Now we know the velocity for 25 different samples over a total of 250ms. We now need to exponentially smooth the data to reduce noise again.

	float a_smooth[25]; //Smoothed velocity table of size 25.
	a_smooth[0] = arr_vel[0]; //Sets the initial smoothed value to the first velocity taken.
	for(int j = 1; j < 25; j++){ //For our 24 elements of velocity..
		a_smooth[j] = exponentialSmoothing(a_smooth[j - 1], arr_vel[j]); //For the current element of smoothed velocity, set it equal to the value that our exponential smoothing function gives us. We input the previous value for the smoothed table as our forecast/second variable, then the element of velocity at the current position. 
	}
	float s_vel = 0; //Summed velocity so we can average it.
	for(int b = 1; b < 25; b++) //For our size 25 smoothed velocity table, we want to ignore the first value because it is a duplicate, then iterate through every other value.
		s_vel += a_smooth[b]; //Set the sum of velocity equal to itself plus the element at our current position. Sums up all of the values in our table.
	return s_vel/24; //The function finally ends with returning the average, or the sum of our 24 elements of velocity divided by 24.
 }

 float exponentialSmoothing(float p_smth_vel, float current_vel){ //We need the previous smoothed value as well as the current un-smoothed value, as indicated by the equation on this page: https://en.wikipedia.org/wiki/Exponential_smoothing#Basic_exponential_smoothing
	float smooth_factor = .2; //20% smoothing factor, needs to be tested.
	return (current_vel * smooth_factor) + (p_smth_vel * (1 - smooth_factor)); //returns the smoothed velocity for the current position of our table! That wasn't so hard, was it?
 }