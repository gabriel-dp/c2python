#ifndef SCOPE_HPP
#define SCOPE_HPP

#include <stack>
#include <string>
#include <unordered_map>
using namespace std;

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

    void init();
    void endActual();
    void insert_variable_data(string identificator, VariableData data);
    VariableData* get_variable(string identificator);
    void kill_variable(string identificator);
    context_data& actual_context_data();
    unordered_map<string, VariableData>& actual_variables_data();
    void print();

   private:
    stack<pair<context_data, unordered_map<string, VariableData>>> scope_stack;
};

#endif