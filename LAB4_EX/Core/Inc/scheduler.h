/*
 * scheduler.h
 *
 *  Created on: Nov 28, 2023
 *      Author: Admin
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include "stdint.h"

void SCH_Init(void);
void SCH_Update(void);
void SCH_Add_Task(void (*p_function)(), uint32_t Delay, uint32_t Period, uint32_t Oneshot);
void SCH_Dispatch_Tasks(void);
void SCH_Delete_Task(uint32_t taskID);

#endif /* INC_SCHEDULER_H_ */
