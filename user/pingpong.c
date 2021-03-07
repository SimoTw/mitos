#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void messageServer(char *message, int readfds[], int writefds[], int mode) {
    close(readfds[1]);
    close(writefds[0]);
    int pid = getpid();
    char buf[4];
    int ops = 0;
    while(ops < 2) {
        if (mode == 0) {
            read(readfds[0], buf, 4);
            fprintf(1, "%d: received %s\n", pid, buf);
            mode = 1;
            ops++;
        } else {
            write(writefds[1],  message, strlen(message));
            mode = 0;
            ops++;
        }
    }
    close(readfds[0]);
    close(writefds[1]);
    exit(0);
}

int main(int argc, char **argv) {
    int pid;
    int ptcfds[2];
    int ctpfds[2];
    pipe(ptcfds);
    pipe(ctpfds);
    pid = fork();
    if (pid == 0) { //child
        messageServer("ping", ptcfds, ctpfds, 0);
    } else {  //parent
        messageServer("pong", ctpfds, ptcfds, 1);
    }
}