/*
 * getVelocity.c
 *
 * Created: 9/20/2016 11:47:58 PM
 *  Author: seanw
 */ 
 #include <asf.h>
 #include "Drivers/Velocity/getVelocity.h"
 #include "Drivers/Altitude/getAltitude.h"

 float exponentialSmoothing(float previousVel, float velocity);

 //Velocity function
 float getVelocity(void){
	float velocityTable[25]; //Creates an array of size 25
	float velocity;
	float finalAlt = getAltitude(0,0); //0,0 is placeholder, sets final altitude for the loop to the current altitude. We will throw out the first element of the array, as it will equal near zero.
	for(int i = 0; i < 25; i++){ //For each element
		velocity = (finalAlt - getAltitude(0,0))/.01; //0,0 is placeholder, sets the delta altitude for this iteration to the final altitude of the previous iteration subtracted by the current altitude.
		velocityTable[i] = velocity; //Set the current element to the delta altitude.
		delay_ms(10); //Delay for 10ms, creates a sample rate for velocity of 100Hz.
		finalAlt = getAltitude(0,0); //0,0 is placeholder, sets the final altitude for the iteration to the current altitude.
	}
	//Now we know the velocity for 24 different samples over a total of 240ms (we threw out the initial calculation) . We now need to exponentially smooth the data.

	float smoothedVelTable[25];
	smoothedVelTable[0] = velocityTable[1]; //Sets the initial smoothed value to the second velocity taken, as the first needs to be thrown out.
	for(int j = 1; j < 25; j++){ //For our 24 elements of velocity..
		smoothedVelTable[j] = exponentialSmoothing(smoothedVelTable[j - 1], velocityTable[j]); //For the current element of smoothed velocity, set it equal to the value that our exponential smoothing function gives us. We input the previous value for the smoothed table as our forecast/second variable, then the element of velocity at the current position. 
	}
	float sumVelocity = 0; //Summed velocity so we can average it.
	for(int b = 1; b < 25; b++) //For our size 25 smoothed velocity table, we want to ignore the first value because it is a duplicate, then iterate through every other value.
		sumVelocity += smoothedVelTable[b]; //Set the sum of velocity equal to itself plus the element at our current position. Sums up all of the values in our table.
	return sumVelocity/24; //The function finally ends with returning the average, or the sum of our 24 elements of velocity divided by 24.
 }

 float exponentialSmoothing(float previousVel, float velocity){ //We need the previous smoothed value as well as the current unsmoothed value, as indicated by the equation on this page: https://en.wikipedia.org/wiki/Exponential_smoothing#Basic_exponential_smoothing
	float smoothingFactor = .2; //20% smoothing factor, needs to be tested.
	return (velocity * smoothingFactor) + (previousVel * (1 - smoothingFactor)); //returns the smoothed velocity for the current position of our table! That wasn't so hard, was it?
 }