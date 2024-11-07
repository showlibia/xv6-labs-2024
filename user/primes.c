#include "kernel/types.h"
#include "user/user.h"

void primes(int pleft) __attribute__((noreturn));

int main(void)
{
    int left[2];
    pipe(left);
    if (fork() == 0) {
        close(left[1]);
        primes(left[0]);
    } else {
        close(left[0]);
        for (int i = 2; i <= 280; i++) {
            if (write(left[1], &i, sizeof(i)) != sizeof(i)) {
                printf("write fails at %d", i);
            }
        }
        close(left[1]);
        wait(0);
    }
    exit(0);
}

void primes(int pleft)
{
    int prime, n;
    if (read(pleft, &prime, sizeof(prime)) == 0) {
        close(pleft);
        exit(0);
    }
    int right[2];
    pipe(right);
    fprintf(2, "prime %d\n", prime);
    if (fork() == 0) {
        close(right[1]);
        close(pleft);
        primes(right[0]);
    } else {
        close(right[0]);
        while (read(pleft, &n, sizeof(n)) == sizeof(n)) {
            if (n % prime != 0) {
                if (write(right[1], &n, sizeof(n)) != sizeof(n)) {
                    printf("write fails\n");
                }
            }
        }
        close(pleft);
        close(right[1]);
        wait(0);
        exit(0);
    }
}