#include <fstream>
#include <iostream>
#include <string>

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

string get_token(string::iterator& scout, string::iterator end) {
    string token;

    while (scout != end && *scout == ' ') {
        ++scout;
    }

    while (scout != end && *scout != ' ') {
        token += *scout;
        ++scout;
    }

    return token;
}

void tokenize(string& buffer) {
    auto scout = buffer.begin();

    while (scout != buffer.end()) {
        string token = get_token(scout, buffer.end());
        if (!token.empty()) {
            cout << token << "\n";
        }
    }
}

int main(int argc, char* argv[]) {
    string buffer = get_content(argc, argv);
    tokenize(buffer);
    return 0;
}
