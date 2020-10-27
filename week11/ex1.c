#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#define FILE_NAME "ex1.txt"

int main()
{
	truncate(FILE_NAME, 18);

	int fd = open(FILE_NAME, O_RDWR);
	if (fd < 0)
	{
		printf("Could not open the file\n");
		return 1;
	}

	struct stat buff;
	int err = stat(FILE_NAME, &buff);
	if (err < 0)
	{
		printf("Could not open the file\n");
		return 1;
	}

	char* ptr = mmap(NULL, buff.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (ptr == MAP_FAILED)
	{
		printf("mmap failed\n");
		return 1;
	}

	close(fd);  // we do not need this file, we have already mapped it to memory

	// working with mapped memory
	const char* source_text = "This is a nice day";
	memcpy(ptr, source_text, 18);
	



	err = munmap(ptr, buff.st_size);
	if (err != 0)
	{
		printf("munmap failed\n");
		return 1;
	}

	return 0;
}