#pragma once

typedef int machine_t;
typedef unsigned int counter_t;
#define StateSchedulerNUM_OF_MACHINES	10


void StateScheduler_Init();
void StateScheduler_InitStateData(machine_t machine, int stateIndex, void (*inFunc)(void), void (*func)(void), void (*outFunc)(void));
machine_t StateScheduler_RegisterMachine( int numOfStates);
void StateScheduler_SetState(machine_t machineIdx, int newState);
void StateScheduler_BlockByTime(machine_t machineIdx, counter_t time);
void StateScheduler_Process();
