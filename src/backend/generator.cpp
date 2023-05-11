#include"backend/generator.h"

#include<assert.h>
#include<iostream>
#include<algorithm>

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
    case rvOPCODE::SEQZ:
        return "seqz";
    case rvOPCODE::SNEZ:
        return "snez";
    case rvOPCODE::SLTZ:
        return "sltz";
    case rvOPCODE::SGTZ:
        return "sgtz";
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
    case rvOPCODE::BNEZ:
        return "bnez";
    case rvOPCODE::JAL:
        return "jal";
    case rvOPCODE::JALR:
        return "jalr";
    case rvOPCODE::LA:
        return "la";
    case rvOPCODE::LI:
        return "li";
    case rvOPCODE::MOV:
        return "mv";
    case rvOPCODE::J:
        return "j";
    case rvOPCODE::JR:
        return "jr";
    default:
        std::cerr << (int) op << "\n";
        assert(0 && "gg");
        break;
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
    case rvOPCODE::SEQZ:
    case rvOPCODE::SNEZ:
    case rvOPCODE::SLTZ:
    case rvOPCODE::SGTZ:
        ret += toString(rd) + ", " + toString(rs1);
        break;
    case rvOPCODE::LW:
        ret += toString(rd) + ", " + std::to_string((int) imm) + "(" + toString(rs1) + ")";
        break;
    case rvOPCODE::SW:
        ret += toString(rs2) + ", " + std::to_string((int) imm) + "(" + toString(rs1) + ")";
        break;
    case rvOPCODE::BEQ:
    case rvOPCODE::BNE:
    case rvOPCODE::BLT:
    case rvOPCODE::BGE:
    case rvOPCODE::BLTU:
    case rvOPCODE::BGEU:
        ret += toString(rs1) + ", " + toString(rs2) + ", " + label;
        break;
    case rvOPCODE::BNEZ:
        ret += toString(rs1) + ", " + label;
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
    case rvOPCODE::JR:
        ret += toString(rs1);
        break;
    default:
        std::cerr << (int) op << "\n";
        assert(0 && "gg");
        break;
    }

    return ret;
}

rv::rv_inst::rv_inst(): is_label(false) {}
rv::rv_inst::rv_inst(string label_name): is_label(true), label(label_name) {}


// operand cmp
bool backend::operandCmp::operator() (const ir::Operand& a, const ir::Operand& b) const {
    return a.name < b.name;
}

// stackVarMap
backend::stackVarMap::stackVarMap(): offset(-16) {}

int backend::stackVarMap::add_operand(Operand op, uint32_t size) { 
    // op.type == Ptr: alloc
    // op.type == Int or Float: called by find_operand
    // store in spill
    // only alloc
    offset -= size;
    _table[op] = offset;
    return offset;
}

int backend::stackVarMap::find_operand(Operand op) {
    auto it = _table.find(op);
    if(it == _table.end()) {
        // op should be Type::Int or Type::Float when op not exist in stack.
        // because Ptr should alloc first, then Ptr is in stack.
        assert(op.type == Type::Float || op.type == Type::Int);
        return add_operand(op);
    } else {
        return it->second;
    }
}

// reg allocator
backend::regAllocator::regAllocator(std::vector<rv::rv_inst*> &rv_insts_out): reg2op_map(32), freg2fop_map(32), reg_timestamp(32), freg_timestamp(32), rv_insts(rv_insts_out) {
    available_regs.insert(rvREG::X5);
    available_regs.insert(rvREG::X6);
    available_regs.insert(rvREG::X7);
    for(int i = 10; i <= 17; i++) {
        available_regs.insert((rvREG) i);
    }
}

void backend::regAllocator::update(rv::rvREG r, int time) {
    auto it = reg_using.find(std::make_pair(reg_timestamp[(int) r], r));
    // r should in using
    assert(it != reg_using.end());

    reg_using.erase(it);
    reg_timestamp[(int) r] = time;
    reg_using.emplace(time, r);
}

