#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void redirect(int fd, int fds[]) {
    close(fd);
    dup(fds[fd]);
    close(fds[0]);
    close(fds[1]);
}

void primeProcess(int pfds[]) {
    int buf[1];
    int p;
    int fds[2];
    int pid;
    // read p
    redirect(0, pfds);
    if (read(0, buf, sizeof(int)) == 0) {
        close(0);
        exit(0);
    } else {
        p = buf[0];
        fprintf(1, "prime %d\n", p);
    }
    pipe(fds);
    pid = fork();
    if (pid == 0) {
        // create a child process to read the pile
        primeProcess(fds);
    } else {
        // write primes to pipe
        redirect(1, fds);
        while (read(0, buf, 1) != 0) {
            int num = buf[0];
            if (num % p != 0)
                write(1, &num, sizeof(num));
        }
        close(1);
        wait(0);
        close(0);
    }
}

int main(void) {
    int i;
    int pid;
    int fds[2];
    pipe(fds);
    pid = fork();
    if (pid == 0) {
         primeProcess(fds);
    } else {
        redirect(1, fds);
        for (i = 2; i <= 35; i++) {
            write(1, &i,  sizeof(i));
        }
        close(1);
        wait(0);
    }
    exit(0);
}