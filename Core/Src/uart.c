/*
 * uart.c
 *
 *  Created on: Mar 20, 2023
 *      Author: tan
 */

#include"uart.h"
#include"sensor.h"
#include"led.h"

#define START_CHR '!'
#define END_CHR '#'

#define SEND_TEMP 0
#define SEND_HUMID 1

UART_HandleTypeDef huart1;

//Global variable
uint8_t buffer[MAX_BUFFER_SIZE];
uint8_t index_buffer = 0;
uint8_t buffer_flag = 0;
uint8_t buffer_byte;
uint8_t str[MAX_BUFFER_SIZE];
uint8_t tempStr[MAX_BUFFER_SIZE];
uint8_t cmd_buffer[MAX_BUFFER_SIZE];
uint8_t cmd_index = 0;
uint8_t cmd_flag = INIT_UART;
uint8_t action_flag = STOP_SEND;

uint32_t checksumResult = 0;
int idx = 0;
int whatToSend = SEND_TEMP;

uint32_t checksumCalc(uint8_t* str, uint32_t n){
	uint32_t result = 0;
	for(uint8_t i=0; i<n; i++){
		result += str[i];
	}
	return result;
}

void resetStr(){
	for(int i=0; i<MAX_BUFFER_SIZE; i++){
		str[i] = 0;
	}
}

void resetCmdBuf(){
	for(int i=0; i<MAX_BUFFER_SIZE; i++){
		cmd_buffer[i] = 0;
	}
}

void resetBuf(){
	for(int i=0; i<MAX_BUFFER_SIZE; i++){
		buffer[i] = 0;
	}
}

void sendMCUInfo(){
	checksumResult = checksumCalc(&(tempStr[0]),
								  sprintf( &(tempStr[0]),
										  "VER:%s:%s", MCU_VER, FIRMWARE_VER));
	HAL_UART_Transmit(&huart1, &(str[0]),
					  sprintf(&(str[0]), "!%s:%u#\n",
							  tempStr, checksumResult),
					  100);
	checksumResult = 0;
}

void sendSensorInfo(){
	char status[8];
	if(getSensorStatus()){
		sprintf(&(status[0]), "!OK#");
		HAL_UART_Transmit(&huart1, &(status[0]), 8, 100);
	} else{
		sprintf(&(status[0]), "!ERROR#");
		HAL_UART_Transmit(&huart1, &(status[0]), 8, 100);
	}
}

void command_parser_fsm(){
//We collect all char sent to process at once
//Remember to flush buffer using the resetBuf and resetCmdBuf(), or else your buffer
//Will be dirty
	switch(cmd_flag){
		case INIT_UART:
			if (buffer[idx] == '!') {
				idx++;
				cmd_flag = READING;
			}
			break;
		case READING:
			if (buffer[idx] != '!' && buffer[idx] != '#'){
				cmd_buffer[cmd_index] = buffer[idx];
				cmd_index++;
				idx++;
			}
			else if (buffer[idx] == '!'){
				cmd_flag = INIT_UART;
				idx = 0;
			}
			else if (buffer[idx] == '#'){
				cmd_flag = INIT_UART;
				if (cmd_buffer[0] == 'R') action_flag = SEND;
				else if (cmd_buffer[0] == 'O' && cmd_buffer[1] == 'K') action_flag = STOP_SEND;
				else if (cmd_buffer[0] == 'V') sendMCUInfo();
				else if (cmd_buffer[0] == 'S') sendSensorInfo();
				else if (cmd_buffer[0] == 'O' && cmd_buffer[1] == 'N'){
					if(cmd_buffer[2] == '1'){
						setLedState(1, ON);
					}
					else setLedState(2,ON);
				}
				else if (cmd_buffer[0] == 'O' && cmd_buffer[1] == 'F' && cmd_buffer[2] == 'F'){
						if(cmd_buffer[3] == '1'){
							setLedState(1, OFF);
						}
						else setLedState(2,OFF);
					}
				index_buffer = 0;
				cmd_index = 0;
				idx = 0;
				resetBuf();
				resetCmdBuf();
			}
			break;
		default:
			break;
	}
}

void uart_communiation_fsm(){
	if(action_flag==SEND){
		//resetStr();
		if(getSensorStatus()==0){
			checksumResult = checksumCalc(&(tempStr[0]),
										  sprintf(&(tempStr[0]),
												  "ERR:%.2f:%.2f", 0.0, 0.0));
			HAL_UART_Transmit(&huart1,
							  &(str[0]),
							  sprintf(&(str[0]),
									  "!%s:%u#\n", tempStr, checksumResult), 100);
			checksumResult = 0;
			return;
		}
		checksumResult = checksumCalc(&(tempStr[0]),
									  sprintf( &(tempStr[0]),
											  "OK:%.2f:%.2f",
											  getTemp(),
											  getHumid()));
		HAL_UART_Transmit(&huart1,
						  &(str[0]),
						  sprintf( &(str[0]), "!%s:%u#\n", tempStr, checksumResult),
						  100);
		checksumResult = 0;
	}
	else if(action_flag==STOP_SEND){
		return;
	}
}


