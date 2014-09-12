#include "stm32f10x.h"
#include "led.h"
#include "state_scheduler.h"

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
    }
}

void machine1_state1_in()
{

}

void machine1_state1()
{
	StateScheduler_SetState(machineTest, 1);
}

void machine1_state1_out()
{

}

void machine1_state2_in()
{

}

void machine1_state2()
{
	StateScheduler_SetState(machineTest, 2);
}

void machine1_state2_out()
{

}

void machine1_state3_in()
{

}

void machine1_state3()
{
	StateScheduler_SetState(machineTest, 0);
}

void machine1_state3_out()
{

}

void assert_failed(uint8_t* file, uint32_t line)
{
	Led_t blueLed;
	Led_Init(&blueLed, GPIOC, GPIO_Pin_8);
	Led_SetState(&blueLed, led_on);
	while(1);
}
