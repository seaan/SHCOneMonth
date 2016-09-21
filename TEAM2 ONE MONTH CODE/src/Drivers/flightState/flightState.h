/*
 * flightState.h
 *
 * Created: 9/20/2016 11:06:45 PM
 *  Author: seanw
 */ 
#include <asf.h>
#ifndef FLIGHTSTATE_H_
#define FLIGHTSTATE_H_

int flightStateZero(void);  //Pre-Launch
int flightStateOne(void);   //Ascent
int flightStateTwo(void);   //Descent
int flightStateThree(void); //Touchdown

#endif /* FLIGHTSTATE_H_ */