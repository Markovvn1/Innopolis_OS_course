#include <limits.h>
#include <stdio.h>
#include <float.h>

int main()
{

	int a = INT_MAX;
	float b = FLT_MAX;
	double c = DBL_MAX;

	printf("int (%lu): %d\n", sizeof(a), a);
	printf("float (%lu): %f\n", sizeof(b), b);
	printf("double (%lu): %f\n", sizeof(c), c);

	return 0;
}