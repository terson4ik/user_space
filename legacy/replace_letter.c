#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    char target, sub;
    int c;

    if(argc != 3) {
        fprintf(stderr, "example of use \"./prog 32 48\"\n");
        return 1;
    }

    target = atoi(argv[1]);
    sub    = atoi(argv[2]);
    while((c = getchar()) != EOF)
        if(c == target)
            putchar(sub);
        else
            putchar(c);
    return 0;
}
