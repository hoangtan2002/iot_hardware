/*
 * sensor.c
 *
 *  Created on: Mar 26, 2023
 *      Author: tan
 */

#include "sensor.h"
#include "main.h"
#include<stdio.h>
#include<string.h>
#define SLAVE_ADDRESS_DHT20 (0x38 << 1)

float temperature = 20.5;
float humidity = 50.5;
int sensorCount = 0;
uint16_t value_x10[2] = {0, 0};
char tempBuf[30];

float getTemp(){
	return temperature;
}
float getHumid(){
	return humidity;
}

int getSensorStatus(){
	return sensorCount;
}

void dht20Init(void){
	//Set register when call a wrong reset
	uint8_t init[3];

	init[0] = 0xA8;
	init[1] = 0x00;
	init[2] = 0x00;
	HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_DHT20, (uint8_t*) init, 3, 0xFF);
	HAL_Delay(20);

	init[0] = 0xBE;
	init[1] = 0x08;
	init[2] = 0x00;
	HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_DHT20, (uint8_t*) init, 3, 0xFF);
	HAL_Delay(20);
}

void dht20Reset(uint8_t regist){
	//reset register
	uint8_t reset[3], reply[3];

	reset[0] = regist;
	reset[1] = 0x00;
	reset[2] = 0x00;
 	HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_DHT20, (uint8_t*) reset, 3, 0xFF);
	HAL_Delay(10);

	HAL_I2C_Master_Receive(&hi2c1, SLAVE_ADDRESS_DHT20 | 0x01, (uint8_t*) reply, 3, 0xFF);
	reset[0] = 0xB0 | regist;
	reset[1] = reply[1];
	reset[2] = reply[2];
	HAL_Delay(10);

	HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_DHT20, (uint8_t*) reset, 3, 0xFF);
}

void sensorCheck(){
	HAL_StatusTypeDef res;
	res = HAL_I2C_IsDeviceReady(&hi2c1, SLAVE_ADDRESS_DHT20, 1, 10);
	if(res != HAL_OK){
		sensorCount = 0;
	}
	else sensorCount = 1;
}

void dht20Start(void){
	//query the DHT20
	uint8_t status[1];
	HAL_I2C_Master_Receive(&hi2c1, SLAVE_ADDRESS_DHT20 | 0x01, (uint8_t*) status, 1, 0xFF);

	if((status[0] & 0x18) != 0x18){
		dht20Reset(0x1B);
		dht20Reset(0x1C);
		dht20Reset(0x1E);
	}

	uint8_t data[3] = {0xAC, 0x33, 0x00};

	//HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_DHT20, (uint8_t*) trigger, 1, 0xFF);
	HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_DHT20, (uint8_t*) data, 3, 0xFF);
	HAL_Delay(80);
}

void dht20Read(uint16_t* value){
	dht20Start();
	uint8_t data[7];
	uint32_t Temper = 0, Humid = 0;
	HAL_I2C_Master_Receive(&hi2c1, SLAVE_ADDRESS_DHT20 | 0x01, (uint8_t*) data, 7, 0xFF);

	//Humid
	Humid = (Humid | data[1]) << 8;
	Humid = (Humid | data[2]) << 8;
	Humid = Humid | data[3];
	Humid = Humid >> 4;
    Humid = (Humid * 100 * 10 / 1024 / 1024);
    value[0] = Humid;
    humidity = value[0]/10 + (value[0]%10)/10;
	//Temperature
    Temper = (Temper | data[3]) << 8;
    Temper = (Temper | data[4]) << 8;
    Temper = Temper | data[5];
    Temper = Temper & 0xfffff;
    Temper = Temper*200*10/1024/1024 - 500;
	value[1] = Temper;
	temperature = value[1]/10 + (value[1]%10)/10;

}

void pullSensor(){
	dht20Read(&(value_x10[0]));
}

void initReading(void){
	dht20Init();
}

void scanAddr() {
    char info[] = "\n\n Scanning I2C bus...\n\n";
    HAL_StatusTypeDef res;
    for(uint16_t i = 0; i < 128; i++) {
        res = HAL_I2C_IsDeviceReady(&hi2c1, i << 1, 1, 10);
        if(res == HAL_OK) {
            char msg[64];
            snprintf(msg, sizeof(msg), "0x%02X", i);
            sensorCount++;
        } else {
        }
    }
}


