#include"backend/generator.h"

#include<assert.h>

using std::string;
using namespace rv;
using namespace ir;


#define TODO assert(0 && "todo")

// rv_defs
string rv::toString(rvREG r) {
    if(r == rvREG::X0) return "zero";
    if(r == rvREG::X1) return "ra";
    if(r == rvREG::X2) return "sp";
    if(r == rvREG::X3) return "gp";
    if(r == rvREG::X4) return "tp";
    if(r >= rvREG::X5 && r <= rvREG::X7) return "t" + std::to_string((int) r - 5);
    if(r == rvREG::X8) return "s0";
    if(r == rvREG::X9) return "s1";
    if(r >= rvREG::X10 && r <= rvREG::X17) return "a" + std::to_string((int) r - 10);
    if(r >= rvREG::X18 && r <= rvREG::X27) return "s" + std::to_string((int) r - 16);
    if(r >= rvREG::X28 && r <= rvREG::X31) return "t" + std::to_string((int) r - 25);
    assert(0 && "invalid r");
}

string rv::toString(rvFREG r) {
    if(r >= rvFREG::F0 && r <= rvFREG::F7) return "ft" + std::to_string((int) r);
    if(r >= rvFREG::F8 && r <= rvFREG::F9) return "fs" + std::to_string((int) r - 8);
    if(r >= rvFREG::F10 && r <= rvFREG::F11) return "fa" + std::to_string((int) r - 10);
    if(r >= rvFREG::F12 && r <= rvFREG::F17) return "fa" + std::to_string((int) r - 10);
    if(r >= rvFREG::F18 && r <= rvFREG::F27) return "fs" + std::to_string((int) r - 16);
    if(r >= rvFREG::F28 && r <= rvFREG::F31) return "ft" + std::to_string((int) r - 25);
    assert(0 && "invalid r");
}

string rv::toString(rvOPCODE op) {
    switch(op) {
    case rvOPCODE::ADD:
        return "add";
    case rvOPCODE::SUB:
        return "sub";
    case rvOPCODE::XOR:
        return "xor";
    case rvOPCODE::OR:
        return "or";
    case rvOPCODE::AND:
        return "and";
    case rvOPCODE::SLL:
        return "sll";
    case rvOPCODE::SRL:
        return "srl";
    case rvOPCODE::SRA:
        return "sra";
    case rvOPCODE::SLT:
        return "slt";
    case rvOPCODE::SLTU:
        return "sltu";
    case rvOPCODE::ADDI:
        return "addi";
    case rvOPCODE::XORI:
        return "xori";
    case rvOPCODE::ORI:
        return "ori";
    case rvOPCODE::ANDI:
        return "andi";
    case rvOPCODE::SLLI:
        return "slli";
    case rvOPCODE::SRLI:
        return "srli";
    case rvOPCODE::SRAI:
        return "srai";
    case rvOPCODE::SLTI:
        return "slti";
    case rvOPCODE::SLTIU:
        return "sltiu";
    case rvOPCODE::LW:
        return "lw";
    case rvOPCODE::SW:
        return "sw";
    case rvOPCODE::BEQ:
        return "beq";
    case rvOPCODE::BNE:
        return "bne";
    case rvOPCODE::BLT:
        return "blt";
    case rvOPCODE::BGE:
        return "bge";
    case rvOPCODE::BLTU:
        return "bltu";
    case rvOPCODE::BGEU:
        return "bgeu";
    case rvOPCODE::JAL:
        return "jal";
    case rvOPCODE::JALR:
        return "jalr";
    case rvOPCODE::LA:
        return "la";
    case rvOPCODE::LI:
        return "li";
    case rvOPCODE::MOV:
        return "mov";
    case rvOPCODE::J:
        return "j";
    }
}

