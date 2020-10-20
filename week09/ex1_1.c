#include <stdlib.h>
#include <stdio.h>

/*
This program read list of ints (from stdin) and separate it into rows.
Each row (except the last one) contain exactly ITEMS_PER_ROW elements.

For example:
	if input is:
		1 2 3 4 5 6 7 and ITEMS_PER_ROW = 2
	then output will be:
		1 2
		3 4
		5 6
		7
*/

// To run use: cat ex1.input.txt | ./ex1_1 2
int main(int argc, char** argv)
{
	if (argc != 2)
	{
		printf("Using: %s ITEMS_PER_ROW\n", argv[0]);
		return 0;
	}

	int ITEMS_PER_ROW = atoi(argv[1]);
	int temp;
	for (int i = 0; scanf("%d", &temp) == 1; i++)
		if ((i + 1) % ITEMS_PER_ROW == 0)
			printf("%d\n", temp);
		else
			printf("%d ", temp);

	return 0;
}