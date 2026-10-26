#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
static size_t mstrlen(const char *s);
static void mstrcpy(char *dst, const char *src);
static void mstrcat(char *dst, const char *src);

static int file_copy(int dst_fd, int src_fd);
static int is_c_lang_type(const char *fname);
static char *two_str_dup(const char *head, const char *tail);

int main(int argc, char **argv)
{
    DIR *dir;
    struct dirent *dent;

    char have_arg = argc > 1;
    char *path = have_arg ? argv[1] : ".";
    
    dir = opendir(path);
    if (!dir) {
        perror(path);
        return 1;
    }

    while ((dent = readdir(dir)) != NULL) {
        int fd;
        char *name;
        if (dent->d_type != DT_REG || !is_c_lang_type(dent->d_name))
            continue;

        name = have_arg ? two_str_dup(path, dent->d_name) : dent->d_name;
        if (!name) {
            perror("malloc");
            closedir(dir);
            return 2;
        }

        fd = open(name, O_RDONLY);
        if (fd < 0) {
            perror(name);
            closedir(dir);
            free(name);
            return 2;
        }

        printf("----------------------------------\nFILE: %s\n", dent->d_name);
        fflush(stdout);
        if (!file_copy(STDOUT_FILENO, fd)) {
            closedir(dir);
            return 2;
        }

        close(fd);
        if (have_arg)
            free(name);
    }

    closedir(dir);
    return 0;
}

#define BUF_SIZE 4096
static int file_copy(int dst_fd, int src_fd)
{
    char buf[BUF_SIZE];
    int count;
    while ((count = read(src_fd, buf, BUF_SIZE)) > 0)
        write(dst_fd, buf, count);

    if (count < 0) {
        perror("Output error");
        return 0;
    }
    return 1;
}

static int is_c_lang_type(const char *fname)
{
    const char *r = fname;

    /* get tail */
    while (*r)
        r++;
    
    if (--r <= fname || (*r != 'c' && *r != 'h'))
        return 0;

    if (--r < fname || *r != '.')
        return 0;

    return 1;
}

static char *two_str_dup(const char *head, const char *tail)
{
    char *new = malloc(mstrlen(head) + mstrlen(tail) + 2); /* '/' + '\0' */
    if (!new)
        return NULL;

    mstrcpy(new, head);
    mstrcat(new, "/");
    mstrcat(new, tail);

    return new;
}

static size_t mstrlen(const char *s)
{
    const char *r = s;
    while (*r)
        r++;
    return r-s;
}

static void mstrcpy(char *dst, const char *src)
{
    while ((*(dst++) = *(src++)))
    {}
}

static void mstrcat(char *dst, const char *src)
{
    while (*dst)
        dst++;

    mstrcpy(dst, src);
}
