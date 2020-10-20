#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int read_command(char** line, size_t* len)
{
	printf(">>> ");

	size_t zero = 0;
	*len = getline(line, &zero, stdin);
	if (*len != 0 && (*line)[*len-1] == '\n')
	{
		(*len)--;
		(*line)[*len] = 0;
		return 0;
	}
	return *len != 0;
}

int main()
{
	printf("Super shell v0.0.0.0.0.0.00001\n");
	printf("Hint: to run process in background put & in the end of line\n");

	char* line;
	size_t len;

	while (1)
	{
		int res = read_command(&line, &len);
		if (res != 0) break;
		if (!strcmp(line, "exit")) break;
		system(line);
		free(line);
	}
	printf("\n");

	return 0;
}