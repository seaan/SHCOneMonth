/*
 * getVelocity.c
 *
 * Created: 9/20/2016 11:47:58 PM
 *  Author: seanw
 */ 
 #include <asf.h>
 #include "Calculations/Velocity/getVelocity.h"
 #include "Calculations/Altitude/getAltitude.h"

 float exponentialSmoothing(float previousVel, float velocity);

 //Velocity function
 float getVelocity(void){
	float altTable[25]; //Creates an array of size 25
	float velocityTable[25];
	float deltaAlt;
	float finalAlt = getAltitude(); //sets final altitude for the loop to the current altitude.
	for(int i = 0; i < 25; i++){ //For each element in altTable
		delay_ms(10); //Delay for 10ms, creates a sample rate for velocity of 100Hz. 
		altTable[i] = finalAlt - getAltitude(); //Set the current element to the delta altitude found with final altitude of the previous iteration subtracted by the current altitude.
		finalAlt = getAltitude(); //Sets the final altitude for the iteration to the current altitude.
	}
	//Attempt at numerical differentiation.
	for(int z = 1; z < 25; z++){
		velocityTable[z] = (altTable(z+1) - altTable(z-1))/.2 //Approximated velocity using a centered difference scheme, reduces noise from taking the derivative.
	}
	
	//Now we know the velocity for 25 different samples over a total of 250ms. We now need to exponentially smooth the data to reduce noise again.

	float smoothedVelTable[25];
	smoothedVelTable[0] = velocityTable[0]; //Sets the initial smoothed value to the first velocity taken.
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