#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*
	Exercise 2

procs -----------memory---------- ---swap-- -----io---- -system-- ------cpu-----
 r  b   swpd   free   buff  cache   si   so    bi    bo   in   cs us sy id wa st
 5  0 540672 1261044  92256 874284    3   29   129    81  603 1272 31 19 48  2  0
 0  0 540672 1249892  92256 874084    0    0     0     0 6587 13191 28 17 54  0  0
 0  0 540672 1239008  92288 873996    0    0     0   196 6998 13876 27 15 49  8  0
 4  0 540672 1229944  92292 872864    0    0     4   352 6318 12093 30 18 51  0  0
 0  0 540672 1220096  92292 872900    0    0     0     0 6456 13188 26 16 58  0  0
 2  0 540672 1209816  92292 872900    0    0     0     0 6531 13129 23 17 61  0  0
 0  0 540672 1199192  92308 873028    0    0     0   624 5350 10122 23 14 59  4  0
 2  0 540672 1192356  92308 872896    0    0    12     0 4839 9876 42 18 39  1  0
 0  0 540672 1187696  92468 872532    0    0   200     0 7207 15098 58 18 24  0  0
 1  0 540672 1178452  92468 872588    0    0     0     0 6527 13514 42 17 41  0  0
 3  0 540672 1280236  92468 872588    0    0     0     0 5817 11234 21 13 66  0  0

	So, as you can see, free memory decreases.
	Column si and so does not changes, because there are no reason to do it:
		I have enough RAM on my computer to not swap this program.
*/


/*
	Exercise 3

	htop is the same as top, but more user friendly. So, let's use htop:

	Before run ./ex2:
  1  [||||||||||||||||||||||||||||                            43.8%]   Tasks: 163, 426 thr; 1 running
  2  [|||||||||||||||||||||||||||                             43.2%]   Load average: 1.99 2.73 2.47 
  Mem[||||||||||||||||||||||||||||||||||                1.19G/3.84G]   Uptime: 04:31:12
  Swp[|||||||||                                          528M/3.81G]

	Before the end of the program ./ex2:
  1  [|||||||||||||||                                         23.5%]   Tasks: 164, 426 thr; 1 running
  2  [|||||||||||||||||||||                                   32.3%]   Load average: 1.70 2.25 2.32 
  Mem[||||||||||||||||||||||||||||||||||||              1.29G/3.84G]   Uptime: 04:31:21
  Swp[|||||||||                                          528M/3.81G]

	You probably won't belive it, but used RAM memory increases! I think it is
	because my program allocate 10MB each second. I don't know what I can add more
*/

int main()
{
	void* a[10];

	for (int i = 0; i < 10; i++)
	{
		a[i] = calloc(10, 1024 * 1024);
		// we need memset call because otherwise compiler will optimize all program
		memset(a[i], 0, 10 * 1024 * 1024);
		sleep(1);
	}

	for (int i = 0; i < 10; i++)
		free(a[i]);

	return 0;
}