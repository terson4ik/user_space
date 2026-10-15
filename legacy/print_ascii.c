#include <stdio.h>

#define MIN_CHR ' '
#define MAX_CHR '~'

int main()
{
    int i, len;

    printf("DEC\n");
    putchar(' ');
    len = 0;
    for(i = 30; i <= 120; i += 10)
        if(i < 100)
            len += printf("%3d", i);
        else
            len += printf("%4d", i);
    putchar('\n');
    putchar(' ');
    while(len) {
        putchar('-');
        --len;
    }
    putchar('\n');
    
    for(i = 0; i <= 9; ++i) {
        int j;
        printf("%d:", i);
        for(j = 30; j <= 120; j += 10) {
            if(j == 30){
                if(j + i < MIN_CHR)
                    printf(" X");
                else
                    printf("%2c", j + i);
            } else {
                if(j + i > MAX_CHR)
                    printf("   X");
                else if(j > 100)
                    printf("%4c", j + i);
                else
                    printf("%3c", j + i);
            }
        }
        putchar('\n');
    }



    printf("\nHEX\n");
    printf("  |");
    for(i = 0; i <= 9; ++i)
        printf(" .%d", i);
    for(i = 'A'; i <= 'F'; ++i)
        printf(" .%c", i);
    putchar('\n');
    printf("  |");
    for(i = 0; i <= 15; ++i)
        printf("---");
    putchar('\n');
    for(i = 2; i <= 7; ++i) {
        int j;
        printf("%d.|", i);
        for(j = 0; j <= 15; ++j)
            printf("  %c", i * 16 + j);
        putchar('\n');
    }

    return 0;
}
