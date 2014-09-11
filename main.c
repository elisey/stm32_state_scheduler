#include "stm32f10x.h"
#include "led.h"

int main(void)
{
	Led_t greenLed;
	Led_Init(&greenLed, GPIOC, GPIO_Pin_9);
	Led_SetState(&greenLed, led_on);
    while(1)
    {

    }
}

void assert_failed(uint8_t* file, uint32_t line)
{
	Led_t blueLed;
	Led_Init(&blueLed, GPIOC, GPIO_Pin_8);
	Led_SetState(&blueLed, led_on);
	while(1);
}
