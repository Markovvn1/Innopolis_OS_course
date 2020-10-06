#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/resource.h>

#define C_CLEAR "\e[0m"
#define C_BOLD_BLUE "\e[1;34m"


// I just want to see what others fields are
void rusage_print(struct rusage* usage)
{
	printf("struct rusage:\n");
	printf("\tuser CPU time used:\n");
	printf("\t\tseconds: %ld\n", usage->ru_utime.tv_sec);
	printf("\t\tmicroseconds: %ld\n", usage->ru_utime.tv_usec);
	printf("\tsystem CPU time used:\n");
	printf("\t\tseconds: %ld\n", usage->ru_stime.tv_sec);
	printf("\t\tmicroseconds: %ld\n", usage->ru_stime.tv_usec);

	printf("\t%smaximum resident set size: %ld%s\n", C_BOLD_BLUE, usage->ru_maxrss, C_CLEAR);
	printf("\tintegral shared memory size: %ld\n", usage->ru_ixrss);
	printf("\tintegral unshared data size: %ld\n", usage->ru_idrss);
	printf("\tintegral unshared stack size: %ld\n", usage->ru_isrss);
	printf("\tpage reclaims (soft page faults): %ld\n", usage->ru_minflt);
	printf("\tpage faults (hard page faults): %ld\n", usage->ru_majflt);
	printf("\tswaps: %ld\n", usage->ru_nswap);
	printf("\tblock input operations: %ld\n", usage->ru_inblock);
	printf("\tblock output operations: %ld\n", usage->ru_oublock);
	printf("\tIPC messages sent: %ld\n", usage->ru_msgsnd);
	printf("\tIPC messages received: %ld\n", usage->ru_msgrcv);
	printf("\tsignals received: %ld\n", usage->ru_nsignals);
	printf("\tvoluntary context switches: %ld\n", usage->ru_nvcsw);
	printf("\tinvoluntary context switches: %ld\n", usage->ru_nivcsw);
}

int main()
{
	void* a[10];

	for (int i = 0; i < 10; i++)
	{
		a[i] = calloc(10, 1024 * 1024);
		// we need memset call because otherwise compiler will optimize all program
		memset(a[i], 0, 10 * 1024 * 1024);

		struct rusage usage;
		getrusage(RUSAGE_SELF, &usage);
		rusage_print(&usage);
		printf("\n\n");

		sleep(1);
	}

	for (int i = 0; i < 10; i++)
		free(a[i]);

	return 0;
}