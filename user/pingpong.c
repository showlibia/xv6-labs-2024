#include "kernel/types.h"
#include "user/user.h"

int main(void)
{
    int parent[2], child[2];
    char parentBuf[5], childBuf[5];
    pipe(parent);
    pipe(child);
    if (fork() == 0) {
        close(parent[1]);
        read(parent[0], childBuf, sizeof(childBuf));
        fprintf(2, "%d: received %s\n", getpid(), childBuf);
        close(parent[0]);
        close(child[0]);
        write(child[1], "pong", 4);
        close(child[1]);
        exit(0);
    } else {
        close(parent[0]);
        write(parent[1], "ping", 4);
        close(parent[1]);
        close(child[1]);
        read(child[0], parentBuf, sizeof(parentBuf));
        fprintf(2, "%d: received %s\n", getpid(), parentBuf);
        close(child[0]);
    } 
    exit(0);
}