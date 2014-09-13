#include "stm32f10x.h"
#include "led.h"
#include "state_scheduler.h"
#include "uart.h"
#include <stdio.h>
#include <stdbool.h>
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
	UART_Init();
	machineTest = StateScheduler_RegisterMachine(3);
	StateScheduler_InitStateData(machineTest, 0, machine1_state1_in, machine1_state1, machine1_state1_out);
	StateScheduler_InitStateData(machineTest, 1, machine1_state2_in, machine1_state2, machine1_state2_out);
	StateScheduler_InitStateData(machineTest, 2, machine1_state3_in, machine1_state3, machine1_state3_out);

	Led_t greenLed;
	Led_Init(&greenLed, GPIOC, GPIO_Pin_9);
	Led_SetState(&greenLed, led_on);
    while(1)
    {
    	StateScheduler_Process();
    	printf("-----------new iteration-----------\r\n");
    	delay();
    }
}

void delay()
{
	volatile int i;
	for (i = 0; i < 4000000; ++i) {

	}
}

void machine1_state1_in()
{
	printf("Machine 1, state 1, IN -switch 2-\r\n");
	StateScheduler_SetState(machineTest, 1);
}

void machine1_state1()
{
	printf("Machine 1, state 1, RUN\r\n");

}

void machine1_state1_out()
{
	printf("Machine 1, state 1, OUT\r\n");
}

void machine1_state2_in()
{

	printf("Machine 1, state 2, IN\r\n");
}

void machine1_state2()
{
	static int counter = 0;
	counter++;
	if (counter >= 3)	{
		counter = 0;
		StateScheduler_SetState(machineTest, 2);
		printf("Machine 1, state 2, RUN -switch 3-\r\n");
		return;
	}
	printf("Machine 1, state 2, RUN\r\n");

}

void machine1_state2_out()
{
	static bool needSwitch = false;
	if (needSwitch)	{
		printf("Machine 1, state 2, OUT -switch 1-\r\n");
		StateScheduler_SetState(machineTest, 0);
		needSwitch = false;
	}
	else	{
		needSwitch = true;
		printf("Machine 1, state 2, OUT\r\n");
	}

}

void machine1_state3_in()
{

	printf("Machine 1, state 3, IN\r\n");
}

void machine1_state3()
{
	printf("Machine 1, state 3, RUN -switch-\r\n");
	StateScheduler_SetState(machineTest, 0);
}

void machine1_state3_out()
{
	printf("Machine 1, state 3, OUT\r\n");
}

void assert_failed(uint8_t* file, uint32_t line)
{
	Led_t blueLed;
	Led_Init(&blueLed, GPIOC, GPIO_Pin_8);
	Led_SetState(&blueLed, led_on);
	while(1);
}
