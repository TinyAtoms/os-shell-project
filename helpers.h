//
// Created by MassiveAtoms on 2/1/21.
// Edited by PyroStefan on 2/16/21

#ifndef OS_PROJ_HELPERS_H
#define OS_PROJ_HELPERS_H

#include <stdio.h>   // this will provide stdin for printf
#include <string.h>  // for all those lovely string manipulation functions with weird names
#include <unistd.h>  // for execvp

#include <cstdlib>


const int MAXLEN = 80;

struct Command {  // funct not allowed to return char* [], that's why there's this
    char input[MAXLEN] = "";
    char* args[MAXLEN];
    bool background = false;
    bool nonempty = false;  // execvp doesn't like it when you give empty args, easy way to check for it
};

// function declarations
Command get_command();                       // read command from consoles & check for special characters
Command executeFromHistory(int lineNumber);  // execute previously used command
int logHandler(char appndLn[]);              // save to file
int readNumberOfLines();
void display_history();  // return number of lines in histfile

/*
 * This captures input from stdin, execute past command if it is (!n, !),
 * @return: Command in a format that's ready to be passed to execvp
 */
Command get_command()
{
    /*
    first, we'll check if you actually input anything before enter
    then we'll handle !!
    after that !n
    after that, history
    if it's none of those, we'll parse the input and transform it 
    to a form that exevp takes
    */
    Command c;
    printf("osh>");
    fgets(c.input, MAXLEN, stdin);
    fflush(stdin);
    if (strlen(c.input) == 1) {  // pressed enter without any text any text
        c.args[0] = NULL;
        c.background = false;
        c.nonempty = false;
        return c;
    }
    else if (c.input[0] == '!' and c.input[1] == '!') { // case "!!"
        int n_line = readNumberOfLines();
        c = executeFromHistory(n_line);
        printf("%s", c.input);
        fflush(stdin);
    }
    else if (c.input[0] == '!') {  // handle ! cases
        char temp[MAXLEN];
        memcpy(temp, &c.input[1], strlen(c.input));
        temp[strlen(c.input)] = '\0';
        int n_line = atoi(temp);
        if (!n_line) {  // atoi("something that's not a number") returns 0
            printf("invalid use of !history. use !<nonzero number>");
            c.args[0] = NULL;
            c.background = false;
            c.nonempty = false;
            return c;
        }
        else if (n_line > readNumberOfLines()) {  // if n > maxlines
            printf("osshell: no such event: ");
            printf("%s", temp);
            c.args[0] = NULL;
            c.background = false;
            c.nonempty = false;
            return c;
        }
        else {
            c = executeFromHistory(n_line);
            printf("%s", c.input);
            fflush(stdin);
        }
    }
    else if (!strcmp(c.input, "history\n")) {  // handle history command
        display_history();
    }

    c.nonempty = true;
    logHandler(c.input);
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

/*
 * displays last 10 commands
 * takes no input
 */
void display_history()
{
    int n = readNumberOfLines();
    for (int i = 0; i < 10; i++) {
        Command x = executeFromHistory(n - i);
        printf("%d\t%s", n - i, x.input);
    }
}

/*
 * Executes the command that ran linenumber times ago
 * @param: int LineNumber is the line# we want to execute????
 * @return: Command object that nearly ready to pass on to execvp
 */
Command executeFromHistory(int lineNumber)
{
    if (lineNumber < 0) {
        lineNumber = readNumberOfLines() + lineNumber + 1;
    }
    int count = 1;
    Command command;
    FILE* fp = fopen("./osshell_history", "r");
    char temp[MAXLEN];
    while (fgets(temp, MAXLEN, fp)) {  // read line from file
        if (count == lineNumber) {
            strcpy(command.input, temp);
            break;
        }
        count++;
    }
    fclose(fp);
    return command;
}

/* Adds command that's going to be run to history file
 * and maintains length of 10 for history file
 * @param: line[] line that needs to be appended
 * @return: void
 */
int logHandler(char line[])  // fixed it already
{
    FILE* file = fopen("./osshell_history", "a");  // change to ~/oshell instead of ./
    auto b = fputs(line, file);
    fclose(file);
    return b;
}

/*
 * Returns how many lines historyfile has
 * @params: void
 * @params: n lines of historyfile
 */
int readNumberOfLines()
{
    char buffer;
    FILE* fp;
    int lines = 0;

    fp = fopen("./osshell_history", "r");
    buffer = getc(fp);

    while (buffer != EOF) {
        buffer = getc(fp);
        if (buffer == '\n') lines++;
    }
    return lines;
}

#endif  // OS_PROJ_HELPERS_H
