#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct	{
	uint8_t *ptrBuffer;
	size_t elementSize;
	uint16_t maxNumOfElements;
	uint16_t headIndex;
	uint16_t numOfElements;
} queue_t;

void Queue_init(queue_t* this, uint16_t numOfElements, size_t elementSize);
bool Queue_push(queue_t* this, uint8_t *element);
bool Queue_pop(queue_t* this, uint8_t *element);
bool Queue_isEmpty(queue_t* this);
bool Queue_isFull(queue_t* this);
void Queue_clear(queue_t* this);
