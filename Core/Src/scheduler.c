/*
 * scheduler.c
 *
 *  Created on: Mar 23, 2023
 *      Author: tan
 */

#include<scheduler.h>

typedef struct{
	void (*pTask)();
	uint32_t 	Delay; // The original delay pushed
	uint32_t 	Period;
	uint32_t 	RunMe;
	uint32_t 	TaskID;
}sTasks;
/*
typedef struct{
	sTasks task;
	struct Node* next;
}Node;
*/
unsigned int Error_code_G = 0;

sTasks SCH_tasks_G[SCH_MAX_TASKS];

uint8_t current_index_task = 0;

uint8_t passed_time_since_last_task = 0;

void SCH_Init(void){
	current_index_task = 0;
}

unsigned int SCH_Report(){
	return Error_code_G;
}

sTasks emptyTask(){
	sTasks empty;
	empty.Delay = 0;
	empty.Period = 0;
	empty.RunMe = 0;
	empty.TaskID = 0;
	empty.pTask = 0x0000;
	return empty;
}

void SCH_shift(uint8_t pos){
	for(uint8_t i = current_index_task-1; i > pos; i--){
		SCH_tasks_G[i] = SCH_tasks_G[i-1];
	}
	SCH_tasks_G[pos] = emptyTask();
}

uint32_t SCH_Add_Task(void (*pFunction)() , uint32_t DELAY, uint32_t PERIOD){
	sTasks task;
	uint8_t insert_idx = 0;
	if(current_index_task < SCH_MAX_TASKS){
		task.pTask = pFunction;
		task.Delay = DELAY;
		task.Period =  PERIOD;
		task.RunMe = 0;
		task.TaskID = current_index_task;
		if(current_index_task == 0){
			SCH_tasks_G[0] = task;
			current_index_task++;
			return 0;
		}
		while(task.Delay > SCH_tasks_G[insert_idx].Delay && insert_idx < current_index_task){
			task.Delay -= SCH_tasks_G[insert_idx].Delay;
			insert_idx++;
		}
		current_index_task++;
		SCH_shift(insert_idx);
		SCH_tasks_G[insert_idx] = task;
		SCH_tasks_G[insert_idx+1].Delay -=  SCH_tasks_G[insert_idx].Delay;
		return task.TaskID;
	}
	return 0;
}

uint8_t SCH_Delete_Task(uint32_t taskID){
	if(SCH_tasks_G[taskID].pTask == 0x0000){
		Error_code_G = ERROR_SCH_CANNOT_DELETE_TASK;
	}

	for(unsigned int idx=taskID; idx<current_index_task-1; idx++){
		SCH_tasks_G[idx] = SCH_tasks_G[idx+1];
	}
	SCH_tasks_G[current_index_task-1] = emptyTask();
	current_index_task--;
	return 0;
}

void SCH_Dispatch_Tasks(void){
	while(SCH_tasks_G[0].pTask!=0x0000){
		if(SCH_tasks_G[0].RunMe > 0){
			(*SCH_tasks_G[0].pTask)();
			SCH_tasks_G[0].RunMe--;
			if(SCH_tasks_G[1].Delay==0) SCH_tasks_G[1].RunMe++;
			if(SCH_tasks_G[0].Period>0) SCH_Add_Task(SCH_tasks_G[0].pTask,
										             SCH_tasks_G[0].Period,
													 SCH_tasks_G[0].Period);
			SCH_Delete_Task(0);
		}
	}
}

void SCH_Update(void){
	if(SCH_tasks_G[0].Delay > 0){
		SCH_tasks_G[0].Delay--;
		passed_time_since_last_task++;
	}
	else if(SCH_tasks_G[0].Delay==0){
		SCH_tasks_G[0].RunMe++;
		return;
	}
}


