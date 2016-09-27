/*
 * timer_counter_init.h
 *
 * Created: 8/18/2015 5:21:58 PM
 *  Author: abower
 */ 


#ifndef TIMER_COUNTER_INIT_H_
#define TIMER_COUNTER_INIT_H_

/* Place any #defines or function prototypes here */
void TCE0_init(uint16_t period,uint8_t duty_cycle);
void TCD0_init(void);

#endif /* TIMER_COUNTER_INIT_H_ */