void backend::regAllocator::update(rv::rvFREG r, int time) {
    auto it = freg_using.find(std::make_pair(freg_timestamp[(int) r], r));
    freg_using.erase(it);
    // r should in using
    assert(it != freg_using.end());

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
    store_inst->rs1 = rvREG::X8;
    store_inst->frs2 = r;
    store_inst->imm = pos;
    // fsw r, pos(s0)
    rv_insts.push_back(store_inst);

    available_fregs.insert(r);
    fop2freg_map.erase(fop2freg_map.find(operand));
    freg2fop_map[(int) r] = Operand();
}

void backend::regAllocator::load(rvREG r, ir::Operand op, int time) {
    assert(available_regs.count(r));
    available_regs.erase(r);
    reg_using.emplace(reg_timestamp[(int) r], r);
    update(r, time);

    // find op in op2reg
    if(op2reg_map.count(op)) {
        rv_inst* mv = new rv_inst();
        mv->op = rvOPCODE::MOV;
        mv->rs1 = op2reg_map[op];
        mv->rd = r;
        rv_insts.push_back(mv);

        available_regs.insert(mv->rs1);
        op2reg_map[op] = r;
        reg2op_map[(int) mv->rs1] = Operand();
        reg2op_map[(int) r] = op;
    } else {
        int pos = stack_var_map.find_operand(op);
        op2reg_map[op] = r;
        reg2op_map[(int) r] = op;

        rv_inst* load = new rv_inst();
        load->op = rvOPCODE::LW;
        load->rs1 = rvREG::X8;
        load->imm = pos;
        load->rd = r;
        rv_insts.push_back(load);
    }
}

void backend::regAllocator::load(rvFREG r, ir::Operand op, int time) {
    assert(available_fregs.count(r));
    available_fregs.erase(r);
    freg_using.emplace(freg_timestamp[(int) r], r);
    update(r, time);

    // find op in op2reg
    if(fop2freg_map.count(op)) {
        rv_inst* mv = new rv_inst();
        mv->op = rvOPCODE::FMOV;
        mv->frs1 = fop2freg_map[op];
        mv->frd = r;
        rv_insts.push_back(mv);

        available_fregs.insert(mv->frs1);
        fop2freg_map[op] = r;
        freg2fop_map[(int) mv->frs1] = Operand();
        freg2fop_map[(int) r] = op;
    } else {
        int pos = stack_var_map.find_operand(op);
        fop2freg_map[op] = r;
        freg2fop_map[(int) r] = op;

        rv_inst* load = new rv_inst();
        load->op = rvOPCODE::FLW;
        load->rs1 = rvREG::X8;
        load->imm = pos;
        load->frd = r;
        rv_insts.push_back(load);
    }
}

rvREG backend::regAllocator::getReg(Operand op, int time) {
    // first check whether op has a reg
    if(op2reg_map.count(op)) {
        // op has reg, return reg belong to it
        rvREG r = op2reg_map[op];
        update(r, time);
        return r;
    }
    if(available_regs.size()) {
        // have available regs, directly alloc available reg to op
        rvREG r = *available_regs.begin();
        load(r, op, time);
        return r;
    }
    // no available regs
    // spill the LRU opreg
    rvREG r;
    int last_time;
    std::tie(last_time, r) = *reg_using.begin();
    reg_using.erase(reg_using.begin());
    spill(r);
    
    // load to r
    load(r, op, time);
    return r;
}

rvFREG backend::regAllocator::fgetReg(Operand op, int time) {
    // first check whether op has a reg
    if(fop2freg_map.count(op)) {
        // op has reg, return reg belong to it
        rvFREG r = fop2freg_map[op];
        update(r, time);
        return r;
    }
    if(available_fregs.size()) {
        // have available regs, directly alloc available reg to op
        rvFREG r = *available_fregs.begin();
        load(r, op, time);
        return r;
    }
    // no available regs
    // spill the LRU opreg
    rvFREG r;
    int last_time;
    std::tie(last_time, r) = *freg_using.begin();
    freg_using.erase(freg_using.begin());
    spill(r);
    
    // load to r
    load(r, op, time);
    return r;
}

