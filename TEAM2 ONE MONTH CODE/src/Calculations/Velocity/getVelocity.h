/*
 * getVelocity.h
 *
 * Created: 9/20/2016 11:47:51 PM
 *  Author: seanw
 */ 
#include <asf.h>

#ifndef GETVELOCITY_H_
#define GETVELOCITY_H_

float getVelocity(void); //Velocity function
float exponentialSmoothing(float previousVel, float velocity);



#endif /* GETVELOCITY_H_ */