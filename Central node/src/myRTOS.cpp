/*
 * RTOS.c
 *
 *  Created on: Nov 17, 2024
 *      Author: vohoa
 */

#include "myRTOS.h"


List list;
 
void InitRTOS(void){
  list.head = list.tail = NULL;
  list.TaskCount = 0;
}
 
void AddTask(void(*function)(), int Delay, int Period){
  Task * newTask = (Task *) malloc (sizeof(Task));
  if(newTask == NULL) return ;
  newTask->functionPointer =  function;
  newTask->Delay = Delay;
  newTask->Period = Period;
  newTask->next = newTask->previous = NULL;
  if(list.head == NULL) {list.head = newTask, list.tail = newTask;}
  else{
      Task*currentPointer = list.head;
      //Moving Current pointer inside list
      while (currentPointer != NULL && newTask->Delay >= currentPointer->Delay){
          newTask->Delay = newTask->Delay - currentPointer->Delay;
          currentPointer = currentPointer->next;
      }
      if(currentPointer == list.head){
        //Add task to list head
        newTask->next = list.head;
        list.head->previous = newTask;
        list.head->Delay = list.head->Delay - newTask->Delay;
        list.head = newTask;
      }
      else if(currentPointer == NULL){
        //Add task to list tail
        newTask->previous = list.tail;
        list.tail->next = newTask;
        list.tail = newTask;
      }
      else{
        //Add task to middle list
        newTask->next = currentPointer;
        newTask->previous = currentPointer->previous;
        currentPointer->previous->next =  newTask;
        currentPointer->previous = newTask;
        currentPointer->Delay = currentPointer->Delay - newTask->Delay;
      }
  }
  list.TaskCount++;
}
void Update(){
  if(list.head == NULL) return;
  list.head->Delay--;
}
 
void DispatchTask(void){
  if(list.head == NULL) return;
  if(list.head->Delay <= 0){
    Task* runPointer = list.head;
    list.head = list.head->next;
    runPointer->functionPointer();
    if(runPointer->Period > 0) AddTask(runPointer->functionPointer, runPointer->Period*10, runPointer->Period*10);
    free(runPointer);
    list.TaskCount--;
  }
}
 
int CheckTask(void(*function)()){
  if(list.TaskCount == 0) return 0;
  Task* listTempPointer = list.head;
  while(listTempPointer != NULL){
    if(listTempPointer->functionPointer == function) return 1;
     listTempPointer = listTempPointer->next;
  }
  return 0;
}
 
int DeleteTask(void(*function)()){
  if(list.TaskCount == 0) return 0;
  if(list.TaskCount == 1){
    Task* temp = list.head;
    list.head = list.tail = NULL;
    list.TaskCount = 0;
    free(temp);
    return 1;
  }
  else{
    Task* listTempPointer = list.head;
    while (listTempPointer != NULL){
      if(listTempPointer->functionPointer == function){
        if(listTempPointer == list.head){
          //Delete task in the list head
          listTempPointer->next->previous = NULL;
          list.head = listTempPointer->next;
          list.head->Delay = list.head->Delay + listTempPointer->Delay;
        }
        else if(listTempPointer == list.tail){
          //Delete task in the list head
          listTempPointer->previous->next = NULL;
          list.tail = listTempPointer->previous;
        }
        else{
          //Delete task in the middle of the list
          listTempPointer->previous->next = listTempPointer->next;
          listTempPointer->next->previous = listTempPointer->previous;
          listTempPointer->next->Delay =listTempPointer->next->Delay + listTempPointer->Delay;
        }
        list.TaskCount--;
        free(listTempPointer);
        return 1;
      }
      listTempPointer = listTempPointer->next;      //Traverse pointer to the next position of list
    }
  }
  return 0;
}