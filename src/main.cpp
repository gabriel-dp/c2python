#include <fstream>
#include <iostream>
#include <string>

#include "generator.hpp"
#include "parser.hpp"
#include "token.hpp"

using namespace std;

string get_content(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Usage: " << argv[0] << " <input> <output>\n";
        exit(1);
    }

    ifstream input_file(argv[1]);
    if (!input_file) {
        cout << "Fail to read input file\n";
        exit(1);
    }

    return string((istreambuf_iterator<char>(input_file)), istreambuf_iterator<char>());
}

int main(int argc, char* argv[]) {
    string buffer = get_content(argc, argv);
    vector<Token> tokens = tokenize(buffer);
    PythonGenerator generator(argv[2]);
    Parser parser(tokens, generator);
    parser.parse();
    return 0;
}
