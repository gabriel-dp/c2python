#include <fstream>
#include <iostream>
#include <string>

#include "parser.hpp"
#include "token.hpp"

using namespace std;

string get_content(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <filename>\n";
        exit(1);
    }

    ifstream file(argv[1]);
    if (!file) {
        cout << "Fail to read file\n";
        exit(1);
    }

    return string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
}

int main(int argc, char* argv[]) {
    string buffer = get_content(argc, argv);
    vector<Token> tokens = tokenize(buffer);
    Parser parser(tokens);
    if (parser.parse()) {
        cout << "Parsing successful!\n";
    } else {
        cout << "Parsing failed.\n";
    }
    return 0;
}
