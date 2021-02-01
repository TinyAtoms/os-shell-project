#include <stdio.h>   // this will provide stdin for when iostream/cout get removed
#include <string.h>  // for all those lovely string manipulation functions with weird names
#include <unistd.h>  // for execvp

#include <cstdlib>
#include <iostream>
using std::cout;

int main()
{
    char input[100];
    char out[100];
    fgets(input, 100, stdin);
    memcpy(out, &input[1], strlen(input));
    out[strlen(input)] = '\0';
    int n_line = atoi(out);

}