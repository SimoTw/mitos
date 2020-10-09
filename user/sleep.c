#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main (int argc, char *argv[]) {
    int interval;

    if (argc <= 1) {
        fprintf(2, "error: sleep required 1 int argument but received none\n");
        exit(1);
    }
    interval = atoi(argv[1]);
    printf("(nothing happens for a little while)\n");
    sleep(interval);
    exit(0);
}