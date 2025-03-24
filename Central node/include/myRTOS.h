/*
 * RTOS.h
 *
 *  Created on: Nov 17, 2024
 *      Author: vohoa
 */

#ifndef INC_RTOS_H_
#define INC_RTOS_H_
 
#include "global.h"
#include <stdlib.h>
 
typedef struct Task{
  void (*functionPointer) (void);
  int Delay;
  int Period;
  struct Task*next;
  struct Task*previous;
}Task;

typedef struct List{
  Task * head;
  Task * tail;
  int TaskCount;
}List;
 
extern List list;
 
void InitRTOS(void);
void AddTask(void(*function)(), int Delay, int Period);
void Update(void);
void DispatchTask(void);
int CheckTask(void(*function)());
int DeleteTask(void(*function)());
 
#endif /* INC_RTOS_H_ */