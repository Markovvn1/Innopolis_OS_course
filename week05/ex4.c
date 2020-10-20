#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define C_CLEAR "\e[0m"
#define C_WHITE "\e[1;37m"
#define C_BOLD_BLUE "\e[1;34m"

uint gcd(uint a, uint b)
{
	while (a != 0 && b != 0)
		if (a > b)
			a = a % b;
		else
			b = b % a;

	return a + b;
}

uint lcm(uint a, uint b) { return a / gcd(a, b) * b; }

int solver(uint* input, uint len, uint* output, uint (*func)(uint, uint))
{
	/*
	Use function 'func' to calculate the answer.

	Args:
		input: pointer to array of integers
		len: length of input array
		output: pointer to integer for answer (answer will be puted there)
		func: function which will be used for calculate answer. Function have
				to satisfy following properties:
					- answer = input[0], if len == 1
					- answer = func(func(...(func(input[0], input[1]), ...), input[len-2]), input[len-1])

	Result:
		0 - work was done
		1 - imposible to complete the task (len == 0)
	*/
	if (len == 0) return 1;

	uint answer = input[0];
	for (int i = 1; i < len; i++)
		answer = func(answer, input[i]);

	*output = answer;
	return 0;
}

struct multi_thr_solver_worker_args
{
	/* See 'int worker(..)' for more details */
	uint* input;
	uint len;
	uint* output;
	uint (*func)(uint, uint);
};

void* multi_thr_solver_worker(void* args)
{
	struct multi_thr_solver_worker_args* p = ((struct multi_thr_solver_worker_args*)args);
	// printf("[DEBUG]: thread was created. input: %p, len: %d\n", p->input, p->len);
	int* res = malloc(sizeof(int));
	*res = solver(p->input, p->len, p->output, p->func);
	free(args);
	return res;
}

uint multi_thr_solver(int n, uint* input, uint len, uint* output, uint (*func)(uint, uint))
{
	/*
	The same as in 'int worker(..)' but for 'n' threads

	Args:
		n: max amount of threads which will be used

	Return:
		0: ok
		1: impossible to solve (len == 0)
		2: problems with threads (impossible to create n threads)
	*/

	if (n < 1) return 2;
	if (n == 1) return solver(input, len, output, func); // there are no reasons to create another thread
	if (n * 2 > len) n = len / 2; // Use more than this threads are useless

	uint* temp_outputs = malloc(sizeof(uint) * n); // for storing resilts of threads
	pthread_t* thread_ids = malloc(sizeof(pthread_t) * n);
	for (int i = 0; i < n; i++) thread_ids[i] = 0;

	int status = 0;

	uint input_shift = 0;
	// create threads and give work for them
	for (int i = 0; i < n; i++)
	{
		struct multi_thr_solver_worker_args* args = malloc(sizeof(struct multi_thr_solver_worker_args));
		args->input = input + input_shift;
		args->len = (len * (i + 1) / n) - input_shift;
		input_shift += args->len;
		args->func = func;
		args->output = temp_outputs + i;
		if (pthread_create(thread_ids+i, NULL, multi_thr_solver_worker, args))
		{
			status = 2;
			break;
		}
	}

	for (int i = 0; i < n; i++)
	{
		if (!thread_ids[i]) continue;

		int* worker_status;
		pthread_join(thread_ids[i], (void**)&worker_status);
		if (status == 0) status = *worker_status;
		free(worker_status);
	}

	if (status == 0)
		status = solver(temp_outputs, n, output, func);

	free(temp_outputs);
	free(thread_ids);

	return status;
}

void printHelp(char* name)
{
	printf("Use: %s FILE_NAME N", name);
}

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		printHelp(argv[0]);
		return 0;
	}

	int n = atoi(argv[2]);


	// Read data from given file
	FILE* file = fopen(argv[1], "r");
	int output, len = 0;
	while (fscanf(file, "%d", &output) != EOF) len++;
	fseek(file, SEEK_SET, 0);
	uint* input = malloc(sizeof(uint) * len);
	for (int i = 0; i < len; i++)
		fscanf(file, "%d", input + i);
	fclose(file);

	printf("%sMulti-threaded%s solution for computing the %sleast common multiple%s\n", C_WHITE, C_CLEAR, C_BOLD_BLUE, C_CLEAR);

	int status = multi_thr_solver(n, input, len, &output, lcm);

	if (status == 0)
		printf("Answer: %d\n", output);
	else
		printf("Exit with non-zero status code (%d)\n", status);

	free(input);
}