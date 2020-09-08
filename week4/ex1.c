#include <stdio.h>
#include <unistd.h>

int main()
{
    int n = 0;

    if (fork() != 0)
        printf("Hello from parent [%d - %d]\n", getpid(), n);
    else
        printf("Hello from child [%d - %d]\n", getpid(), n);

    return 0;
}