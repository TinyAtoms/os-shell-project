#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
using std::cout;
#include "./helpers.h"

int main()
{
    while (true) {
        Command c = get_command();
        if (c.input == "exit\n") {  // close shell
            return 0;
        }
        // background spawn
        pid_t pid = fork();
        if (pid < 0) { /* error occurred */
            fprintf(stderr, "Fork Failed");
            return 1;
        }
        else if (pid == 0) { /* child process */
            // wait(NULL);
            execvp(c.args[0], c.args);
            return 0;
        }
        else { // parent process
            // return 0;
            ;
        }
    }
}
