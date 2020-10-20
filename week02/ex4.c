#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void swap(int* a, int* b)
{
	int t = *a;
	*a = *b;
	*b = t;
}

int main()
{
	printf("Input two numbers you want to swap: ");

	int a, b;
	scanf("%d %d", &a, &b);
	swap(&a, &b);
	printf("I had swapped it: %d %d\n", a, b);

	return 0;
}