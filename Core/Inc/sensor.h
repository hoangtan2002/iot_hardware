/*
 * sensor.h
 *
 *  Created on: Mar 26, 2023
 *      Author: tan
 */

#ifndef INC_SENSOR_H_
#define INC_SENSOR_H_
#include "main.h"
#include "uart.h"

extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;

float getTemp();
float getHumid();

void scanAddr();
void pullSensor();
void dht20Init(void);
void dht20Reset(uint8_t regist);
void dht20Start(void);
void dht20Read(uint16_t* value);
void initReading(void);

int getSensorStatus();

#endif /* INC_SENSOR_H_ */
