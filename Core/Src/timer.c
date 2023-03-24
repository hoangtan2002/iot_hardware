/*
 * timer.c
 *
 *  Created on: Mar 20, 2023
 *      Author: tan
 */

#include"timer.h"

int timer1_counter = 0;
int timer1_flag = 0;
int timer2_counter = 0;
int timer2_flag = 0;
int timer3_counter = 0;
int timer3_flag = 0;
int timer4_counter = 0;
int timer4_flag = 0;
int timer5_counter = 0;
int timer5_flag = 0;

int interruptCycle = 10;

void setTimer1(int duratuon){
	timer1_counter = duratuon/interruptCycle;
	timer1_flag = 0;
}

void setTimer2(int duratuon){
	timer2_counter = duratuon/interruptCycle;
	timer2_flag = 0;
}

void setTimer3(int duratuon){
	timer3_counter = duratuon/interruptCycle;
	timer3_flag = 0;
}

void setTimer4(int duratuon){
	timer4_counter = duratuon/interruptCycle;
	timer4_flag = 0;
}

void setTimer5(int duratuon){
	timer5_counter = duratuon/interruptCycle;
	timer5_flag = 0;
}

int isTimerFlagRaised(int n){
	switch(n){
	case 1:
		return timer1_flag;
		break;
	case 2:
		return timer2_flag;
		break;
	case 3:
		return timer3_flag;
		break;
	case 4:
		return timer4_flag;
		break;
	case 5:
		return timer5_flag;
		break;
	}
}

void timerRun(){
	if(timer1_counter > 0) {
		timer1_counter--;
	}
	if(timer1_counter == 0) timer1_flag = 1;
	//-----------------------------
	if(timer2_counter > 0) {
		timer2_counter--;
	}
	if(timer2_counter == 0) timer2_flag = 1;
	//------------------------------
	if(timer3_counter > 0) {
		timer3_counter--;
	}
	if(timer3_counter == 0) timer3_flag = 1;
	//-------------------------------
	if(timer4_counter > 0) {
		timer4_counter--;
	}
	if(timer4_counter == 0) timer4_flag = 1;
	//-------------------------------
	if(timer5_counter > 0) {
		timer5_counter--;
	}
	if(timer5_counter == 0) timer5_flag = 1;
}
