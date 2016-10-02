/*
 * getVelocity.h
 *
 * Created: 9/20/2016 11:47:51 PM
 *  Author: seanw
 */ 
#include <asf.h>

#ifndef GETVELOCITY_H_
#define GETVELOCITY_H_

float getVelocity(float current_alt, float previous_alt); //Velocity function
float exponentialSmoothing(float p_smth_vel, float current_vel);



#endif /* GETVELOCITY_H_ */