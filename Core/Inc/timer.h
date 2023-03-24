/*
 * timer.h
 *
 *  Created on: Mar 20, 2023
 *      Author: tan
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

extern int timer1_flag;
extern int timer2_flag;
extern int timer3_flag;
extern int timer4_flag;
extern int timer5_flag;

void setTimer1(int duration);
void setTimer2(int duration);
void setTimer3(int duration);
void setTimer4(int duration);
void setTimer5(int duration);
int isTimerFlagRaised(int n);
void initTimer();
void timerRun();

#endif /* INC_TIMER_H_ */
