/* simple replica of POSIX ln; ```suckless.org``` edition */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> 

typedef struct {
/* WRITE NEW FIELD IN PARSING FUNCTIONS */
    unsigned char is_sym    : 1;
    unsigned char is_verbose: 1;
} flags;

static void usage(void)
{
    fprintf(stderr, "usage: ln [-s -v] oldfile newfile\n");
    exit(1);
}

static void linking(const flags *flg, const char *old, const char *new)
{
    int (*operation)(const char *oldpath, const char *newpath);

    operation = (flg->is_sym) ? &symlink : &link; /* in the future write switch */
    if (operation(old, new) == -1) {
        perror("ln");
        exit(1);
    }

    if (flg->is_verbose)
        fprintf(stdout, "'%s' %c> '%s'\n", new, (flg->is_sym) ? '-' : '=', old);
}

static int parse_flag(char opt, flags *flg)
{
    switch (opt) {
    case 's': flg->is_sym     = 1; return 1;
    case 'v': flg->is_verbose = 1; return 1;
    default : /* nothing; */       return 0;
    }
}

static void parse_arguments(const char **args, flags *flg, char **old, char **new)
{
    int i;
    flg->is_sym = flg->is_verbose = 0;
    *old = *new = NULL;

    while (*(++args) != NULL) {
        switch(**args) {
            case '-':
                for (i = 1; args[0][i] != '\0'; i++) {
                    if (!parse_flag(*(*args+i), flg)) {
                        fprintf(stderr, "ln: invalid option: '%s'\n", *args);
                        usage();
                    }
                }
                break;

            default:
                if (!*old)
                    *old = (char *)*args;
                else if (!*new)
                    *new = (char *)*args;
                else {
                    fprintf(stderr, "Too many arguments.\n");
                    usage();
                }
                break;
        }
    }
}

int main(int argc, char **argv)
{
    char *old, *new;
    flags flg;

    if (argc < 3)
        usage();

    parse_arguments((const char **)argv, &flg, &old, &new);
    linking(&flg, old, new);

    return 0;
}
