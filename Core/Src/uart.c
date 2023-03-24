/*
 * uart.c
 *
 *  Created on: Mar 20, 2023
 *      Author: tan
 */

#include"uart.h"

UART_HandleTypeDef huart1;

//Global variable
uint8_t buffer[MAX_BUFFER_SIZE];
uint8_t index_buffer = 0;
uint8_t buffer_flag = 0;
uint8_t buffer_byte;
uint8_t str[MAX_BUFFER_SIZE];
uint32_t toSend = 0;
uint8_t cmd_buffer[MAX_BUFFER_SIZE];
uint8_t cmd_index = 0;
uint8_t cmd_flag = INIT_UART;
uint8_t action_flag = STOP_SEND;

int idx = 0;

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

void command_parser_fsm(){
//We collect all char sent to process at once
//Remember to flush buffer using the resetBuf and resetCmdBuf(), or else your buffer
//Will be as dirty as Minklee
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
				else if (cmd_buffer[0] == 'O') action_flag = STOP_SEND;
				index_buffer = 0;
				cmd_index = 0;
				idx = 0;
				resetBuf();
				resetCmdBuf();
				uart_communiation_fsm();
			}
			break;
		default:
			break;
	}

}

void uart_communiation_fsm(){
	if(action_flag==SEND){
		toSend = 30;
		HAL_UART_Transmit(&huart1, &(str[0]), sprintf( &(str[0]), "!1:T:%ld#\n", toSend), 100);
	}
	else if(action_flag==STOP_SEND){
		return;
	}
}

