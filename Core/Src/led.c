/*
 * led.c
 *
 *  Created on: Apr 15, 2023
 *      Author: tan
 */
#include "led.h"
#include "main.h"


void setLedState(int led, int state){
	if(led == 1) HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, state);
	else HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, state);
}
