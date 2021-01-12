#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

int MAXLEN = 100;

using std::cout;
struct command {
    bool background;
    std::vector<std::string> comm;
};

command get_command()
{
    command c;
    cout << ">>";
    std::string input;
    getline(std::cin, input);
    int pos = 0;
    // split with spaces
    while (pos != -1) {
        pos = input.find(' ');
        c.comm.push_back(input.substr(0, pos));
        input = input.substr(pos + 1, input.size() - pos - 1);
    }
    // remove & if it is there
    auto last = c.comm[c.comm.size() - 1];
    c.background = last[last.size() - 1] == '&';
    if (c.background) {
        last = last.substr(0, last.size() - 1);
        c.comm[c.comm.size() - 1] = last;
    }
    return c;
}

int main() { command c = get_command(); }