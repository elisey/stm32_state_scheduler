#include "queue.h"
#include "debug.h"

void Queue_test()
{
	queue_t testQueue;
	Queue_init(&testQueue, 5, sizeof(int));

	assert( Queue_isEmpty(&testQueue) == true );
	assert( Queue_isFull(&testQueue) == false );

	int a = 34;
	int b = 35;
	int c = 36;
	int d = 37;
	int e = 38;

	assert( Queue_push( &testQueue, (uint8_t*)&a ) == true);
	assert( Queue_push( &testQueue, (uint8_t*)&b ) == true);
	assert( Queue_push( &testQueue, (uint8_t*)&c ) == true);

	assert( Queue_isEmpty(&testQueue) == false );
	assert( Queue_isFull(&testQueue) == false );

	assert( Queue_push( &testQueue, (uint8_t*)&d ) == true);
	assert( Queue_push( &testQueue, (uint8_t*)&e ) == true);
	assert( Queue_push( &testQueue, (uint8_t*)&e ) == false);

	assert( Queue_isEmpty(&testQueue) == false );
	assert( Queue_isFull(&testQueue) == true );

	int read;

	assert( Queue_pop( &testQueue, (uint8_t*)&read ) == true);
	assert(read == 34);

	assert( Queue_pop( &testQueue, (uint8_t*)&read ) == true);
	assert(read == 35);

	assert( Queue_pop( &testQueue, (uint8_t*)&read ) == true);
	assert(read == 36);

	assert( Queue_pop( &testQueue, (uint8_t*)&read ) == true);
	assert(read == 37);

	assert( Queue_pop( &testQueue, (uint8_t*)&read ) == true);
	assert(read == 38);

	assert( Queue_pop( &testQueue, (uint8_t*)&read ) == false);
	assert(read == 38);

	assert( Queue_isEmpty(&testQueue) == true );
	assert( Queue_isFull(&testQueue) == false );


	assert( Queue_push( &testQueue, (uint8_t*)&a ) == true);
	assert( Queue_push( &testQueue, (uint8_t*)&b ) == true);
	assert( Queue_push( &testQueue, (uint8_t*)&c ) == true);

	Queue_clear(&testQueue);

	assert( Queue_isEmpty(&testQueue) == true );
	assert( Queue_isFull(&testQueue) == false );
}
