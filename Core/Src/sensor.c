/*
 * sensor.c
 *
 *  Created on: Mar 26, 2023
 *      Author: tan
 */

#include "sensor.h"

uint32_t adcVal;

float temp = 20.5;
float humid = 50.5;

float getTemp(){
	return temp;
}
float getHumid(){
	return humid;
}

void pullSensor(){
	temp += 0.5;
	if(temp > 32){
		temp = 20.5;
	}
	humid += 20.1;
	if(humid > 85){
		humid = 50.5;
	}
}


