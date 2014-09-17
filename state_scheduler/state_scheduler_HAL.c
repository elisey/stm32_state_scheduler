#include "state_scheduler_HAL.h"
#include "stm32f10x.h"

static void prv_timerInit();
void TimerService_IRQHandler();
extern void prv_onTImer();


void StateSchedulerHAL_Init()
{
	prv_timerInit();
}

static void prv_timerInit()
{
	RCC_APB1PeriphClockCmd( TimerService_RCC_APB1, ENABLE );
	TIM_TimeBaseInitTypeDef timerStruct;
	timerStruct.TIM_ClockDivision = 0;
	timerStruct.TIM_CounterMode = TIM_CounterMode_Up;
	timerStruct.TIM_Period = 2000;
	timerStruct.TIM_Prescaler = (12 - 1);
	TIM_TimeBaseInit( TimerService_TIMx, &timerStruct );

	TIM_ITConfig( TimerService_TIMx, TIM_IT_Update, ENABLE );
	TIM_Cmd( TimerService_TIMx, ENABLE );

	NVIC_EnableIRQ( TimerService_IRQn );

	DBGMCU->CR |= TImerService_DBGMCU_CR_DBG_TIMx_STOP;
}

void TimerService_IRQHandler()
{
	if (TIM_GetITStatus(TimerService_TIMx, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TimerService_TIMx, TIM_IT_Update);
		prv_onTImer();
	}
}
