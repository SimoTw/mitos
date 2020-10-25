#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define R 0
#define W 1

int main(int argc, char *argv[]) {
    int first_pipe[2];
    int second_pipe[2];
    int *left_pipe, *right_pipe;
    int i, num, prime, pid, n;
    int *temp;

    pipe(first_pipe);
    if (fork() > 0) {
        close(first_pipe[R]);
        for(i = 2; i <= 35; i++) {
            write(first_pipe[W], &i, sizeof(i));
        }
        write(first_pipe[W], "\0", 1);
        close(first_pipe[W]);
        printf("exfore wait");

        wait((int *)0);
        printf("after wait");

    } else {
        pipe(second_pipe);
        left_pipe = first_pipe;
        right_pipe = second_pipe;
        for(;;) {
            pid = fork();
            if (pid == 0) {
                close(left_pipe[1]);
                close(right_pipe[0]);
                n = read(left_pipe[0], &prime, sizeof(prime));
                if (n <= 1) {
                 exit(0);

                }
                printf("prime %d\n", prime);
                for (;;) {
                    n = read(left_pipe[0], &num, sizeof num);
                    if (n <= 1) {
                        write(right_pipe[1], "\0", 1);
                        break;
                    }
                    if (num % prime != 0) {
                        write(right_pipe[1], &num, sizeof num);
                    }
                }
                close(left_pipe[0]);
                close(right_pipe[1]);
                exit(0);
            } else {
                wait((int *)0);
                temp = left_pipe;
                left_pipe = right_pipe;
                right_pipe = temp;
            }
        }
    }
    exit(0);

}