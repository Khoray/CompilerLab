#ifndef RV_INST_IMPL_H
#define RV_INST_IMPL_H

#include "backend/rv_def.h"

namespace rv {

struct rv_inst {
    rvREG rd, rs1, rs2;         // operands of rv inst
    rvFREG frd, frs1, frs2;
    rvOPCODE op;                // opcode of rv inst
    bool is_label;
    
    uint32_t imm;               // optional, in immediate inst
    std::string label;          // optional, in beq/jarl inst
    std::string symbol;         // optional, for la

    std::string draw() const;
    rv_inst();
    rv_inst(std::string label_name);
};

};

#endif