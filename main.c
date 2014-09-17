#include "stm32f10x.h"
#include "led.h"
#include "state_scheduler.h"
#include "uart.h"
#include <stdio.h>
#include <stdbool.h>
#include "led_fsm.h"
void delay();
void machine1_state1_in();
void machine1_state1();
void machine1_state1_out();
void machine1_state2_in();
void machine1_state2();
void machine1_state2_out();
void machine1_state3_in();
void machine1_state3();
void machine1_state3_out();

machine_t machineTest;

int main(void)
{
	StateScheduler_Init();
	UART_Init();

	LedFsm_Init();
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
