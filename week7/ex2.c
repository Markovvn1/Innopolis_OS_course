#include <stdio.h>
#include <stdlib.h>

int read_N(int* N)
{
	printf("N = ");
	if (scanf("%d", N) != 1 || *N <= 0)
	{
		printf("Incorrect N");
		return -1;
	}
	return 0;
}

int main()
{
	int N;
	if (read_N(&N) != 0) return 0;

	int* mem = malloc(sizeof(int) * N);

	for (int i = 0; i < N; i++) mem[i] = i;
	for (int i = 0; i < N; i++) printf("%d ", mem[i]);
	printf("\n");
	free(mem);

	return 0;
}