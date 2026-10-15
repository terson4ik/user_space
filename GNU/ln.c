/* simple replica of POSIX ln */
#include <stdio.h>
#include <unistd.h>
#include <string.h> 

typedef struct {
/* WRITE NEW FIELD IN PARSING FUNCTIONS */
    unsigned char is_sym    : 1;
    unsigned char is_verbose: 1;
    unsigned char show_help : 1;
    unsigned char show_vers : 1;
} flags;
enum values_for_if { GOOD = 1, BAD = 0 }; /* !BAD == true */

static int parse_arguments(const char **args, flags *flg, char **old, char **new);
static int parse_word(const char *wrd, flags *flg);
static int parse_flag(char opt, flags *flg);
static int linking(const flags *flg, const char *old, const char *new);

#define SHOW_TIP "Try 'ln --help' for more information.\n"
int main(int argc, char **argv)
{
    enum os_codes { SUCCESS, ARGS_FAIL, SYSTEM_FAIL };
    char *old, *new;
    flags flg;

    if (argc < 2) {
        fprintf(stderr, "ln: missing file operand\n" SHOW_TIP);
        return ARGS_FAIL;
    }

    if (!parse_arguments((const char **)argv, &flg, &old, &new))
        return ARGS_FAIL;

    if (!linking(&flg, old, new))
        return SYSTEM_FAIL;

    return SUCCESS;
}

#define HELP_MSG "Usage: ln [OPTION]... TARGET LINK_NAME\n" \
                 "Create hard links by default, symbolic links with --symbolic\n" \
                 "  -s, --symbolic\n" \
                 "\tmake symbolic links instead of hard links\n" \
                 "  -v, --verbose\n" \
                 "\tprint name of each linked file\n" \
                 "  --help\n" \
                 "\tdisplay this help and exit\n" \
                 "  --version\n" \
                 "\toutput version information and exit\n"
static int linking(const flags *flg, const char *old, const char *new)
{
    int (*operation)(const char *oldpath, const char *newpath);
    if (flg->show_help) {
        fputs(HELP_MSG, stdout);
        return GOOD;
    } else if (flg->show_vers) {
        fputs("ln (VladOS coreutils) 0.1\nwritten by Vladyslav Y. Y.\n", stdout);
        return GOOD;
    }

    operation = (flg->is_sym) ? &symlink : &link; /* in the future write switch */
    if (operation(old, new) == -1) {
        perror("ln");
        return BAD;
    }

    if (flg->is_verbose)
        fprintf(stdout, "'%s' %c> '%s'\n", new, (flg->is_sym) ? '-' : '=', old);

    return GOOD;
}

static int parse_arguments(const char **args, flags *flg, char **old, char **new)
{
    flg->is_sym = flg->is_verbose = flg->show_help = flg->show_vers = 0;
    *old = *new = NULL;

    while (*(++args) != NULL) {
        switch(**args) {
            case '-':
                if (args[0][1] == '-') {
                    if (!parse_word(*args+2, flg)) {
                        fprintf(stderr, "ln: unrecognized option: '%s'\n"
                                        SHOW_TIP, *args);
                        return BAD;
                    }
                } else {
                    int i;
                    for (i = 1; args[0][i] != '\0'; i++) {
                        if (!parse_flag(*(*args+i), flg)) {
                            fprintf(stderr, "ln: invalid option: '%s'\n"
                                    SHOW_TIP, *args);
                            return BAD;
                        }
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
                    return BAD;
                }
                break;
        }
    }

    if ((!*old || !*new) && !flg->show_help && !flg->show_vers) {
        fprintf(stderr, "ln: missing file operand\n" SHOW_TIP);
        return BAD;
    }

    return GOOD;
}


static int parse_word(const char *wrd, flags *flg)
{
    if (strcmp("help", wrd) == 0) {
        flg->show_help = 1;
        return GOOD;
    } else if (strcmp("symbolic", wrd) == 0) {
        flg->is_sym = 1;
        return GOOD;
    } else if (strcmp("verbose", wrd) == 0) {
        flg->is_verbose = 1;
        return GOOD;
    } else if (strcmp("version", wrd) == 0) {
        flg->show_vers = 1;
        return GOOD;
    }
    return BAD;
}

static int parse_flag(char opt, flags *flg)
{
    switch (opt) {
    case 's': flg->is_sym     = 1; return GOOD;
    case 'v': flg->is_verbose = 1; return GOOD;
    default : /* nothing; */       return BAD;
    }
}
