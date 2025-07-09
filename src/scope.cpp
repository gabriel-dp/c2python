#include "scope.hpp"

#include <iostream>

void Scope::init() {
    this->scope_stack.emplace(make_pair(this->actual_context_data(), this->actual_variables_data()));
}

void Scope::endActual() {
    scope_stack.pop();
}

void Scope::insert_variable_data(string identificator, VariableData data) {
    auto& variables = actual_variables_data();
    variables[identificator] = data;
}

VariableData* Scope::get_variable(string identificator) {
    auto& variables = actual_variables_data();
    auto found = variables.find(identificator);
    if (found != variables.end()) {
        return &(found->second);
    }
    return nullptr;
}

void Scope::kill_variable(string identificator) {
    auto& variables = actual_variables_data();
    if (variables.find(identificator) != variables.end()) {
        variables.erase(identificator);
    }
}

context_data& Scope::actual_context_data() {
    return this->scope_stack.top().first;
}

unordered_map<string, VariableData>& Scope::actual_variables_data() {
    return this->scope_stack.top().second;
}

void Scope::print() {
    cout << "is_function = " << actual_context_data().is_function << " |  is_loop = " << actual_context_data().is_loop << "\n";
    cout << actual_variables_data().size() << "\n";
    for (auto variable : actual_variables_data()) {
        cout << variable.first << " { " << variable.second.type << ", " << variable.second.is_initialized << ", " << variable.second.is_used << " }\n";
    }
    cout << "\n";
}