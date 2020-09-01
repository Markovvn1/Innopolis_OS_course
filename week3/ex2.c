#include <stdio.h>

#define bool int
#define false 0
#define true !false

int bubble_sort(int* begin, int* end)
{
	if (begin > end) return -1;

	bool swapped = true;
	while (swapped)
	{
		swapped = false;
		for (int* t = begin; t < end; t++)
			if (t[0] > t[1])
			{
				int temp = t[1];
				t[1] = t[0];
				t[0] = temp;
				swapped = true;
			}
	}

	return 0;
}

int main()
{
	int arr[] = {5, 4, 2, 3, 1};
	const int N = sizeof(arr) / sizeof(int);

	for (int i = 0; i < N; i++)
		printf("%d ", arr[i]);
	printf("\n");

	bubble_sort(arr, arr + N);

	for (int i = 0; i < N; i++)
		printf("%d ", arr[i]);
	printf("\n");

	return 0;
}