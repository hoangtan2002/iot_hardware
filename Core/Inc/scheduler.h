/*
 * scheduler.h
 *
 *  Created on: Mar 23, 2023
 *      Author: tan
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include <stdint.h>

#define ERROR_SCH_TOO_MANY_TASKS 2
#define ERROR_SCH_WAITING_FOR_SLAVE_TO_ACK 3
#define ERROR_SCH_WAITING_FOR_START_COMMAND_FROM_MASTER 4
#define ERROR_SCH_ONE_OR_MORE_SLAVES_DID_NOT_START 5
#define ERROR_SCH_LOST_SLAVE 6
#define ERROR_SCH_CAN_BUS_ERROR 7
#define ERROR_I2C_WRITE_BYTE_AT24C64 8
#define ERROR_SCH_CANNOT_DELETE_TASK 9
#define SCH_MAX_TASKS 40
#define ONE_SHOT_TASK 0

void SCH_Init(void);

uint32_t SCH_Add_Task ( void (*pFunction)() ,
					uint32_t DELAY,
					uint32_t PERIOD);

void SCH_Update(void);
void SCH_Dispatch_Tasks(void);

#endif /* INC_SCHEDULER_H_ */
