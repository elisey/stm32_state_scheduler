#include "flag.h"

void Flag_init(flag_t *this)
{
	this->state = 0;
}
int  Flag_isSet(flag_t *this)
{
	int state = this->state;
	this->state = 0;
	return state;
}
void Flag_set(flag_t *this)
{
	this->state = 1;
}
