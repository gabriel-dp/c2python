#ifndef PARSER_HPP
#define PARSER_HPP

#include <stack>

#include "token.hpp"

typedef struct context_data_t {
    bool is_function = false;
    bool is_loop = false;
} context_data;

class VariableData {
   public:
    VariableData() {};
    VariableData(string type) : type(type) {};
    VariableData(string type, bool is_initialized) : type(type), is_initialized(is_initialized) {};
    VariableData(string type, bool is_initialized, bool is_used) : type(type), is_initialized(is_initialized), is_used(is_used) {};

    string type;
    bool is_initialized = false;
    bool is_used = false;
};

class Scope {
   public:
    Scope() {
        context_data initial_context;
        unordered_map<string, VariableData> initial_variables;
        this->scope_stack.emplace(make_pair(initial_context, initial_variables));
    }

    void init() {
        this->scope_stack.emplace(make_pair(this->actual_context_data(), this->actual_variables_data()));
    }

    void endActual() {
        scope_stack.pop();
    }

    void insert_variable_data(string identificator, VariableData data) {
        auto& variables = actual_variables_data();
        variables[identificator] = data;
    }

    VariableData* get_variable(string identificator) {
        auto& variables = actual_variables_data();
        auto found = variables.find(identificator);
        if (found != variables.end()) {
            return &(found->second);
        }
        return nullptr;
    }

    void kill_variable(string identificator) {
        auto& variables = actual_variables_data();
        if (variables.find(identificator) != variables.end()) {
            variables.erase(identificator);
        }
    }

    context_data& actual_context_data() {
        return this->scope_stack.top().first;
    }

    unordered_map<string, VariableData>& actual_variables_data() {
        return this->scope_stack.top().second;
    }

    void print() {
        cout << "is_function = " << actual_context_data().is_function << " |  is_loop = " << actual_context_data().is_loop << "\n";
        cout << actual_variables_data().size() << "\n";
        for (auto variable : actual_variables_data()) {
            cout << variable.first << " { " << variable.second.type << ", " << variable.second.is_initialized << ", " << variable.second.is_used << " }\n";
        }
        cout << "\n";
    }

   private:
    stack<pair<context_data, unordered_map<string, VariableData>>> scope_stack;
};

class Parser {
   public:
    Parser(const std::vector<Token>& tokens) : tokens(tokens), pos(0), level(0) {}
    void parse();

   private:
    vector<string> TYPES = {"int", "float", "double", "char", "void"};
    vector<string> TYPE_DECORATORS = {"unsigned", "short", "long", "signed"};
    vector<string> OPERATORS_BINARY = {"+", "-", "*", "/", "==", "!=", "<", "<=", ">", ">="};
    vector<string> OPERATORS_UNARY_PREFIX = {"!", "+", "-", "++", "--"};
    vector<string> OPERATORS_UNARY_POSTFIX = {"++", "--"};

    const std::vector<Token>& tokens;
    size_t pos;
    int level = 0;
    Scope scope;

    Token peek() const;
    Token advance();
    Token last() const;
    bool match(TokenType type, bool must_advance);
    bool match(string content, bool must_advance);
    bool match(vector<string> content, bool must_advance);
    void printAction(string action, bool newLine, bool printLevel);

    bool program();

    bool declaration();
    bool statement();
    bool assignment();

    bool type();
    bool block();

    bool keyStatement();
    bool ifStatement();
    bool whileStatement();
    bool doWhileStatement();
    bool forStatement();
    bool returnStatement();
    bool loopCommandsStatement();

    bool expression();
    bool parseBinaryExpression();
    bool parseUnaryExpression();
    bool parsePrimaryExpression();
};

class ParserException : public exception {
   public:
    ParserException(string message, Token token)
        : message(message), token(token) {
        full_message = message + " '" + token.content + "' (" +
                       to_string(token.position.first) + ", " +
                       to_string(token.position.second) + ")";
    }

    const char* what() const noexcept override {
        return full_message.c_str();
    }

   private:
    string message;
    Token token;
    string full_message;
};

#endif