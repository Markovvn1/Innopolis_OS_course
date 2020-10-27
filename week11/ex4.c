#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#define SRC_FILE_NAME "ex1.txt"
#define DEST_FILE_NAME "ex1.memcpy.txt"


char* my_fopen(const char* file_name, int flags, int* length)
{
	int fd = open(file_name, flags);
	if (fd < 0) return NULL;

	struct stat buff;
	int err = stat(file_name, &buff);
	if (err < 0) return NULL;
	*length = buff.st_size;

	int mmap_flags = 0;
	if (flags == O_RDONLY) mmap_flags = PROT_READ;
	if (flags == O_RDWR) mmap_flags = PROT_READ | PROT_WRITE;
	if (flags == O_WRONLY) mmap_flags = PROT_WRITE;

	char* ptr = mmap(NULL, buff.st_size, mmap_flags, MAP_SHARED, fd, 0);
	if (ptr == MAP_FAILED) return NULL;

	close(fd);

	return ptr;
}

int my_fclose(char* ptr, int length)
{
	if (ptr == NULL) return 0;

	int err = munmap(ptr, length);
	if (err != 0) return 1;
	
	return 0;
}

int main()
{
	int file1_len = 0;
	char* file1 = my_fopen(SRC_FILE_NAME, O_RDONLY, &file1_len);
	if (file1 == NULL)
	{
		printf("Could not open the file\n");
		return 1;
	}

	truncate(DEST_FILE_NAME, file1_len); // file should already exist
	int file2_len = 0;
	char* file2 = my_fopen(DEST_FILE_NAME, O_RDWR, &file2_len);
	if (file2 == NULL)
	{
		printf("Could not open the file\n");
		return 1;
	}

	memcpy(file2, file1, file1_len < file2_len ? file1_len : file2_len);

	my_fclose(file1, file1_len);
	my_fclose(file2, file2_len);

	return 0;
}