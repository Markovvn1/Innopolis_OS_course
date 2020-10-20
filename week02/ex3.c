#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int isposnumber(char* str)
{
	for (int i = 0; str[i] != 0; i++)
		if (!isdigit(str[i])) return 0;

	return 1;
}

void print_help(char* p)
{
	printf("Usage: %s NUMBER\n", p);
}

void draw_mask(char (*mask)(int, int, int, int), int w, int h)
{
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			printf("%c", mask(x, y, w, h));
			if (x + 1 != w) printf(" ");
		}
		printf("\n");
	}
}

char right_side_up_triangle(int x, int y, int w, int h)
{
	if (x + y >= h - 1 && x - y < h) return '*';
	return ' ';
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		print_help(argv[0]);
		return 0;
	}

	if (!isposnumber(argv[1]))
	{
		print_help(argv[0]);
		return 0;
	}

	int n = atoi(argv[1]);
	
	draw_mask(right_side_up_triangle, 2*n-1, n);

	return 0;
}