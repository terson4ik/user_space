#include <stdio.h>
#include <unistd.h>

typedef struct {
    unsigned char is_recur : 1;
    unsigned char is_force : 1;
} flags;

static int parse_flags(flags *flg, char **args);
static const char *eject_arguments(char ***cur_pos);

int main(int argc, char **argv)
{
    char **runner = argv + 1;
    flags flg;
    if (*runner == NULL) {
        fputs("rm: missing operand\n", stderr);
        return 1;
    }
    
    if (!parse_flags(flg, runner))
        return 1;

    
    target = argv[1];
    if (unlink(target) < 0) {
        perror(target);
        return 2;
    }

    return 0;
}
