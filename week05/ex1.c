#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void* thr_worker(void* args)
{
	int idx = *((int*)args);
	free(args);

	printf("I'm a worker #%d\n", idx+1);

	return NULL;
}

int main()
{
	int N = 0;
	printf("Input N: ");
	scanf("%d", &N);
	pthread_t* thr = malloc(sizeof(pthread_t) * N);

	for (int i = 0; i < N; i++)
	{
		int* args = malloc(sizeof(int));
		*args = i;
		if (pthread_create(thr + i, NULL, thr_worker, args))
			printf("Cannot create thread #%d\n", i+1);
		else
			printf("Thread #%d was created\n", i+1);

		pthread_join(thr[i], NULL);
	}

	// for (int i = 0; i < N; i++)
	// 	pthread_join(thr[i], NULL);

	free(thr);
}