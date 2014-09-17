#include "state_scheduler.h"
#include "state_scheduler_HAL.h"
#include "stm32f10x.h"
#include <string.h>
#include <stdlib.h>

typedef struct	{
	void (*inFunc)(void);
	void (*func)(void);
	void (*outFunc)(void);
} stateData_t;

typedef struct	{
	stateData_t *stateDataTable;
	int numOfStates;
	int currentState;
	int stateGoTo;
	counter_t counter;
} machineData_t;

static machineData_t machines[StateSchedulerNUM_OF_MACHINES];
static int numOfMachines = 0;


void prv_onTImer();
static void prv_setCounter(machine_t machineIdx, counter_t counter);
static counter_t prv_getCounter(machine_t machineIdx);

void StateScheduler_Init()
{
	StateSchedulerHAL_Init();
}

void StateScheduler_InitStateData(machine_t machine, int stateIndex, void (*inFunc)(void), void (*func)(void), void (*outFunc)(void))
{
	machines[machine].stateDataTable[stateIndex].inFunc = inFunc;
	machines[machine].stateDataTable[stateIndex].func = func;
	machines[machine].stateDataTable[stateIndex].outFunc = outFunc;
}

machine_t StateScheduler_RegisterMachine( int numOfStates)
{
	assert_param(numOfMachines < StateSchedulerNUM_OF_MACHINES);
	machine_t currentMachineIndex = numOfMachines;
	numOfMachines++;

	machines[currentMachineIndex].stateDataTable = calloc(numOfStates, sizeof(stateData_t) );
	machines[currentMachineIndex].numOfStates = numOfStates;
	machines[currentMachineIndex].currentState = -1;
	machines[currentMachineIndex].stateGoTo = 0;
	machines[currentMachineIndex].counter = 0;
	return currentMachineIndex;
}

void StateScheduler_SetState(machine_t machineIdx, int newState)
{
	assert_param(newState < machines[machineIdx].numOfStates);
	machines[machineIdx].stateGoTo = newState;
}

void StateScheduler_BlockByTime(machine_t machineIdx, counter_t time)
{
	prv_setCounter(machineIdx, time);
}

void StateScheduler_Process()
{
	int i;
	for (i = 0; i < numOfMachines; ++i) {
		counter_t counter = prv_getCounter(i);
		if (counter != 0)	{
			continue;
		}
		if (machines[i].stateGoTo != (-1))	{
			// была смена состояния
			if ( (machines[i].currentState != (-1)) && machines[i].stateDataTable[machines[i].currentState].outFunc != NULL)	{
				machines[i].stateDataTable[machines[i].currentState].outFunc(); //если при выходе из состояния произошла
																				//смена состояния, то будет вход в новое состояние
			}
			machines[i].currentState = machines[i].stateGoTo;
			int prevStateToGo = machines[i].stateGoTo;
			if (machines[i].stateDataTable[machines[i].stateGoTo].inFunc != NULL)	{
				machines[i].stateDataTable[machines[i].stateGoTo].inFunc();
			}

			if (prevStateToGo != machines[i].stateGoTo)	{ // при входе в состояние была смена состояния. Не выполняется
														 //функция работы в состоянии, а при следующей итерации будет выход из
														 // состояния и вход в другое состояние
				continue;
			}

			machines[i].stateGoTo = (-1);
		}
		if (machines[i].stateDataTable[machines[i].currentState].func != NULL)	{
			machines[i].stateDataTable[machines[i].currentState].func();
		}

	}
}

void prv_onTImer()
{
	int i;
	for (i = 0; i < numOfMachines; ++i) {
		if (machines[i].counter != 0)	{
			machines[i].counter--;
		}
	}
}

static void prv_setCounter(machine_t machineIdx, unsigned int counter)
{
	StateSchedulerHAL_DisableIrq();
	machines[machineIdx].counter = counter;
	StateSchedulerHAL_EnableIrq();
}

static unsigned int prv_getCounter(machine_t machineIdx)
{
	counter_t time;
	StateSchedulerHAL_DisableIrq();
	time = machines[machineIdx].counter;
	StateSchedulerHAL_EnableIrq();
	return time;
}
