#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
	printf("Input string you want to reverse: ");

	char* line = NULL;
	size_t zero = 0;
	ssize_t line_len = getline(&line, &zero, stdin);
	if (line_len != 0 && line[line_len-1] == '\n')
	{
		line_len--;
		line[line_len] = 0;
	}

	for (int i = line_len-1; i >= 0; i--)
		printf("%c", line[i]);
	printf("\n");

	return 0;
}