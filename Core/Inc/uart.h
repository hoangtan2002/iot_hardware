/*
 * uart.h
 *
 *  Created on: Mar 20, 2023
 *      Author: tan
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "main.h"
#include"timer.h"
#include<stdio.h>

extern UART_HandleTypeDef huart1;

#define MAX_BUFFER_SIZE 10

#define CMD_READY 5
#define	CMD_WAIT 6

#define READING 1
#define STOP  2

#define IDLE 0
#define START_CMD 1

#define INIT_UART 0


#define FLAG_R 10
#define FLAG_S 11
#define FLAG_T 12
#define FLAG_O 13
#define FLAG_K 14

#define ERR_CMD_BUF 7

#define SEND 3
#define STOP_SEND 4

extern uint8_t temp;
extern uint8_t buffer[MAX_BUFFER_SIZE];
extern uint8_t index_buffer;
extern uint8_t buffer_flag;
extern uint8_t buffer_byte;
extern uint8_t action_flag;
extern uint8_t cmd_buffer[MAX_BUFFER_SIZE];
extern uint8_t cmd_flag;
extern uint32_t toSend;

void sendMCUStatus();
void resetCmdBuf();
void resetBuf();
void command_parser_fsm();
void uart_communiation_fsm();

#endif /* INC_UART_H_ */
