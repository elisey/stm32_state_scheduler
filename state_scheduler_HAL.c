#include "state_scheduler_HAL.h"
#include "state_scheduler_config.h"

#include "stm32f10x.h"

void TimerService_IRQHandler();
extern void StateScheduler_onTimer();

void StateSchedulerHAL_TimerInit()
{
	RCC_APB1PeriphClockCmd( TimerService_RCC_APB1, ENABLE );
	TIM_TimeBaseInitTypeDef timerStruct;
	timerStruct.TIM_ClockDivision = 0;
	timerStruct.TIM_CounterMode = TIM_CounterMode_Up;
	timerStruct.TIM_Period = TimerService_TIM_Period;
	timerStruct.TIM_Prescaler = TimerService_TIM_Prescaler;
	TIM_TimeBaseInit( TimerService_TIMx, &timerStruct );

	TIM_ITConfig( TimerService_TIMx, TIM_IT_Update, ENABLE );
	//TIM_Cmd( TimerService_TIMx, ENABLE );

	NVIC_SetPriority(TimerService_IRQn, TImerService_IRQPriority);
	NVIC_EnableIRQ( TimerService_IRQn );

	DBGMCU->CR |= TImerService_DBGMCU_CR_DBG_TIMx_STOP;
}

void StateSchedulerHAL_DisableIrq()
{
	NVIC_DisableIRQ( TimerService_IRQn );
}

void StateSchedulerHAL_EnableIrq()
{
	NVIC_EnableIRQ( TimerService_IRQn );
}

void StateSchedulerHAL_Run()
{
	TIM_Cmd( TimerService_TIMx, ENABLE );
}

void StateSchedulerHAL_Stop()
{
	TIM_Cmd( TimerService_TIMx, DISABLE );
}

void TimerService_IRQHandler()
{
	if (TIM_GetITStatus(TimerService_TIMx, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TimerService_TIMx, TIM_IT_Update);
		StateScheduler_onTimer();
	}
}