// gen
backend::Generator::Generator(ir::Program& p, std::ofstream& f): program(p), fout(f) {}

int backend::Generator::get_label_id(int line_num) {
    auto it = std::lower_bound(goto_label_lines->begin(), goto_label_lines->end(), line_num);
    if(it == goto_label_lines->end() || *it != line_num) return -1;
    return (int) (it - goto_label_lines->begin());
}

void backend::Generator::gen() {
    // do global things
    fout << "\t.text\n";
    for(auto i : program.globalVal) {
        fout << "\t.comm\t" + i.val.name + "," + std::to_string(i.maxlen * 4) + ",4" + "\n";
    }
    for(auto i : program.functions) {
        gen_func(i);
    }

}

void backend::Generator::gen_func(const Function& func) {
    fout << "\t.align\t1\n\t.global\t" + func.name + "\n\t.type\t" + func.name + ", @function\n" + func.name + ":\n";
    
    rv_insts = new std::vector<rv_inst*>();
    reg_allocator = new regAllocator(*rv_insts);
    goto_label_lines = new std::vector<int>();
    auto &ngoto_label_lines = *goto_label_lines;
    ret_set = new std::set<rv_inst*>();

    // abi get func parameters
    int apr = 0, fapr = 0, stackpr = 0;
    for(int i = 0; i < func.ParameterList.size(); i++) {
        Operand op = func.ParameterList[i];
        if(op.type == Type::Int || op.type == Type::IntPtr) {
            if(apr <= 7) {
                reg_allocator->reg2op_map[apr] = op;
                reg_allocator->op2reg_map[op] = (rvREG) apr;
                reg_allocator->available_regs.erase((rvREG) apr);
                reg_allocator->reg_using.emplace(reg_allocator->reg_timestamp[apr], (rvREG) apr);
                apr++;
            } else {
                reg_allocator->stack_var_map._table[op] = stackpr * 4;
                stackpr++;
            }
        } else {
            if(fapr <= 7) {
                reg_allocator->freg2fop_map[fapr] = op;
                reg_allocator->fop2freg_map[op] = (rvFREG) fapr;
                reg_allocator->available_fregs.erase((rvFREG) fapr);
                reg_allocator->freg_using.emplace(reg_allocator->freg_timestamp[fapr], (rvFREG) fapr);
                apr++;
            } else {
                reg_allocator->stack_var_map._table[op] = stackpr * 4;
                stackpr++;
            }
        }
    }

    // sp -= frame size
    rv_inst *spsub_inst = new rv_inst();
    spsub_inst->op = rvOPCODE::ADDI;
    spsub_inst->rd = rvREG::X2;
    spsub_inst->rs1 = rvREG::X2;
    // sp = sp + ?
    rv_insts->push_back(spsub_inst);
    
    // save ra
    rv_inst *svra_inst = new rv_inst();
    svra_inst->op = rvOPCODE::SW;
    svra_inst->rs2 = rvREG::X1;
    svra_inst->rs1 = rvREG::X2;
    // sw ra, ?-4(sp)
    rv_insts->push_back(svra_inst);

    // save s0
    rv_inst *svs0_inst = new rv_inst();
    svs0_inst->op = rvOPCODE::SW;
    svs0_inst->rs2 = rvREG::X8;
    svs0_inst->rs1 = rvREG::X2;
    // sw ra, ?-8(sp)
    rv_insts->push_back(svs0_inst);

    // s0 = sp + frame size
    rv_inst *spadd_inst = new rv_inst();
    spadd_inst->op = rvOPCODE::ADDI;
    spadd_inst->rd = rvREG::X8;
    spadd_inst->rs1 = rvREG::X2;
    // s0 = sp + ?
    rv_insts->push_back(spadd_inst);


    // unique goto labels
    for(int i = 0; i < func.InstVec.size(); i++) {
        Instruction* inst = func.InstVec[i];
        
        if(inst->op == Operator::_goto) {
            ngoto_label_lines.push_back(i + stoi(inst->des.name));
        }
    }
    std::sort(ngoto_label_lines.begin(), ngoto_label_lines.end());
    ngoto_label_lines.erase(std::unique(ngoto_label_lines.begin(), ngoto_label_lines.end()), ngoto_label_lines.end());
    
    max_call_overflow_paras = 0;
    for(int i = 0; i < func.InstVec.size(); i++) {
        int label_id = -1;
        if((label_id = get_label_id(i)) != -1) {
            rv_insts->push_back(new rv_inst(".L" + std::to_string(label_id)));
            // break;
        }
        Instruction* inst = func.InstVec[i];
        std::cerr << inst->draw() << "\n";
        gen_instr(*inst, i);
    }

    // write back
    spadd_inst->imm = -(spsub_inst->imm = reg_allocator->stack_var_map.offset);
    svra_inst->imm = -reg_allocator->stack_var_map.offset - 4;
    svs0_inst->imm = -reg_allocator->stack_var_map.offset - 8;

    for(rv_inst* reti : *ret_set) {
        if(reti->rd == rvREG::X2) {
            reti->imm = -reg_allocator->stack_var_map.offset;
        } else if(reti->rd == rvREG::X8) {
            reti->imm = -reg_allocator->stack_var_map.offset - 8;
        } else {
            reti->imm = -reg_allocator->stack_var_map.offset - 4;
        }
    }

    for(int i = 0; i < rv_insts->size(); i++) {
    std::cerr<< "\t.size\t" << rv_insts->size() << "now:" << i << "\n";
    std::cerr << "\t" << (*rv_insts)[i]->draw() << "\n";
        if((*rv_insts)[i]->is_label) {
            fout << (*rv_insts)[i]->label << ":\n";
        } else{
            fout << "\t" << (*rv_insts)[i]->draw() << "\n";
        }
    }

    fout << "\t.size\t" + func.name + ", .-" + func.name + "\n";
    delete rv_insts;
    delete reg_allocator;
    delete goto_label_lines;
    delete ret_set;
}

