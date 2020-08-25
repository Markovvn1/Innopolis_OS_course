#include <stdlib.h>
#include <time.h>
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
	printf("Usage: %s NUMBER [TYPE]\n", p);
	printf("    TYPE can be:\n");
	printf("      0 - right side up triangle\n");
	printf("      1 - another triangle\n");
	printf("      2 - one more triangle\n");
	printf("      3 - fill square\n");
	printf("      4 - random fill\n");
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

char right_side_up_triangle_mask(int x, int y, int w, int h)
{
	return (x + y >= h - 1 && x - y < h) ? '*' : ' ';
}

char another_triangle_mask(int x, int y, int w, int h)
{
	return (x - y <= 0) ? '*' : ' ';
}

char one_more_triangle_mask(int x, int y, int w, int h)
{
	return (x - y <= 0 && x + y < h) ? '*' : ' ';
}

char fill_square_mask(int x, int y, int w, int h)
{
	return '*';
}

char random_fill_mask(int x, int y, int w, int h)
{
	return (rand() % 2) ? '*' : ' ';
}




int main(int argc, char** argv)
{
	srand(time(NULL));

	if (argc != 2 && argc != 3)
	{
		print_help(argv[0]);
		return 0;
	}

	if (!isposnumber(argv[1]) || (argc == 3 && !isposnumber(argv[2])))
	{
		print_help(argv[0]);
		return 0;
	}

	int n = atoi(argv[1]);
	int type = argc == 3 ? atoi(argv[2]) : 0;

	switch (type)
	{
		case 0:
			draw_mask(right_side_up_triangle_mask, 2*n-1, n);
			break;
		case 1:
			draw_mask(another_triangle_mask, n, n);
			break;
		case 2:
			draw_mask(one_more_triangle_mask, n, 2*n-1);
			break;
		case 3:
			draw_mask(fill_square_mask, n, n);
			break;
		case 4:
			draw_mask(random_fill_mask, n, n);
			break;
		default:
			print_help(argv[0]);
			return 0;
	}
	
	return 0;
}