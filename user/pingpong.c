#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int message_server(char *message, int fd[]) {
        int childid;
        char buf[52];
        childid = getpid();
        if (read(fd[0], buf, sizeof buf) > 0) 
            printf("%d: received %s", childid, buf);
        else 
            write(fd[1], message, strlen(message));  
        exit(0);
}

int main(int argc, char *argv[]) {
    int pid;
    int fd1[2], fd2[2];
    pipe(fd1);
    pipe(fd2);
    pid = fork();
    if (pid < 0) {
        fprintf(2, "fork error");
        exit(1);
    }
    if (pid == 0) { // child
        char buf[52];
        int childid;
        close(fd1[1]);
        close(fd2[0]);
        childid = getpid();
        read(fd1[0], buf, 52);
        printf("%d: received %s\n", childid, buf);
        close(fd1[0]);
        write(fd2[1], "pong", 4);
        close(fd2[1]);
        exit(0);
    } else {
        char buf[52];
        int parentID;
        close(fd1[0]);
        close(fd2[1]);
        write(fd1[1], "ping", 4);
        wait((int *)0);
        parentID = getpid();
        read(fd2[0], buf, 52);
        printf("%d: received %s\n", parentID, buf);
        
        close(fd2[0]);
    }

    exit(0);
}