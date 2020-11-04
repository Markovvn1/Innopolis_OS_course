#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024


int main(int args, char** argv)
{
	char** file_names = argv + 1;
	int file_count = args - 1;
	int append_mode = 0;

	if (args > 1 && strcmp(argv[1], "-a") == 0)
	{
		append_mode = 1;
		file_count--;
		file_names++;
	}

	FILE* files[file_count + 1];  // use gcc 7.5.0

	for (int i = 0; i < file_count; i++)
		files[i] = fopen(file_names[i], append_mode ? "a" : "w");
	files[file_count] = stdout;

	char* buffer[BUFFER_SIZE];
	while (!feof(stdin))
	{
		int len = fread(buffer, sizeof(char), BUFFER_SIZE, stdin);
		for (int i = 0; i <= file_count; i++)
			fwrite(buffer, sizeof(char), len, files[i]);
	}

	for (int i = 0; i < file_count; i++)
		fclose(files[i]);
	
	return 0;
}