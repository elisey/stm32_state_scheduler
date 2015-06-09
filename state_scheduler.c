#include "state_scheduler.h"
#include "state_scheduler_HAL.h"
#include "stm32f10x.h"	//for assert_param
#include <stdlib.h>		//for dinamic memory
#include <stdbool.h>

static counter_t upTime_ms = 0;

#define NO_STATE	(-1)

typedef struct	{
	void (*inFunc)(void);
	void (*func)(void);
	void (*outFunc)(void);
} stateData_t;

typedef struct	{
	stateData_t *stateDataTable;
	uint8_t numOfStates;
	int currentState;
	int stateGoTo;
	bool stateChanged;
	counter_t counter;
} machineData_t;

static machineData_t machines[StateSchedulerMAX_NUM_OF_MACHINES];
static int numOfMachines = 0;

void StateScheduler_onTimer();

static bool machineIsBlocked(int machineIndex);
static void prv_processMachine(int machineIndex);
static void prv_callInFunction(int machineIndex, int state);
static void prv_callFunction(int machineIndex, int state);
static void prv_callOutFunction(int machineIndex, int state);
static void prv_tryRunFunction(void (*outFunc)(void));
static void prv_setCounter(machine_t machineIdx, counter_t counter);
static counter_t prv_getCounter(machine_t machineIdx);

machine_t StateScheduler_RegisterMachine( uint8_t numOfStates)
{
	assert_param(numOfMachines < StateSchedulerMAX_NUM_OF_MACHINES);
	machine_t currentMachineIndex = numOfMachines;
	numOfMachines++;

	machines[currentMachineIndex].stateDataTable = calloc(numOfStates, sizeof(stateData_t) );
	machines[currentMachineIndex].numOfStates = numOfStates;
	machines[currentMachineIndex].currentState = NO_STATE;
	machines[currentMachineIndex].stateGoTo = NO_STATE;
	machines[currentMachineIndex].stateChanged = true;
	machines[currentMachineIndex].counter = 0;
	return currentMachineIndex;
}

void StateScheduler_InitStateData(machine_t machineIndex, int stateIndex, void (*inFunc)(void), void (*func)(void), void (*outFunc)(void))
{
	machines[machineIndex].stateDataTable[stateIndex].inFunc = inFunc;
	machines[machineIndex].stateDataTable[stateIndex].func = func;
	machines[machineIndex].stateDataTable[stateIndex].outFunc = outFunc;
}

void StateScheduler_SetState(machine_t machineIndex, int newState)
{
	assert_param(newState < machines[machineIndex].numOfStates);
	machines[machineIndex].stateGoTo = newState;
	machines[machineIndex].stateChanged = true;
}

void StateScheduler_BlockByTime(machine_t machineIndex, counter_t time)
{
	prv_setCounter(machineIndex, time);
}

void StateScheduler_Unblock(machine_t machineIndex)
{
	prv_setCounter(machineIndex, 0);
}

void StateScheduler_SetStateByTime(machine_t machineIndex, int newState, counter_t time)
{
	StateScheduler_SetState(machineIndex, newState);
	StateScheduler_BlockByTime(machineIndex, time);
}

counter_t StateScheduler_GetUpTimeMs()
{
	return upTime_ms;
}

void StateScheduler_Process()
{
	int i;
	for (i = 0; i < numOfMachines; ++i) {
		if (machineIsBlocked(i))	{
			continue;
		}
		prv_processMachine(i);
	}
}

void StateScheduler_TimerInitAndRun()
{
	bool isInitialized = false;
	if (isInitialized == false)	{
		isInitialized = true;
		StateSchedulerHAL_TimerInit();
	}
	StateSchedulerHAL_Run();
}
void StateScheduler_TimerStop()
{
	StateSchedulerHAL_Stop();
}

static bool machineIsBlocked(int machineIndex)
{
	counter_t counter = prv_getCounter(machineIndex);
	if (counter != 0)	{
		return true;
	}
	return false;
}

static void prv_processMachine(int machineIndex)
{
	machineData_t *machine = &(machines[machineIndex]);

	if (machine->stateChanged == true)	{
		if (machine->currentState != (NO_STATE))	{
			prv_callOutFunction(machineIndex, machine->currentState);
		}
		machine->currentState =machine->stateGoTo;
		int prevStateToGo = machine->stateGoTo;

		prv_callInFunction(machineIndex, machine->stateGoTo);
		if (prevStateToGo != machine->stateGoTo)	{ // при входе в состояние была смена состояния. Не выполняется
													 //функция работы в состоянии, а при следующей итерации будет выход из
													 // состояния и вход в другое состояние
			return;
		}

		machine->stateChanged = false;				//окончательно вошли в состояние
	}
	prv_callFunction(machineIndex, machine->currentState);
}

static void prv_callInFunction(int machineIndex, int state)
{
	assert_param(state < machines[machineIndex].numOfStates);

	prv_tryRunFunction(machines[machineIndex].stateDataTable[state].inFunc);
}

static void prv_callFunction(int machineIndex, int state)
{
	assert_param(state < machines[machineIndex].numOfStates);

	prv_tryRunFunction(machines[machineIndex].stateDataTable[state].func);
}

static void prv_callOutFunction(int machineIndex, int state)
{
	assert_param(state < machines[machineIndex].numOfStates);

	prv_tryRunFunction(machines[machineIndex].stateDataTable[state].outFunc);
}

static void prv_tryRunFunction(void (*ptrFunc)(void))
{
	if (ptrFunc != NULL)	{
		ptrFunc();
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

void StateScheduler_onTimer()
{
	upTime_ms++;
	int num = numOfMachines;	//доступ к переменной из другого потока
	int i;
	for (i = 0; i < num; ++i) {
		if (machines[i].counter != 0)	{
			machines[i].counter--;
		}
	}
}
