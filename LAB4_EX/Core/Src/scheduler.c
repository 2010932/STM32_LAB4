/*
 * scheduler.c
 *
 *  Created on: Nov 28, 2023
 *      Author: Admin
 */

#include "scheduler.h"
#define SCH_MAX_TASKS 			10
#define	NO_TASK_ID				0
#define TICK					10

typedef struct {
	void ( * pTask)(void);
	uint32_t Delay;
	uint32_t Period;
	uint8_t RunMe;
	uint32_t TaskID;
	uint32_t Oneshot;
} sTask;

sTask SCH_tasks_G[SCH_MAX_TASKS];

void SCH_Add_Task ( void (*pFunction)() , uint32_t Delay, uint32_t Period,uint32_t Oneshot){
	int pos;
	for(pos = 0;pos<SCH_MAX_TASKS;pos++){
		if(SCH_tasks_G[pos].pTask == 0x0000) break;
	}
	if(pos == SCH_MAX_TASKS) return;
	if(pos == 0){
		SCH_tasks_G[0].pTask = pFunction;
		SCH_tasks_G[0].Delay = Delay/TICK;
		SCH_tasks_G[0].Period = Period/TICK;
		SCH_tasks_G[0].RunMe = 0;
		SCH_tasks_G[0].Oneshot = Oneshot;
	}
	else{
		int Delay_ms = Delay/TICK;
		int pos2;
		for(int i = 0;i<=pos;i++){
			if(SCH_tasks_G[i].Delay <= Delay_ms && SCH_tasks_G[i].pTask != 0){
				Delay_ms-=SCH_tasks_G[i].Delay;
			}
			else{
				pos2 = i;
				if(SCH_tasks_G[i].pTask){
					SCH_tasks_G[i].Delay-=Delay_ms;
				}
				break;
			}
		}
		for(int i = pos;i>pos2;i--){
			SCH_tasks_G[i].pTask = SCH_tasks_G[i-1].pTask;
			SCH_tasks_G[i].Delay = SCH_tasks_G[i-1].Delay;
			SCH_tasks_G[i].Period = SCH_tasks_G[i-1].Period;
			SCH_tasks_G[i].RunMe = SCH_tasks_G[i-1].RunMe;
			SCH_tasks_G[i].Oneshot = SCH_tasks_G[i-1].Oneshot;
		}
		SCH_tasks_G[pos2].pTask = pFunction;
		SCH_tasks_G[pos2].Delay = Delay_ms;
		SCH_tasks_G[pos2].Period = Period/TICK;
		SCH_tasks_G[pos2].RunMe = 0;
		SCH_tasks_G[pos2].Oneshot = Oneshot;
	}

}

void SCH_Delete_Task(uint32_t taskID){
	for(int i = taskID+1;i<SCH_MAX_TASKS;i++){
		SCH_tasks_G[i-1].pTask = SCH_tasks_G[i].pTask;
		SCH_tasks_G[i-1].Delay = SCH_tasks_G[i].Delay;
		SCH_tasks_G[i-1].Period = SCH_tasks_G[i].Period;
		SCH_tasks_G[i-1].RunMe = SCH_tasks_G[i].RunMe;
		SCH_tasks_G[i-1].Oneshot = SCH_tasks_G[i].Oneshot;
	}
	SCH_tasks_G[SCH_MAX_TASKS-1].pTask = 0x0000;
	SCH_tasks_G[SCH_MAX_TASKS-1].Delay = 0;
	SCH_tasks_G[SCH_MAX_TASKS-1].Period = 0;
	SCH_tasks_G[SCH_MAX_TASKS-1].RunMe = 0;
	SCH_tasks_G[SCH_MAX_TASKS-1].Oneshot = 0;
}

void SCH_Update(void){
	if(SCH_tasks_G[0].pTask){
		if(SCH_tasks_G[0].Delay <=0){
			SCH_tasks_G[0].RunMe++;
		}
		else{
			SCH_tasks_G[0].Delay--;
		}
	}
}

void SCH_Dispatch_Tasks(void){
	sTask temtask;
	for (int i = 0 ; i < SCH_MAX_TASKS ; i++) {
		if(SCH_tasks_G[i].RunMe > 0){
			SCH_tasks_G[i].RunMe--;
			(*SCH_tasks_G[i].pTask)();
			temtask.Period = SCH_tasks_G[i].Period;
			temtask.pTask = SCH_tasks_G[i].pTask;
			temtask.Oneshot = SCH_tasks_G[i].Oneshot;
			SCH_Delete_Task(i);
			if(temtask.Oneshot == 0){
				SCH_Add_Task(temtask.pTask, temtask.Period*TICK, temtask.Period*TICK, temtask.Oneshot);
			}
		}
	}
}
