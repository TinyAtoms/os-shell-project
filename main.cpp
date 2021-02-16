#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstring>
#include "./helpers.h"

int main()
{
    while (true) {
        Command c = get_command();
        if (!strcmp(c.input, "exit")) {
            return 0;
        }

        pid_t pid = fork();
        if (pid < 0) { /* error occurred */
            fprintf(stderr, "Fork Failed");
            return 1;
        }
        else if (pid == 0) { /* child process */
            execvp(c.args[0], c.args);
            return 0;
        }
        else {
            if (!c.background) {
                wait(NULL);
            }
        }
    }
}
