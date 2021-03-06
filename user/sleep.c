#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char **argv) {
    int interval;
    if (argc < 2) {
        char *err_message = "sleep usage: sleep <time>\n";
        write(2, err_message, strlen(err_message));
        exit(1);
    }
    interval = atoi(argv[1]);
    sleep(interval);
    exit(0);
}