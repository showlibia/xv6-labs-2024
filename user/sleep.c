#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[])
{
    if (argc < 2) {
        fprintf(2, "Usage: sleep time\n");
        exit(1);
    }
    int time = atoi(argv[1]);
    if (time >= 0) {
        fprintf(2, "(nothing happens for a little while)\n");
        sleep(time);
    } else {
        exit(1);
    }
    exit(0);
}