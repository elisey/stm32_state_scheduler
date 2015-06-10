#pragma once

#include <stdint.h>

typedef struct	{
	uint8_t state;
} flag_t;

void Flag_init(flag_t *this);
int  Flag_isSet(flag_t *this);
void Flag_set(flag_t *this);
