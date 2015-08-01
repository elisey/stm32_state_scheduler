#include "state_scheduler_HAL.h"

#include <iostm8s003f3.h>

extern void StateScheduler_onTimer();

void StateSchedulerHAL_TimerInit()
{
	TIM4_SR_bit.UIF = 0;
	TIM4_PSCR = 0x7;	//Предделитель на 2^7=128   16000000/128 = 125000 Hz
	TIM4_ARR = 125;		//1kHz
	TIM4_IER_bit.UIE = 1;
	//TIM4_CR1_bit.CEN = 1;
}

void StateSchedulerHAL_DisableIrq()
{
	TIM4_IER_bit.UIE = 0;
}

void StateSchedulerHAL_EnableIrq()
{
	TIM4_IER_bit.UIE = 1;
}

void StateSchedulerHAL_Run()
{
	TIM4_CR1_bit.CEN = 1;
}

void StateSchedulerHAL_Stop()
{
	TIM4_CR1_bit.CEN = 0;
}

#pragma vector=0x19 
__interrupt void tim4_ovr_irq(void)
{
	TIM4_SR_bit.UIF = 0;
	StateScheduler_onTimer();	
}