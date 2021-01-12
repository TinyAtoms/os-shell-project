
#include <stdio.h>   // this will provide stdin for when iostream/cout get removed
#include <string.h>  // for all those lovely string manipulation functions with weird names
#include <unistd.h>  // for execvp

#include <iostream>  // for cout
// #include <stdlib.h>
// #include <unistd.h>

// using std::cout;

const int MAXLEN = 100;
struct command {  // funct not allowed to return char* [], that's why there's this
    char input[MAXLEN];
    char* args[MAXLEN];
    bool background;
    bool nonempty;  // execvp doesn't like it when you give empty args, easy way to check for it
};

/*
 * logs all the commands to a file
 * @argument: char[]
 * returns int for success_status
 */
int log_history(char line[])
{
    FILE* file = fopen("./osshell_history", "a");  // change to ~/oshell instead of ./
    auto b = fputs(line, file);
    fclose(file);
    return b;
};

command get_command()
{
    command c;
    printf(">>");
    fgets(c.input, MAXLEN, stdin);
    fflush(stdin);
    if (strlen(c.input) == 1) {  // pressed enter without any text any text
        c.args[0] = NULL;
        c.background = false;
        c.nonempty = false;
        return c;
    }
    c.nonempty = true;
    char temp[100];
    strcpy(temp, c.input);
    log_history(temp);
    c.input[strlen(c.input) - 1] = '\0';  // newline is captured too, need to remove

    // snips char arr into arguments
    char* token;
    char* rest = c.input;
    int i = 0;
    while ((token = strtok_r(rest, " ", &rest))) {
        c.args[i] = token;
        i++;
    }
    c.args[i] = NULL;
    // check for '&' which indicates we want it to run in the bg
    char* last = c.args[i - 1];
    if (last[strlen(last) - 1] == '&') {
        c.background = true;
        c.args[i - 1][strlen(last) - 1] = '\0';
    }
    return c;
}

int main()
{
    command c = get_command();
    if (c.nonempty) {
        execvp(c.args[0], c.args);
    }
}