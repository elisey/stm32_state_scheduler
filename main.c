#include "stm32f10x.h"
#include "led.h"
#include "state_scheduler.h"
#include "uart.h"
#include "led_fsm.h"

machine_t machineTest;

int main(void)
{
	UART_Init();

	LedFsm_Init();
	StateScheduler_TimerInitAndRun();
    while(1)
    {
    	StateScheduler_Process();
    }
}

void assert_failed(uint8_t* file, uint32_t line)
{
	Led_t blueLed;
	Led_Init(&blueLed, GPIOC, GPIO_Pin_8);
	Led_SetState(&blueLed, led_on);
	while(1);
}
