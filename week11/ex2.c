#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define FILE_NAME "ex2.txt"

int main()
{
	// Actually it is don't needed, because default mode of stdout is _IOLBF
	char buff[128];
	setvbuf(stdout, buff, _IOLBF, 128);

	printf("H");
	sleep(1);
	printf("e");
	sleep(1);
	printf("l");
	sleep(1);
	printf("l");
	sleep(1);
	printf("o");
	sleep(1);

	return 0;
}