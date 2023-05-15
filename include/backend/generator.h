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
    const std::vector<ir::GlobalVal> &globalVal;
    std::map<ir::Operand, int, operandCmp> _table;
    int offset; // offset to s0

    stackVarMap(const std::vector<ir::GlobalVal> &gb);

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
public:
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
    regAllocator(std::vector<rv::rv_inst*> &rv_insts, const std::vector<ir::GlobalVal> &globalVal);

    void add_availables();

    void update(rvREG r, int time);
    void update(rvFREG r, int time);

    void spill(rvREG r);
    void spill(rvFREG r);
    void load(rvREG r, ir::Operand op, int time, int needload);
    void load(rvFREG r, ir::Operand op, int time, int needload);

    rv::rvREG getReg(ir::Operand, int time, int needload);
    rv::rvFREG fgetReg(ir::Operand, int time, int needload);

    void clearregs();
};

struct FloatConstTable {
    std::ofstream &fout;
    std::map<float, std::string> float_const_to_string_map;

    std::string add_float_const(float f);

    std::string find_float_const(float f);

    void gen_rodata();
    FloatConstTable(std::ofstream &);
};

struct Generator {
    const ir::Program& program;         // the program to gen
    std::ofstream& fout;                 // output file

    Generator(ir::Program&, std::ofstream&);

    FloatConstTable *float_const_table;
    regAllocator *reg_allocator;
    std::vector<rv_inst*> *rv_insts;
    std::vector<int> *goto_label_lines;
    std::set<rv_inst*> *ret_set;
    int max_call_overflow_paras;
    int global_label_id;
    // generate wrapper function
    int get_label_id(int line_num);
    void gen();
    void gen_func(const ir::Function&);
    void gen_instr(const ir::Instruction&, int time);
};



} // namespace backend


#endif