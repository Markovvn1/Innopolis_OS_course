#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE* f = fopen("/dev/urandom", "r");

	char buff[20];
	fread(buff, sizeof(char), 20, f);

	fclose(f);

	// As far as I understand, we should generate only printable ASCII symbols. If no, uncoment next to line
	// for (int i = 0; i < 20; i++) printf("%c", buff[i]);
	// return 0;

	char convert[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
	int convert_len = sizeof(convert) / sizeof(char) - 1;

	unsigned char* ubuff = buff;

	int buff_i = 0;
	uint random = ubuff[buff_i]; buff_i++;
	for (int i = 0; i < 20; i++)
	{
		if (random < convert_len)
		{
			random = (random << 8) | ubuff[buff_i];
			buff_i++;
		}

		int r = random % convert_len;
		random /= convert_len;

		printf("%c", convert[r]);
	}

	return 0;
}