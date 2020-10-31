#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


void redirect(int fd, int fds[]) {
    close(fd);
    dup(fds[fd]);
    close(fds[0]);
    close(fds[1]);    
}

void source() {
    int i;
    for (i = 2; i <= 35; i++)
        write(1, &i, sizeof(i));
    close(1);
    
}

void parent_process(int parent_pds[]) {
    int prime;
    if (read(0, &prime, sizeof(prime))) {
        printf("prime %d\n", prime);
    } else {
        exit(0);
    }
    int child_pds[2];
    pipe(child_pds);
    int pid;
    pid = fork();
    if (pid > 0) {
        int num;
        redirect(1, child_pds);
        while(read(0, &num, sizeof(num)))
            if (num % prime != 0) 
                write(1, &num, sizeof(num));
        close(0);
        close(1);
        wait((int *)0);
    } else if (pid == 0) {
        redirect(0, child_pds);
        parent_process(child_pds);

    }
}


int main(int argc, char *argv[]) {
    int parent_pds[2];
    pipe(parent_pds);
    if (fork() > 0) { 
        redirect(1, parent_pds);
        source();
        wait((int *)0);
    } else {
        redirect(0, parent_pds);
        parent_process(parent_pds);
        wait(0);
    }
    exit(0);
}