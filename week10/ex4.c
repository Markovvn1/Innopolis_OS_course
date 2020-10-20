#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	DIR* dirp = opendir("tmp");
	if (dirp == NULL)
	{
		printf("Fatal error: Cannot open directiory\n");
		return 1;
	}

	uint list_len = 1;
	struct dirent* list = malloc(list_len * sizeof(struct dirent));

	for (int i = 0;; i++)
	{
		if (i == list_len)
		{
			list_len *= 2;
			list = realloc(list, list_len * sizeof(struct dirent));
		}

		struct dirent* temp = readdir(dirp);
		if (temp == NULL)
		{
			list_len = i;
			list = realloc(list, list_len * sizeof(struct dirent));
			break;
		}

		memcpy(list + i, temp, sizeof(struct dirent));
	}

	closedir(dirp);



	int used[list_len];
	for (int i = 0; i < list_len; i++) used[i] = 0;

	for (int i = 0; i < list_len; i++)
	{
		if (used[i]) continue;
		int count = 1;
		for (int j = i + 1; j < list_len; j++)
		{
			if (list[i].d_ino != list[j].d_ino) continue;
			count++;
			used[j] = 1;
		}

		if (count <= 1) continue;
		printf("Detected files (%d) with the same i-node (#%ld): ", count, list[i].d_ino);
		for (int j = i; j < list_len; j++)
		{
			if (list[i].d_ino != list[j].d_ino) continue;
			printf("%s ", list[j].d_name);
		}
		printf("\n");
	}

	free(list);
}