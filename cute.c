/* suckless style CUTE cat */
#include <stdio.h>
#include <unistd.h>

enum delays {
    DELAY_SLOW  = 800000,
    DELAY_NORM  = 70000,
    DELAY_HIGH = 9999,
    DELAY_SUPER = 3000,
    DELAY_ERROR = -1
};
static void cute_file_cpy(FILE *dst, FILE *src, enum delays delay);
static enum delays get_delay_flag(char c);

#define HELP_MSG "Usage: cute [OPTION] FILE...\n" \
                 "OPTIONS:\n" \
                 "`l` to sLOW speed\n" \
                 "`n` to NORMAL speed\n" \
                 "`h` to HIGH speed\n" \
                 "`s` to SUPER speed\n"

int main(int argc, char **argv)
{
    enum delays delay;
    delay = (*(++argv) && **argv == '-') ? get_delay_flag(argv[0][1]) : DELAY_NORM;
    if (delay == DELAY_ERROR) {
        fputs(HELP_MSG, stderr);
        return 1;
    }

    argv += (*(argv) && **argv == '-') ? 1 : 0;
    do {
        FILE *src = (*argv) ? fopen(*argv, "r") : stdin;
        if (!src) {
            perror(*argv);
            continue;
        }

        cute_file_cpy(stdout, src, delay);
        if (src != stdin)
            fclose(src);
    } while (*argv && *(++argv));

    return 0;
}

static void cute_file_cpy(FILE *dst, FILE *src, enum delays delay)
{
    int c;

    while ((c = fgetc(src)) != EOF) {
        putc(c, dst);
        fflush(dst);
        usleep(delay);
    }
}

static enum delays get_delay_flag(char c)
{
    switch (c) {
    case 'l': return DELAY_SLOW;
    case 'n': return DELAY_NORM;
    case 'h': return DELAY_HIGH;
    case 's': return DELAY_SUPER;
    default : return DELAY_ERROR;
    }
}
