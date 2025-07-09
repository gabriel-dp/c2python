#include "generator.hpp"

void PythonGenerator::startBlock() {
    write(":\n");
    indentation_level++;
}

void PythonGenerator::endBlock() {
    indentation_level--;
}

void PythonGenerator::startStatement() {
    for (int i = 0; i < indentation_level; i++) {
        Generator::write("\t");
    }
}

void PythonGenerator::endStatement() {
    write("\n");
}

void PythonGenerator::generateToken(string token) {
    write(token);
}

void PythonGenerator::generateCondition() {
    write("if ");
}

void PythonGenerator::generateConditionNot() {
    write("else ");
}

void PythonGenerator::generateLoop() {
    write("while ");
}

void PythonGenerator::generateExpression() {
    write("if ");
}

void PythonGenerator::generateAssignment(string identificator) {
    write(identificator + " = ");
}

void PythonGenerator::generateDeclaration(string identificator) {
    write(identificator);
}

void PythonGenerator::generateFunction(string identificator) {
    write("def " + identificator);
}

void PythonGenerator::generateReturn() {
    write("return ");
}
