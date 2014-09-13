#include "led_fsm.h"
#include "led.h"
#include "state_scheduler.h"

Led_t greenLed;
machine_t ledMachine;
typedef enum	{
	state_init = 0,
	state_tougle_led = 1,
	state_wait = 2
} led_fsm_states_t;

static void prv_state_init();
static void prv_state_tougle_led();
static void prv_state_wait();

void LedFsm_Init()
{
	ledMachine = StateScheduler_RegisterMachine(3);
	StateScheduler_InitStateData(ledMachine, state_init, 0, prv_state_init, 0);
	StateScheduler_InitStateData(ledMachine, state_tougle_led, 0, prv_state_tougle_led, 0);
	StateScheduler_InitStateData(ledMachine, state_wait, 0, prv_state_wait, 0);
}


static void prv_state_init()
{
	Led_Init(&greenLed, GPIOC, GPIO_Pin_9);
	StateScheduler_SetState(ledMachine, state_tougle_led);
}

static void prv_state_tougle_led()
{
	Led_Tougle(&greenLed);
	StateScheduler_SetState(ledMachine, state_wait);
}

static void prv_state_wait()
{
	static int counter = 0;
	counter++;
	if (counter >= 40000)	{
		counter = 0;
		StateScheduler_SetState(ledMachine, state_tougle_led);
	}
}
