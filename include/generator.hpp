#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <fstream>
#include <string>
using namespace std;

class Generator {
   public:
    Generator(const char* output_path) : output_path(output_path) {
        file = ofstream(output_path);
    };
    virtual ~Generator() {
        file.close();
    }

    virtual void startStatement() = 0;
    virtual void endStatement() = 0;
    virtual void startBlock() = 0;
    virtual void endBlock() = 0;
    virtual void generateToken(string content) = 0;
    virtual void generateCondition() = 0;
    virtual void generateConditionNot() = 0;
    virtual void generateLoop() = 0;
    virtual void generateExpression() = 0;
    virtual void generateFunction(string identificator) = 0;
    virtual void generateDeclaration(string identificator) = 0;
    virtual void generateAssignment(string identificator) = 0;
    virtual void generateReturn() = 0;

   private:
    const char* output_path;
    ofstream file;

   protected:
    virtual void write(string content) {
        file << content;
    }
};

class PythonGenerator : public Generator {
   public:
    PythonGenerator(const char* output_path) : Generator(output_path) {}

    void startStatement();
    void endStatement();
    void startBlock();
    void endBlock();
    void generateToken(string content);
    void generateLoop();
    void generateCondition();
    void generateConditionNot();
    void generateExpression();
    void generateFunction(string identificator);
    void generateAssignment(string identificator);
    void generateDeclaration(string identificator);
    void generateReturn();

   private:
    int indentation_level = 0;
};

#endif