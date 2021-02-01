#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "./helpers.h"

using std::string;
using std::vector;

// executeFromHistory tests ------------------------
bool EFH_test()  // as mentioned in assignment
{                // checks if output matches
    std::ifstream file("./osshell_history");
    vector<string> CPP_strings{""};
    string temp;
    while (std::getline(file, temp)) {
        CPP_strings.push_back(temp);
    }

    for (int i = 1; i < CPP_strings.size(); i++) {
        CPP_strings[i] += "\n";
        // string EFH_string = executeFromHistory(CPP_strings.size() - i + 1).input; // what is programmed in now
        string EFH_string = executeFromHistory(i).input;  // what actually needs to happen tho
        // std::cout << i << ":" << EFH_string; // TODO: remove
        // std::cout << i << ":" << CPP_strings[i] << "\n";
        if (CPP_strings[i].compare(EFH_string) != 0) {
            return false;
        }
    }
    return true;
}

bool EFH_high_line()
{
    std::ifstream file("./osshell_history");
    int n_lines = 1;
    while (!file.eof()) {
        string temp;
        std::getline(file, temp);
        n_lines += 1;
    }
    std::cout << executeFromHistory(n_lines + 2).input;
}

int main()
{
    vector<std::pair<string, bool>> test_descr
    {
        {"execFromHist expected behaviour:\t", EFH_test()}, 
        { "execFromHist when n > total lines:\t", EFH_high_line() }
    };
    for (const auto& test : test_descr) {
        std::cout << test.first;
        if (test.second) {
            std::cout << "passed\n";
        }
        else {
            std::cout << "FAILED!!";
        }
    }
}
