#include "./helpers.h"


int main()
{
    Command c = get_command();
    if (c.nonempty) {
        execvp(c.args[0], c.args);
    }
}
