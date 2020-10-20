// To run use: cat ex1.input.txt | ./ex1_1 PAGES_PER_TICK | ./ex1_2 PAGE_FRAMES
// To run use: cat ex1.input.txt | ./ex1_1 1 | ./ex1_2 10
// To run use: cat ex1.input.txt | ./ex1_1 1 | ./ex1_2 10
// To run use: cat ex1.input.txt | ./ex1_1 1 | ./ex1_2 10
// To run use: cat ex1.input.txt | ./ex1_1 1 | ./ex1_2 10
// To run use: cat ex1.input.txt | ./ex1_1 1 | ./ex1_2 10
// To run use: cat ex1.input.txt | ./ex1_1 1 | ./ex1_2 10
// To run use: cat ex1.input.txt | ./ex1_1 1 | ./ex1_2 10

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

struct PageTable
{
	int page_frames;
	int* P2V;
	int next_empty;
	uint* counter; // ageing algorithm
};

void PageTable_init(struct PageTable* pt, int page_frames)
{
	pt->page_frames = page_frames;
	pt->next_empty = 0;
	pt->P2V = malloc(sizeof(int) * page_frames);
	pt->counter = malloc(sizeof(uint) * page_frames);
	for (int i = 0; i < page_frames; i++)
	{
		pt->P2V[i] = -1;
		pt->counter[i] = 0;
	}
}

void PageTable_deinit(struct PageTable* pt)
{
	free(pt->P2V);
	free(pt->counter);
}

/*
	Convert virtual page number to physical page number
*/
int PageTable_V2P(struct PageTable* pt, int virtual_page)
{
	for (int i = 0; i < pt->page_frames; i++)
		if (pt->P2V[i] == virtual_page) return i;
	return -1;
}

/*
	Chose and remove one page
	Returns:
		physical page number which is free now
*/
int PageTable_remove_one(struct PageTable* pt)
{
	if (pt->next_empty < pt->page_frames)
	{
		pt->next_empty++;
		return pt->next_empty-1;
	}

	uint min_v = pt->counter[0];
	int min_id = 0;
	for (int i = 1; i < pt->page_frames; i++)
	{
		if (pt->counter[i] > min_v) continue;
		min_id = i;
		min_v = pt->counter[i];
	}

	// Move page to disk (not needed)
	// Do some other things (not needed)

	return min_id;
}

/*
	Simulate next tick of aging algorithm.
	Args:
		pages (int*): list of pages witch need to be load in next tick

	Return:
		hits (int): amount of pages which already was in memory
*/
int PageTable_next_tick(struct PageTable* pt, int* pages, int pages_len)
{
	int hits = 0;

	for (int i = 0; i < pt->page_frames; i++)
		pt->counter[i] >>= 1;

	// Run all pages which are already in table
	for (int i = 0; i < pages_len; i++)
	{
		int ppn = PageTable_V2P(pt, pages[i]);
		if (ppn < 0) continue;
		pages[i] = -1; // used
		hits++;
		pt->counter[ppn] |= (1 << 31);
	}

	// Load pages which was not in table
	for (int i = 0; i < pages_len; i++)
	{
		if (pages[i] < 0) continue;
		if (PageTable_V2P(pt, pages[i]) >= 0)
		{
			hits++;
			continue;
		}

		// Remove old page
		int id = PageTable_remove_one(pt);

		// Load new page
		pt->P2V[id] = pages[i];
		pt->counter[id] = (1 << 31);
	}

	return hits;
}

void PageTable_print_state(struct PageTable* pt)
{
	printf("PageTable state (%d available pages):\n", pt->page_frames);
	for (int i = 0; i < pt->page_frames; i++)
		printf("\t%d\t%u\n", pt->P2V[i], pt->counter[i]);
}

int read_line(char** line, size_t* len)
{
	size_t zero = 0;
	*len = getline(line, &zero, stdin);
	if (*len != 0 && (*line)[*len-1] == '\n')
	{
		(*len)--;
		(*line)[*len] = 0;
		return 0;
	}
	return *len != 0;
}

// To run use: cat ex1.input.txt | ./ex1_1 PAGES_PER_TICK | ./ex1_2 PAGE_FRAMES
int main(int argc, char** argv)
{
	if (argc != 2 && argc != 3)
	{
		printf("Using: %s PAGE_FRAMES\n", argv[0]);
		return 0;
	}

	int PAGE_FRAMES = atoi(argv[1]);

	struct PageTable pt;
	PageTable_init(&pt, PAGE_FRAMES);
	// PageTable_print_state(&pt);

	char* line = NULL;
	ssize_t line_len = 0;

	int hits = 0, total = 0;
	
	while (read_line(&line, &line_len) == 0)
	{
		int pages_num = 1;
		for (int i = line_len-1; i >= 0; i--)
		{
			if (isspace(line[i])) pages_num++;
		}

		int pages[pages_num];
		int pages_i = 0;
		for (int i = line_len-1; i >= 0; i--)
		{
			if (!isspace(line[i])) continue;
			line[i] = 0;
			pages[pages_i] = atoi(line + (i+1));
			pages_i++;
		}
		pages[pages_i] = atoi(line);

		free(line);

		total += pages_num;
		hits += PageTable_next_tick(&pt, pages, pages_num);
		// PageTable_print_state(&pt);
	}

	// PageTable_print_state(&pt);
	// printf("hit_rate: %f\n", (float)(hits) / total);
	printf("hit/miss ratio: %d/%d = %f\n", hits, total - hits, (float)(hits) / (total - hits));

	PageTable_deinit(&pt);

	return 0;
}