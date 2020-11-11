#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

int read_vector(int** vector, size_t* len, FILE* fp, int skip_lines)
{
	if (vector == NULL) return 1;

	char* line = NULL;
	size_t line_capacity = 0;
	size_t line_len = 0;

	int is_last_space;
	size_t l = 0;

	do
	{
		line_len = getline(&line, &line_capacity, fp);
		if (line[line_len - 1] == 0)
			line_len--;

		// calculate len
		is_last_space = isspace(line[line_len - 1]);
		l = is_last_space == 0;
		for (int i = line_len - 2; i >= 0; i--)
		{
			int is_space = isspace(line[i]);
			if (is_space) line[i] = 0;
			if (is_last_space == is_space) continue;
			is_last_space = is_space;
			l += is_space == 0;
		}
	}
	while (!feof(fp) && (l == 0) && skip_lines);

	if (l <= 0)
	{
		if (len != NULL) *len = 0;
		*vector = NULL;
		free(line);
		return 0;
	}

	int vector_i = l - 1;
	int* res = malloc(sizeof(int) * l);

	is_last_space = line[line_len - 1] == 0;
	for (int i = line_len - 2; i >= 0; i--)
	{
		int is_space = line[i] == 0;
		if (is_last_space == is_space) continue;
		is_last_space = is_space;
		if (!is_space) continue;

		res[vector_i] = atoi(line + (i+1));
		vector_i--;
	}

	if (line[0] != 0) res[vector_i] = atoi(line);

	free(line);


	*vector = res;
	if (len != NULL) *len = l;
	return 0;
}

int read_matrix(int*** matrix, size_t* height, size_t* width, FILE* fp, int skip_lines)
{
	if (matrix == NULL) return 1;

	size_t res_capacity = 4;
	int** res = malloc(sizeof(int*) * res_capacity);

	size_t h = 0, w = 0;

	while (!feof(fp))
	{
		size_t lw;
		if (h >= res_capacity)
		{
			res_capacity *= 2;
			res = realloc(res, sizeof(int*) * res_capacity);
		}
		read_vector(res + h, &lw, fp, skip_lines && (h == 0));
		if (h == 0) w = lw;
		if (lw == 0) break;
		if (w != lw) return 1;
		h++;
	}

	res = realloc(res, sizeof(int*) * h);
	*matrix = res;
	if (height != NULL) *height = h;
	if (width != NULL) *width = w;
	return 0;
}

int print_vector(int* vector, size_t len)
{
	for (int i = 0; i < len; i++)
		if (i == 0)
			printf("%d", vector[i]);
		else
			printf(" %d", vector[i]);
	printf("\n");
}

int print_matrix(int** matrix, size_t h, size_t w)
{
	for (int i = 0; i < h; i++)
		print_vector(matrix[i], w);
}


int vector_not_less(int* A, int* B, int n)
{
	for (int i = 0; i < n; i++)
		if (A[i] < B[i]) return 0;
	return 1;
}

// C = A + B
int vector_sum(int* A, int* B, int* C, int n)
{
	for (int i = 0; i < n; i++)
		C[i] = A[i] + B[i];
	return 0;
}


int main(int argc, char** argv)
{
	if (argc != 2)
	{
		printf("Using: %s FILE_NAME\n", argv[0]);
		return 1;
	}

	FILE* f = fopen(argv[1], "r");

	size_t m, n;
	size_t tm, tn;

	int* A;
	int** C;
	int** R;

	read_vector(&A, &m, f, 1);
	free(A); // I do not see any reason to use vector E...
	read_vector(&A, &tm, f, 1);
	if (tm != m) return 1;  // need to free memory, but I'm too lazy...
	read_matrix(&C, &n, &tm, f, 1);
	if (tm != m) return 1;
	read_matrix(&R, &tn, &tm, f, 1);
	if (tm != m || tn != n) return 1;

	// print_vector(A, m);
	// printf("\n");
	// print_matrix(C, n, m);
	// printf("\n");
	// print_matrix(R, n, m);


	int used[n];
	for (int i = 0; i < n; i++) used[i] = 0;

	// kind of greedy algorithm, but O(m*n^2)
	for (int i = 0; i < n; i++)
	{
		int at_least_one = 0;

		// find first best process
		for (int j = 0; j < n; j++)
		{
			if (used[j]) continue;
			if (!vector_not_less(A, R[j], m)) continue;

			vector_sum(A, C[j], A, m);
			used[j] = 1;
			at_least_one = 1;
		}

		if (!at_least_one) break;
	}

	int not_used_count = 0;
	for (int i = 0; i < n; i++)
		not_used_count += !used[i];

	if (not_used_count == 0)
		printf("Deadlock can be avoided (deadlock not found)\n");
	else
	{
		printf("There is a deadlock. Imposible to run following process:\n");
		for (int i = 0; i < n; i++)
			if (!used[i]) printf("%d ", i+1);
		printf("\n");
	}


	return 0;
}