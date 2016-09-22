/*
 * flightState.h
 *
 * Created: 9/20/2016 11:06:45 PM
 *  Author: seanw
 */ 
#include <asf.h>
#ifndef FLIGHTSTATE_H_
#define FLIGHTSTATE_H_

void flightStateZero(void);  //Pre-Launch
void flightStateOne(void);   //Ascent
void flightStateTwo(void);   //Descent
void flightStateThree(void); //Touchdown

#endif /* FLIGHTSTATE_H_ */