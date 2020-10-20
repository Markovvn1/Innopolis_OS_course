#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define C_CLEAR "\e[0m"
#define C_YELLOW "\e[0;33m"
#define C_BOLD_READ "\e[1;31m"
#define C_BOLD_BLUE "\e[1;34m"

// to determine if programs are still running
int thr_producer_amount = 0;
int thr_consumer_amount = 0;

int* buffer;
int buffer_len = 0;
int buffer_capacity = 0;

void buffer_put(int item)
{
	buffer[buffer_len] = item;
	buffer_len++;
}

int buffer_pop()
{
	int item = buffer[buffer_len];
	buffer_len--;
	return item;
}


int thr_producer_working = 1;
int thr_consumer_working = 1;

void thr_producer_wakeup() { thr_producer_working = 1; }
void thr_consumer_wakeup() { thr_consumer_working = 1; }

void* thr_producer(void* args)
{
	while (1)
	{
		thr_producer_amount++;  // I'm alive!

		if (buffer_len == buffer_capacity)
		{
			// sleep
			thr_producer_working = 0;
			while (!thr_producer_working);
		}

		int item = rand();  // gerenate new item
		buffer_put(item);

		if (buffer_len == 1) thr_consumer_wakeup();
	}

	return NULL;
}

void* thr_consumer(void* args)
{
	while (1)
	{
		thr_consumer_amount++;  // I'm alive!

		if (buffer_len == 0)
		{
			// sleep
			thr_consumer_working = 0;
			while (!thr_consumer_working);
		}

		int item = buffer_pop();

		if (buffer_len == buffer_capacity-1) thr_producer_wakeup();
	}	

	return NULL;
}

int main()
{
	printf("Demonstration of %sproducer-consumer problem%s\n", C_BOLD_READ, C_CLEAR);
	printf("%sPlease, wait...%s\n", C_YELLOW, C_CLEAR);

	buffer_capacity = 10;
	buffer = malloc(buffer_capacity * sizeof(int));

	pthread_t producer, consumer;
	pthread_create(&producer, NULL, thr_producer, NULL);
	pthread_create(&consumer, NULL, thr_consumer, NULL);

	int last_amount_of_work = -1;

	while (last_amount_of_work != thr_producer_amount + thr_consumer_amount)
	{
		last_amount_of_work = thr_producer_amount + thr_consumer_amount;
		sleep(1);
	}

	printf("Status of producer: %d\nStatus of consumer: %d\n", thr_producer_working, thr_consumer_working);

	printf("%sFatal race condition has occurred%s after %d steps of producer and %d steps of consumer\n", C_BOLD_BLUE, C_CLEAR, thr_producer_amount, thr_consumer_amount);

	printf("%sKilling%s all created threads\n", C_BOLD_READ, C_CLEAR);
	free(buffer);
	pthread_cancel(producer);
	pthread_cancel(consumer);
}