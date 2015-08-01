#include "queue.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static uint16_t calculateTailIndex(queue_t *this);

void Queue_init(queue_t* this, uint16_t numOfElements, size_t elementSize)
{
	this->elementSize = elementSize;
	this->maxNumOfElements = numOfElements;

	this->ptrBuffer = calloc(numOfElements, elementSize);
	assert(this->ptrBuffer != NULL);

	Queue_clear(this);
}

bool Queue_push(queue_t* this, uint8_t *element)
{
	if ( Queue_isFull(this) == true )	{
		return (false);
	}

	memcpy(
			(void*)&(this->ptrBuffer[ this->elementSize * this->headIndex ]),
			(const void*)( element ),
			this->elementSize);

	this->headIndex++;
	if (this->headIndex >= this->maxNumOfElements)	{
		this->headIndex = 0;
	}
	this->numOfElements++;
	return true;
}

bool Queue_pop(queue_t* this, uint8_t *element)
{
	if ( Queue_isEmpty(this) == true )	{
		return (false);
	}

	uint16_t tail = calculateTailIndex(this);

	memcpy(
			(void*)(element),
			(const void*)&(this->ptrBuffer[ this->elementSize * tail ] ),
			this->elementSize);

	this->numOfElements--;
	return true;
}

bool Queue_isEmpty(queue_t* this)
{
	if (this->numOfElements == 0)	{
		return true;
	}
	return false;
}

bool Queue_isFull(queue_t* this)
{
	if (this->numOfElements == this->maxNumOfElements)	{
		return true;
	}
	return false;
}

void Queue_clear(queue_t* this)
{
	this->headIndex = 0;
	this->numOfElements = 0;
}

static uint16_t calculateTailIndex(queue_t *this)
{
	uint16_t head = this->headIndex;
	uint16_t numOfElements = this->numOfElements;

	if (head >= numOfElements)	{
		return (head - numOfElements);
	}
	else	{
		return ( this->maxNumOfElements - ( numOfElements - head ) );
	}
}
