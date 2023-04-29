#include "ir/ir_program.h"

#include <vector>
#include <string>
#include <iostream>


ir::Program::Program(): functions(std::vector<ir::Function>()) {}

ir::GlobalVal::GlobalVal(ir::Operand va) {this->val = va;}

ir::GlobalVal::GlobalVal(ir::Operand va, int len) {this->val = va; this->maxlen = len;}

void ir::Program::addFunction(const ir::Function& proc) {
    functions.push_back(proc);
}

std::string ir::Program::draw() {
    std::cerr << "drawing program:\n";
    std::string ret;
    for (auto i :functions) {
        ret += i.draw();
        std::cerr << "function name:" << i.name << "\n";
    }
    ret += "GVT:\n";
    for (auto i : this->globalVal) {
        ret += "\t" + i.val.name + " " + toString(i.val.type) + " " + std::to_string(i.maxlen) + "\n";
    }
    return ret;
}