#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define C_CLEAR "\e[0m"
#define C_WHITE "\e[1;37m"
#define C_BOLD_BLUE "\e[1;34m"

struct ProcessAttr
{
	int arrival_time;
	int burst_time;
};

int len_of_num(int x)
{
	if (x == 0) return 1;
	int i;
	for (i = 0; x != 0; i++) x /= 10;
	return i;
}

void print_char(char c, int N)
{
	for (int i = 0; i < N; i++)
		printf("%c", c);
}

/*
	Read input data from console.

	Args:
		attrs - pointer to array of struct ProcessAttr
	Returns:
		len - length of array. -1 is there is an error
*/
int read_from_console(struct ProcessAttr** attrs)
{
	if (attrs == NULL) return -1;

	printf("Number of process: ");
	int N;
	if (scanf("%d", &N) != 1 || N < 0) return -1;
	if (N == 0) return 0;
	printf("\n");

	struct ProcessAttr* res = (struct ProcessAttr*)malloc(sizeof(struct ProcessAttr) * N);

	int lenN = len_of_num(N) + 1;
	if (lenN < 4) lenN = 4;
	printf("%sPID", C_WHITE);
	print_char(' ', lenN - 3);
	printf("AT BT%s\n", C_CLEAR);

	for (int i = 0; i < N; i++)
	{
		printf("%d", i);
		print_char(' ', lenN - len_of_num(i));
		if (scanf("%d %d", &res[i].arrival_time, &res[i].burst_time) != 2 ||
			res[i].arrival_time < 0 || res[i].burst_time <= 0)
		{
			free(res);
			return -1;
		}
	}

	*attrs = res;
	return N;
}

struct SchedulerAttr
{
	int PID;
	int startTime;
	int duration;
};

int SJF_scheduler(int N, struct ProcessAttr* attrs, struct SchedulerAttr** sattr)
{
	if (sattr == NULL || N < 0) return -1;
	if (N == 0) return 0;

	int resN = N;
	struct SchedulerAttr* res = (struct SchedulerAttr*)malloc(sizeof(struct SchedulerAttr) * resN);

	int* used = malloc(sizeof(int) * N);
	for (int i = 0; i < N; i++) used[i] = 0;

	int rtime = 0;
	for (int i = 0; i < resN; i++)
	{
		int min_t = -1;
		for (int j = 0; j < N; j++)
		{
			if (used[j]) continue;
			if (min_t != -1 && attrs[j].arrival_time >= min_t) continue;
			min_t = attrs[j].arrival_time;
		}

		if (min_t > rtime) rtime = min_t;

		int min_dur, min_j = -1;
		for (int j = 0; j < N; j++)
		{
			if (used[j]) continue;
			if (attrs[j].arrival_time > rtime) continue;
			if (min_j != -1 && min_dur <= attrs[j].burst_time) continue;
			min_dur = attrs[j].burst_time;
			min_j = j;
		}

		used[min_j] = 1;

		res[i].PID = min_j;
		res[i].startTime = rtime;
		res[i].duration = attrs[min_j].burst_time;
		rtime += res[i].duration;
	}

	free(used);

	*sattr = res;

	return resN;
}

void print_stat(int N, struct ProcessAttr* proc, int scN, struct SchedulerAttr* sched)
{
	printf("%sPID\tAT\tBT\tCT\tTAT\tWT%s\n", C_WHITE, C_CLEAR);

	int sumTAT = 0, sumWT = 0;

	for (int i = 0; i < N; i++)
	{
		int PID = i, AT = proc[i].arrival_time, BT = proc[i].burst_time;
		int CT = 0;
		for (int j = scN-1; j >= 0; j--)
		{
			if (sched[j].PID != PID) continue;
			CT = sched[j].startTime + sched[j].duration;
			break;
		}

		int TAT = CT - AT;
		int WT = TAT - BT;

		printf("%d\t%d\t%d\t%d\t%d\t%d\n", PID, AT, BT, CT, TAT, WT);
		sumTAT += TAT;
		sumWT += WT;
	}

	printf("\n");
	printf("Amount of context switch: %d\n", scN-1);
	printf("Average TAT: %f\n", (float)sumTAT / N);
	printf("Average WT: %f\n", (float)sumWT / N);
}

int main()
{
	printf("%sSimple simulator of SJF algorithm%s\n", C_BOLD_BLUE, C_CLEAR);

	struct ProcessAttr* proc;
	int N = read_from_console(&proc);
	if (N < 1)
	{
		printf("Incorrect input format\n");
		return 0;
	}

	struct SchedulerAttr* sched;
	int scN = SJF_scheduler(N, proc, &sched);
	if (N < 1)
	{
		printf("Something wrong during calculation\n");
		return 0;
	}

	printf("\n");

	print_stat(N, proc, scN, sched);

	free(sched);
	free(proc);

	return 0;
}