#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define R 0
#define W 1
#define True 1
#define False 0

int main(int argc, char *argv[]) {
    int mod, pid, i, num;
    int writer_pipe[2];
    int reader_pipe[2];
    mod = 0;

    pid = fork();
    pipe(writer_pipe);
    pipe(reader_pipe);

    if (pid > 0) {
        // root parent producer
        close(writer_pipe[R]);
        close(reader_pipe[W]);
        for(i = 0; i < 35; i++) {
            write(writer_pipe[W], &i, sizeof(i));
        }
        close(writer_pipe[W]);
        wait(0);
        while (read(reader_pipe[R], &num, sizeof(num))) 
            printf("prime %d\n", num);
        
        exit(0);
    } else {
    // child process    
        while (mod <= 35) {
            pid = fork();
            if (pid < 0) {
                fprintf(2, "fork error");
                exit(1);
            }
            if (pid == 0) { 
                close(reader_pipe[W]);
                close(writer_pipe[R]);
                int num;
                while (read(reader_pipe[0], &num, sizeof num) > 0) {
                    if (num % mod != 0) {
                        write(writer_pipe[R], &num, sizeof num);
                    }
                }
                close(reader_pipe[R]);
                close(writer_pipe[W]);
            }
        }
    }
}