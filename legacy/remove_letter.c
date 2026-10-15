#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int c, targ;

    if(argc != 2) {
        fprintf(stderr, "example of work: \"./prog 32\"\n");
        return 1;
    }

    targ = atoi(argv[1]);
    while((c = getchar()) != EOF)
        if(c != targ)
            putchar(c);
    return 0;
}