// rv_inst_impl
string rv::rv_inst::draw() const {
    string ret;
    ret += toString(op) + " ";
    switch(op) {
    case rvOPCODE::ADD:
    case rvOPCODE::SUB:
    case rvOPCODE::XOR:
    case rvOPCODE::OR:
    case rvOPCODE::AND:
    case rvOPCODE::SLL:
    case rvOPCODE::SRL:
    case rvOPCODE::SRA:
    case rvOPCODE::SLT:
    case rvOPCODE::SLTU:
        ret += toString(rd) + ", " + toString(rs1) + ", " + toString(rs2);
        break;
    case rvOPCODE::ADDI:
    case rvOPCODE::XORI:
    case rvOPCODE::ORI:
    case rvOPCODE::ANDI:
    case rvOPCODE::SLLI:
    case rvOPCODE::SRLI:
    case rvOPCODE::SRAI:
    case rvOPCODE::SLTI:
    case rvOPCODE::SLTIU:
        ret += toString(rd) + ", " + toString(rs1) + ", " + std::to_string((int) imm);
        break;
    case rvOPCODE::LW:
        ret += toString(rd) + ", " + std::to_string((int) imm) + "(" + toString(rs1) + ")";
        break;
    case rvOPCODE::SW:
        ret += toString(rs1) + ", " + std::to_string((int) imm) + "(" + toString(rs2) + ")";
        break;
    case rvOPCODE::BEQ:
    case rvOPCODE::BNE:
    case rvOPCODE::BLT:
    case rvOPCODE::BGE:
    case rvOPCODE::BLTU:
    case rvOPCODE::BGEU:
        ret += toString(rs1) + ", " + toString(rs2) + ", " + label;
        break;
    case rvOPCODE::JAL:
        assert("no jal");
        break;
    case rvOPCODE::JALR:
        assert("no jalr");
        break;
    case rvOPCODE::LA:
        ret += toString(rd) + ", " + symbol;
        break;
    case rvOPCODE::LI:
        ret += toString(rd) + ", " + std::to_string((int) imm);
        break;
    case rvOPCODE::MOV:
        ret += toString(rd) + ", " + toString(rs1);
        break;
    case rvOPCODE::J:
        ret += label;
        break;
    }

    return ret;
}

// stackVarMap
backend::stackVarMap::stackVarMap(): offset(-20) {}

// reg allocator
backend::regAllocator::regAllocator(std::vector<rv::rv_inst*> &rv_insts_out): reg2op_map(32), freg2fop_map(32), reg_timestamp(32), freg_timestamp(32), rv_insts(rv_insts_out) {}

void backend::regAllocator::update(rv::rvREG r, int time) {
    auto it = reg_using.find(std::make_pair(reg_timestamp[(int) r], r));
    reg_using.erase(it);
    reg_timestamp[(int) r] = time;
    reg_using.emplace(time, r);
}

void backend::regAllocator::update(rv::rvFREG r, int time) {
    auto it = freg_using.find(std::make_pair(freg_timestamp[(int) r], r));
    freg_using.erase(it);
    freg_timestamp[(int) r] = time;
    freg_using.emplace(time, r);
}

void backend::regAllocator::spill(rvREG r) {
    Operand operand = reg2op_map[(int) r];
    if(operand.name == "null") {
        return;
    }
    int pos = stack_var_map.find_operand(operand);

    rv_inst* store_inst = new rv_inst();
    store_inst->op = rvOPCODE::SW;
    store_inst->rs1 = rvREG::X8;
    store_inst->rs2 = r;
    store_inst->imm = pos;
    // sw r, pos(s0)
    rv_insts.push_back(store_inst);

    available_regs.insert(r);
    op2reg_map.erase(op2reg_map.find(operand));
    reg2op_map[(int) r] = Operand();
}

void backend::regAllocator::spill(rvFREG r) {
    Operand operand = freg2fop_map[(int) r];
    if(operand.name == "null") {
        return;
    }
    int pos = stack_var_map.find_operand(operand);

    rv_inst* store_inst = new rv_inst();
    store_inst->op = rvOPCODE::FSW;
    store_inst->frs1 = rvREG::X8;
    store_inst->rs2 = r;
    store_inst->imm = pos;
    // fsw r, pos(s0)
    rv_insts.push_back(store_inst);

    available_fregs.insert(r);
    fop2freg_map.erase(fop2freg_map.find(operand));
    freg2fop_map[(int) r] = Operand();
}


backend::Generator::Generator(ir::Program& p, std::ofstream& f): program(p), fout(f), reg_allocator(rv_insts) {}

void backend::Generator::gen() {
    TODO;
}