void backend::Generator::gen_instr(const Instruction& inst, int time) {
    switch(inst.op) {
        case Operator::mov:
        case Operator::def: {
            if(inst.op1.type == Type::IntLiteral) {
                rvREG r = reg_allocator->getReg(inst.des, time);

                rv_inst *li_inst = new rv_inst();
                li_inst->op = rvOPCODE::LI;
                li_inst->imm = stoi(inst.op1.name);
                li_inst->rd = r;
                // li r, op1.name
                rv_insts->push_back(li_inst);
            } else {
                rvREG rs1 = reg_allocator->getReg(inst.op1, time);
                rvREG rd = reg_allocator->getReg(inst.des, time);

                rv_inst *mv_inst = new rv_inst();
                mv_inst->op = rvOPCODE::MOV;
                mv_inst->rs1 = rs1;
                mv_inst->rd = rd;
                // li r, op1.name
                rv_insts->push_back(mv_inst);
            }
        } break;

        case Operator::sub:
        case Operator::add: {
            // add rd, rs, rt
            rvREG rs1 = reg_allocator->getReg(inst.op1, time);
            rvREG rs2 = reg_allocator->getReg(inst.op2, time);
            rvREG rd = reg_allocator->getReg(inst.des, time);

            rv_inst *op_inst = new rv_inst();
            switch(inst.op) {
                case Operator::add: op_inst->op = rvOPCODE::ADD; break;
                case Operator::sub: op_inst->op = rvOPCODE::SUB; break;
                default:
                    assert(0);
                    break;
            }
            op_inst->rs1 = rs1;
            op_inst->rs2 = rs2;
            op_inst->rd = rd;
            // add rd, rs1, rs2
            rv_insts->push_back(op_inst);
        } break;
        
        case Operator::_return: {
            if(inst.op1.name == "null") {
                // do nothing
            } else if(inst.op1.type == Type::Int) {
                reg_allocator->spill(rvREG::X10);
                reg_allocator->load(rvREG::X10, inst.op1, time);
            } else if(inst.op1.type == Type::Float) {
                reg_allocator->spill(rvFREG::F10);
                reg_allocator->load(rvFREG::F10, inst.op1, time);
            } else if(inst.op1.type == Type::IntLiteral) {
                reg_allocator->spill(rvREG::X10);
                rv_inst* li_inst = new rv_inst();
                li_inst->op = rvOPCODE::LI;
                li_inst->rd = rvREG::X10;
                li_inst->imm = stoi(inst.op1.name);
                rv_insts->push_back(li_inst);
            } else if(inst.op1.type == Type::FloatLiteral) {
                TODO;
            }
        
            // save s0
            rv_inst *lds0_inst = new rv_inst();
            lds0_inst->op = rvOPCODE::LW;
            lds0_inst->rd = rvREG::X8;
            lds0_inst->rs1 = rvREG::X2;
            // sw ra, ?-8(sp)
            rv_insts->push_back(lds0_inst);
            ret_set->insert(lds0_inst);

            // load ra
            rv_inst *ldra_inst = new rv_inst();
            ldra_inst->op = rvOPCODE::LW;
            ldra_inst->rd = rvREG::X1;
            ldra_inst->rs1 = rvREG::X2;
            // sw ra, ?-4(sp)
            rv_insts->push_back(ldra_inst);
            ret_set->insert(ldra_inst);

            // s0 = sp + frame size
            rv_inst *spadd_inst = new rv_inst();
            spadd_inst->op = rvOPCODE::ADDI;
            spadd_inst->rd = rvREG::X2;
            spadd_inst->rs1 = rvREG::X2;
            // s0 = sp + ?
            rv_insts->push_back(spadd_inst);

            // jr ra
            rv_inst *jr_inst = new rv_inst();
            jr_inst->op = rvOPCODE::JR;
            jr_inst->rs1 = rvREG::X1;
            // s0 = sp + ?
            rv_insts->push_back(jr_inst);
        } break;

        case Operator::_goto: {
            rv_inst *goto_inst = new rv_inst();
            if(inst.op1.name == "null") {
                goto_inst->op = rvOPCODE::J;
            } else {
                goto_inst->op = rvOPCODE::BNEZ;
                goto_inst->rs1 = reg_allocator->getReg(inst.op1, time);
            }
            goto_inst->label = ".L" + std::to_string(get_label_id(time + stoi(inst.des.name)));
            rv_insts->push_back(goto_inst);
        } break;

        case Operator::call: {
            CallInst *callinst = (CallInst*) &inst;
            // TODO: 这里不应该是-8，因为有Int和Float，但是测试点没有
            max_call_overflow_paras = std::max(max_call_overflow_paras, (int) callinst->argumentList.size() - 8);
        } break;

        case Operator::neq:
        case Operator::lss:
        case Operator::gtr:
        case Operator::eq: {
            // des = op1 == 0

            rvREG rs1 = reg_allocator->getReg(inst.op1, time);
            rvREG rd = reg_allocator->getReg(inst.des, time);

            rv_inst *op_inst = new rv_inst();
            switch(inst.op) {
                case Operator::neq: op_inst->op = rvOPCODE::SNEZ; break;
                case Operator::lss: op_inst->op = rvOPCODE::SLTZ; break;
                case Operator::gtr: op_inst->op = rvOPCODE::SGTZ; break;
                case Operator::eq: op_inst->op = rvOPCODE::SEQZ; break;
                default:
                    assert(0);
                    break;
            }
            
            op_inst->rs1 = rs1;
            op_inst->rd = rd;
            // seqz des, op1
            rv_insts->push_back(op_inst);

        } break;

        

        default:
            assert(0 && "invalid operator");
            break;
    }

}