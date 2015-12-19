#ifndef _INCLUDE_TIMER_H
#define _INCLUDE_TIMER_H

#define MILLISECONDS_PER_TICK 10
#define freq 100

void init_timer(void);
void timer_handler(unsigned hz);
void delay(unsigned milliseconds);

#endif
