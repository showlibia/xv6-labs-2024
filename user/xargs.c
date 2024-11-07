#include "kernel/types.h"
#include "kernel/param.h"
#include "kernel/fcntl.h"
#include "user/user.h"

#define BUFFSIZE 512

int main(int argc, char* argv[])
{
    int i;
    char buf[BUFFSIZE];
    char *cmd_args[MAXARG];
    if (argc < 2) {
        printf("Usage: xargs command ...");
    }

    for (i = 1; i < argc && i < MAXARG - 1; i++) {
        cmd_args[i - 1] = argv[i];
    }
    cmd_args[i - 1] = 0;

    int n, index = 0;
    while ((n = read(0, &buf[index], 1)) != 0) {
        if (buf[index] == '\n') {
            buf[index] = 0;
            
            if (fork() == 0) {
                cmd_args[i - 1] = buf;
                cmd_args[i] = 0;

               exec(cmd_args[0], cmd_args);
               printf("xargs: exec fails");
               exit(1);
            } else {
                wait(0);
            }
            index = 0;
        } else {
            index++;

        }
    }

    exit(0);
}