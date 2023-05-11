#ifndef GENERARATOR_H
#define GENERARATOR_H

#include "ir/ir.h"
#include "backend/rv_def.h"
#include "backend/rv_inst_impl.h"


#include<set>
#include<queue>
#include<utility>
#include<map>
#include<string>
#include<vector>
#include<fstream>
using namespace rv;
namespace backend {

// it is a map bewteen variable and its mem addr, the mem addr of a local variable can be identified by ($s0 + off)
struct operandCmp {
    bool operator() (const ir::Operand& a, const ir::Operand& b) const;
};
struct stackVarMap {
    std::map<ir::Operand, int, operandCmp> _table;
    int offset; // offset to s0

    stackVarMap();

    /**
     * @brief find the addr of a ir::Operand
     * @return the offset
    */
    int find_operand(ir::Operand);

    /**
     * @brief add a ir::Operand into current map, alloc space for this variable in memory 
     * @param[in] size: the space needed(in byte)
     * @return the offset
    */
    int add_operand(ir::Operand, uint32_t size = 4);
};


class regAllocator {
private:
    std::vector<rv::rv_inst*> &rv_insts;
    std::map<ir::Operand, rvREG, operandCmp> op2reg_map;
    std::map<ir::Operand, rvFREG, operandCmp> fop2freg_map;
    std::vector<ir::Operand> reg2op_map;
    std::vector<ir::Operand> freg2fop_map;

    std::set<rvREG> available_regs;
    std::set<rvFREG> available_fregs;
    // LRU Cache
    std::set<std::pair<int, rvREG>> reg_using;
    std::set<std::pair<int, rvFREG>> freg_using;
    std::vector<int> reg_timestamp;
    std::vector<int> freg_timestamp;

    stackVarMap stack_var_map;
public:
    regAllocator(std::vector<rv::rv_inst*> &rv_insts);

    void update(rvREG r, int time);
    void update(rvFREG r, int time);

    void spill(rvREG r);
    void spill(rvFREG r);
    void load(rvREG r, ir::Operand op, int time);
    void load(rvFREG r, ir::Operand op, int time);

    rv::rvREG getReg(ir::Operand, int time);
    rv::rvFREG fgetReg(ir::Operand, int time);
};


struct Generator {
    const ir::Program& program;         // the program to gen
    std::ofstream& fout;                 // output file

    Generator(ir::Program&, std::ofstream&);

    regAllocator *reg_allocator;
    std::vector<rv_inst*> *rv_insts;
    std::vector<int> *goto_label_lines;
    // generate wrapper function
    int get_label_id(int line_num);
    void gen();
    void gen_func(const ir::Function&);
    void gen_instr(const ir::Instruction&, int time);
};



} // namespace backend


#endif