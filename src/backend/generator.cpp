#include"backend/generator.h"
#include"front/semantic.h"

#include<assert.h>
#include<iostream>
#include<algorithm>
#include<iomanip>

using std::string;
using namespace rv;
using namespace ir;

using std::cerr;
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
    cerr << "rv freg tostring:" << (int) r << "\n";
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
    case rvOPCODE::MUL:
        return "mul";
    case rvOPCODE::DIV:
        return "div";
    case rvOPCODE::REM:
        return "rem";
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
    case rvOPCODE::CALL:
        return "call";
    case rvOPCODE::FLW:
        return "flw";
    case rvOPCODE::FSW:
        return "fsw";
    case rvOPCODE::FLSS:
        return "flt.s";
    case rvOPCODE::FLEQ:
        return "fle.s";
    case rvOPCODE::FEQ:
        return "feq.s";
    case rvOPCODE::FCVTSW:
        return "fcvt.s.w";
    case rvOPCODE::FCVTWS:
        return "fcvt.w.s";
    case rvOPCODE::FADD:
        return "fadd.s";
    case rvOPCODE::FSUB:
        return "fsub.s";
    case rvOPCODE::FDIV:
        return "fdiv.s";
    case rvOPCODE::FMUL:
        return "fmul.s";
    case rvOPCODE::FMOV:
        return "fmv.s";
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
    case rvOPCODE::MUL:
    case rvOPCODE::DIV:
    case rvOPCODE::REM:
        ret += toString(rd) + ", " + toString(rs1) + ", " + toString(rs2);
        break;
    case rvOPCODE::FADD:
    case rvOPCODE::FSUB:
    case rvOPCODE::FMUL:
    case rvOPCODE::FDIV:
        ret += toString(frd) + ", " + toString(frs1) + ", " + toString(frs2);
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
        if(imm == INT32_MAX) {
            ret += toString(rd) + ", " + symbol;
        } else {
            ret += toString(rd) + ", " + std::to_string((int) imm) + "(" + toString(rs1) + ")";
        }
        break;
    case rvOPCODE::FLW:
        if(imm == INT32_MAX) {
            ret += toString(frd) + ", " + symbol + ", " + toString(rvREG::X31);
        } else {
            ret += toString(frd) + ", " + std::to_string((int) imm) + "(" + toString(rs1) + ")";
        }
        break;
    case rvOPCODE::SW:
        if(imm == INT32_MAX) {
            ret += toString(rs2) + ", " + symbol + ", " + toString(rvREG::X31);
        } else {
            ret += toString(rs2) + ", " + std::to_string((int) imm) + "(" + toString(rs1) + ")";
        }
        break;
    case rvOPCODE::FSW:
        if(imm == INT32_MAX) {
            ret += toString(frs2) + ", " + symbol + ", " + toString(rvREG::X31);
        } else {
            ret += toString(frs2) + ", " + std::to_string((int) imm) + "(" + toString(rs1) + ")";
        }
        break;
    case rvOPCODE::FLSS:
    case rvOPCODE::FLEQ:
    case rvOPCODE::FEQ:
        ret += toString(rd) + ", " + toString(frs1) + ", " + toString(frs2);
        break;
    case rvOPCODE::FCVTSW:
        ret += toString(frd) + ", " + toString(rs1);
        break;
    case rvOPCODE::FCVTWS:
        ret += toString(rd) + ", " + toString(frs1) + ", rtz";
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
    case rvOPCODE::CALL:
        ret += label;
        break;
    case rvOPCODE::FMOV:
        ret += toString(frd) + ", " + toString(frs1);
        break;
    default:
//        std::cerr << (int) op << "\n";
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
backend::stackVarMap::stackVarMap(const std::vector<ir::GlobalVal> &gb): offset(-16), globalVal(gb) {}

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
        // find op in globalvar
        for(GlobalVal gv : globalVal) {
            if(gv.val.name == op.name) {
                return INT32_MAX;
            }
        }
    //    std::cerr << "op.name:" << op.name << ' ' << "op.type:" << (int) op.type << "\n";
        return add_operand(op);
    } else {
        return it->second;
    }
}

// reg allocator
backend::regAllocator::regAllocator(std::vector<rv::rv_inst*> &rv_insts_out, const std::vector<ir::GlobalVal> &globalVal_out): reg2op_map(32), freg2fop_map(32), reg_timestamp(32), freg_timestamp(32), rv_insts(rv_insts_out), stack_var_map(globalVal_out) {
    add_availables();
}

void backend::regAllocator::add_availables() {
    available_regs.insert(rvREG::X5);
    available_regs.insert(rvREG::X6);
    available_regs.insert(rvREG::X7);
    for(int i = 10; i <= 17; i++) {
        available_regs.insert((rvREG) i);
    }

    // ft0~ft7
    for(int i = 0; i <= 7; i++) {
        available_fregs.insert((rvFREG) i);
    }
    // fa0~fa7
    for(int i = 10; i <= 17; i++) {
        available_fregs.insert((rvFREG) i);
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
    if(pos == INT32_MAX) {
        store_inst->symbol = operand.name;
        // 如果是全局变量 且 是指针，才不用加rv_insts sw
    }
    // sw r, pos(s0)
    if(!(pos == INT32_MAX && (operand.type == Type::IntPtr || operand.type == Type::FloatPtr))) {
        rv_insts.push_back(store_inst);
    }

    available_regs.insert(r);
    op2reg_map.erase(op2reg_map.find(operand));
    reg_using.erase(reg_using.find({reg_timestamp[(int) r], r}));
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
    if(pos == INT32_MAX) {
        store_inst->symbol = operand.name;
    }
    // fsw r, pos(s0)
    rv_insts.push_back(store_inst);

    available_fregs.insert(r);
    fop2freg_map.erase(fop2freg_map.find(operand));
    freg_using.erase(freg_using.find({freg_timestamp[(int) r], r}));
    freg2fop_map[(int) r] = Operand();
}

void backend::regAllocator::load(rvREG r, ir::Operand op, int time, int needload) {
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
        if(needload) {
            rv_insts.push_back(mv);
        }

        available_regs.insert(mv->rs1);
        op2reg_map[op] = r;
        reg_using.erase(reg_using.find({reg_timestamp[(int) mv->rs1], mv->rs1}));
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
        if(pos == INT32_MAX) {
            load->symbol = op.name;
            if(op.type == Type::IntPtr || op.type == Type::FloatPtr) {
                load->op = rvOPCODE::LA;
            }
        }
        if(needload) {
            rv_insts.push_back(load);
        }
    }
}

void backend::regAllocator::load(rvFREG r, ir::Operand op, int time, int needload) {
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
        if(needload) rv_insts.push_back(mv);

        available_fregs.insert(mv->frs1);
        fop2freg_map[op] = r;
        freg_using.erase(freg_using.find({reg_timestamp[(int) mv->frs1], mv->frs1}));
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
        if(pos == INT32_MAX) {
            load->symbol = op.name;
        }
        if(needload) rv_insts.push_back(load);
    }
}

rvREG backend::regAllocator::getReg(Operand op, int time, int needload) {
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
        load(r, op, time, needload);
        return r;
    }
    // no available regs
    // spill the LRU opreg
    rvREG r;
    int last_time;
    std::tie(last_time, r) = *reg_using.begin();
    // std::cerr << "fuck\n";
    spill(r);
    
    // load to r
    load(r, op, time, needload);
    return r;
}

rvFREG backend::regAllocator::fgetReg(Operand op, int time, int needload) {
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
        load(r, op, time, needload);
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
    load(r, op, time, needload);
    return r;
}

void backend::regAllocator::clearregs() {
    op2reg_map.clear();
    fop2freg_map.clear();
    reg2op_map = std::vector<ir::Operand>(32);
    freg2fop_map = std::vector<ir::Operand>(32);

    available_regs.clear();
    available_fregs.clear();
    add_availables();
    // LRU Cache
    reg_using.clear();
    freg_using.clear();
    reg_timestamp = std::vector<int>(32);
    freg_timestamp = std::vector<int>(32);
}

// float const table
string backend::FloatConstTable::add_float_const(float fx) {
    // cerr << "add " << std::fixed << std::setprecision(6) << fx << "!\n";
    assert(!float_const_to_string_map.count(fx));
    string ret = (float_const_to_string_map[fx] = ".LC" + std::to_string(float_const_to_string_map.size()));
    return ret;
}
string backend::FloatConstTable::find_float_const(float fx) {
    if(float_const_to_string_map.count(fx)) {
        return float_const_to_string_map[fx];
    } else {
        // cerr << "not found float const in map!\n";
        return add_float_const(fx);
    }
}

backend::FloatConstTable::FloatConstTable(std::ofstream &ost): fout(ost) {}

void backend::FloatConstTable::gen_rodata() {
    fout << "\t.section\t.rodata\n";
    auto getword = [&] (float fx) -> unsigned int {
        return *reinterpret_cast<unsigned int*>(&fx);
    };
    for(auto it : float_const_to_string_map) {
        float val;
        string label;
        tie(val, label) = it;
        fout << "\t.align\t2\n";
        fout << label << ":\n";
        fout << "\t.word\t" << getword(val) << "\n";
    }
}

// gen
backend::Generator::Generator(ir::Program& p, std::ofstream& f): program(p), fout(f), global_label_id(0) {}

int backend::Generator::get_label_id(int line_num) {
    auto it = std::lower_bound(goto_label_lines->begin(), goto_label_lines->end(), line_num);
    if(it == goto_label_lines->end() || *it != line_num) return -1;
    return (int) (it - goto_label_lines->begin() + global_label_id);
}

void backend::Generator::gen() {
    // init float const table
    float_const_table = new FloatConstTable(fout);
    // do global things
    fout << "\t.text\n";
    for(auto i : program.globalVal) {
        fout << "\t.comm\t" + i.val.name + "," + std::to_string(std::max(1, i.maxlen) * 4) + ",4" + "\n";
    }
    for(auto i : program.functions) {
        gen_func(i);
    }
    float_const_table->gen_rodata();
    delete float_const_table;
}

void backend::Generator::gen_func(const Function& func) {
    fout << "\t.align\t1\n\t.global\t" + func.name + "\n\t.type\t" + func.name + ", @function\n" + func.name + ":\n";
    
    rv_insts = new std::vector<rv_inst*>();
    reg_allocator = new regAllocator(*rv_insts, program.globalVal);
    goto_label_lines = new std::vector<int>();
    auto &ngoto_label_lines = *goto_label_lines;
    ret_set = new std::set<rv_inst*>();

    // abi get func parameters
    int apr = 10, fapr = 10, stackpr = 0;
    for(int i = 0; i < func.ParameterList.size(); i++) {
        Operand op = func.ParameterList[i];
        if(op.type == Type::Int || op.type == Type::IntPtr || op.type == Type::FloatPtr) {
            if(apr <= 17) {
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
            if(fapr <= 17) {
                reg_allocator->freg2fop_map[fapr] = op;
                reg_allocator->fop2freg_map[op] = (rvFREG) fapr;
                reg_allocator->available_fregs.erase((rvFREG) fapr);
                reg_allocator->freg_using.emplace(reg_allocator->freg_timestamp[fapr], (rvFREG) fapr);
                fapr++;
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
    
    // gen insts
    max_call_overflow_paras = 0;
    for(int i = 0; i < func.InstVec.size(); i++) {
        int label_id = -1;
        if((label_id = get_label_id(i)) != -1) {
            // store all temp
            std::vector<rvREG> store_regs;
            std::vector<rvFREG> store_fregs;
            // std::cerr << "reg_using.size():" << reg_allocator->reg_using.size() << "\n";
            for(auto reg_info : reg_allocator->reg_using) {
                    // std::cerr << (int) reg_info.second << "mp:" << reg_allocator->reg2op_map[(int) reg_info.second].name << "\n";
                store_regs.push_back(reg_info.second);
            }
            for(auto reg_info : reg_allocator->freg_using) {
                store_fregs.push_back(reg_info.second);
            }
            for(auto reg : store_regs) {
                reg_allocator->spill(reg);
            }
            for(auto reg : store_fregs) {
                reg_allocator->spill(reg);
            }
            rv_insts->push_back(new rv_inst(".L" + std::to_string(label_id)));
            reg_allocator->clearregs();
            // break;
        }
        Instruction* inst = func.InstVec[i];
        std::cerr << inst->draw() << "\n";
        if(inst->op == Operator::call) {
            int b = 1;
        }
        gen_instr(*inst, i);
    }

    // write back
    if((reg_allocator->stack_var_map.offset - max_call_overflow_paras * 4) % 8 != 0) {
        max_call_overflow_paras++;
    }
    spadd_inst->imm = -(spsub_inst->imm = reg_allocator->stack_var_map.offset - max_call_overflow_paras * 4);
    svra_inst->imm = spadd_inst->imm - 4;
    svs0_inst->imm = spadd_inst->imm - 8;

    for(rv_inst* reti : *ret_set) {
        if(reti->rd == rvREG::X2) {
            reti->imm = spadd_inst->imm;
        } else if(reti->rd == rvREG::X8) {
            reti->imm = spadd_inst->imm - 8;
        } else {
            reti->imm = spadd_inst->imm - 4;
        }
    }

    for(int i = 0; i < rv_insts->size(); i++) {
//    std::cerr<< "\t.size\t" << rv_insts->size() << "now:" << i << "\n";
//    std::cerr << "\t" << (*rv_insts)[i]->draw() << "\n";
        if((*rv_insts)[i]->is_label) {
            fout << (*rv_insts)[i]->label << ":\n";
        } else{
            fout << "\t" << (*rv_insts)[i]->draw() << "\n";
        }
    }

    fout << "\t.size\t" + func.name + ", .-" + func.name + "\n";
    global_label_id += ngoto_label_lines.size();
    delete rv_insts;
    delete reg_allocator;
    delete goto_label_lines;
    delete ret_set;
}

void backend::Generator::gen_instr(const Instruction& inst, int time) {
    switch(inst.op) {
        case Operator::alloc: {
            reg_allocator->stack_var_map.offset -= stoi(inst.op1.name) * 4;
            rvREG r = reg_allocator->getReg(inst.des, time, 0);

            rv_inst* addi_inst = new rv_inst();
            addi_inst->op = rvOPCODE::ADDI;
            addi_inst->rd = r;
            addi_inst->rs1 = rvREG::X8;
            addi_inst->imm = reg_allocator->stack_var_map.offset + 4;
            rv_insts->push_back(addi_inst);
        } break;

        case Operator::load: {
            // TODO: add float case
            // assert(inst.op1.type == Type::IntPtr);
            /*
            mv t6, op2
            sll t6, t6, 2
            add t6, t6, op1
            lw des, 0(t6)
            */
            rv_inst* mv_inst = new rv_inst();
            mv_inst->op = rvOPCODE::MOV;
            mv_inst->rd = rvREG::X31;
            mv_inst->rs1 = reg_allocator->getReg(inst.op2, time, 1);
            rv_insts->push_back(mv_inst);

            rv_inst* slli_inst = new rv_inst();
            slli_inst->op = rvOPCODE::SLLI;
            slli_inst->rd = rvREG::X31;
            slli_inst->rs1 = rvREG::X31;
            slli_inst->imm = 2;
            rv_insts->push_back(slli_inst);

            rv_inst* add_inst = new rv_inst();
            add_inst->op = rvOPCODE::ADD;
            add_inst->rd = rvREG::X31;
            add_inst->rs1 = rvREG::X31;
            add_inst->rs2 = reg_allocator->getReg(inst.op1, time, 1);
            rv_insts->push_back(add_inst);

            rv_inst* lw_inst = new rv_inst();
            if(inst.des.type == Type::Float) {
                lw_inst->op = rvOPCODE::FLW;
                lw_inst->frd = reg_allocator->fgetReg(inst.des, time, 0);
            } else {
                lw_inst->op = rvOPCODE::LW;
                lw_inst->rd = reg_allocator->getReg(inst.des, time, 0);
            }
            lw_inst->rs1 = rvREG::X31;
            
            lw_inst->imm = 0;
            rv_insts->push_back(lw_inst);
        } break;

        case Operator::store: {
            // TODO: add float case
            // assert(inst.op1.type == Type::IntPtr);
            /*
            mv t6, op2
            sll t6, t6, 2
            add t6, t6, op1
            lw des, 0(t6)
            */
            if(inst.op2.type == Type::Int) {
                rv_inst* mv_inst = new rv_inst();
                mv_inst->op = rvOPCODE::MOV;
                mv_inst->rd = rvREG::X31;
                mv_inst->rs1 = reg_allocator->getReg(inst.op2, time, 1);
                rv_insts->push_back(mv_inst);

                rv_inst* slli_inst = new rv_inst();
                slli_inst->op = rvOPCODE::SLLI;
                slli_inst->rd = rvREG::X31;
                slli_inst->rs1 = rvREG::X31;
                slli_inst->imm = 2;
                rv_insts->push_back(slli_inst);

                rv_inst* add_inst = new rv_inst();
                add_inst->op = rvOPCODE::ADD;
                add_inst->rd = rvREG::X31;
                add_inst->rs1 = rvREG::X31;
                add_inst->rs2 = reg_allocator->getReg(inst.op1, time, 1);
                rv_insts->push_back(add_inst);

                rv_inst* sw_inst = new rv_inst();
                if(inst.des.type == Type::Float) {
                    sw_inst->op = rvOPCODE::FSW;
                    sw_inst->frs2 = reg_allocator->fgetReg(inst.des, time, 1);
                } else {
                    sw_inst->op = rvOPCODE::SW;
                    sw_inst->rs2 = reg_allocator->getReg(inst.des, time, 1);
                }
                sw_inst->rs1 = rvREG::X31;
                sw_inst->imm = 0;
                rv_insts->push_back(sw_inst);
            } else {
                rv_inst* sw_inst = new rv_inst();
                sw_inst->rs1 = reg_allocator->getReg(inst.op1, time, 1);
                if(inst.des.type == Type::Float) {
                    sw_inst->op = rvOPCODE::FSW;
                    sw_inst->frs2 = reg_allocator->fgetReg(inst.des, time, 1);
                } else {
                    sw_inst->op = rvOPCODE::SW;
                    sw_inst->rs2 = reg_allocator->getReg(inst.des, time, 1);
                }
                sw_inst->imm = stoi(inst.op2.name) * 4;
                rv_insts->push_back(sw_inst);
            }
        } break;

        case Operator::mov:
        case Operator::def: {
            if(inst.op1.type == Type::IntLiteral) {
                rvREG r = reg_allocator->getReg(inst.des, time, 0);

                rv_inst *li_inst = new rv_inst();
                li_inst->op = rvOPCODE::LI;
                li_inst->imm = stoi(inst.op1.name);
                li_inst->rd = r;
                // li r, op1.name
                rv_insts->push_back(li_inst);
            } else {
                // std::cerr << "inst.op1:" << inst.op1.name << " \n";
                rvREG rs1 = reg_allocator->getReg(inst.op1, time, 1);
                rvREG rd = reg_allocator->getReg(inst.des, time, 0);

                rv_inst *mv_inst = new rv_inst();
                mv_inst->op = rvOPCODE::MOV;
                mv_inst->rs1 = rs1;
                mv_inst->rd = rd;
                // li r, op1.name
                rv_insts->push_back(mv_inst);
            }
        } break;

        case Operator::fmov:
        case Operator::fdef: {
            if(inst.op1.type == Type::FloatLiteral) {
                rvFREG r = reg_allocator->fgetReg(inst.des, time, 0);

                rv_inst *flw_inst = new rv_inst();
                flw_inst->op = rvOPCODE::FLW;
                flw_inst->symbol = float_const_table->find_float_const(stof(inst.op1.name));
                flw_inst->frd = r;
                flw_inst->imm = INT32_MAX;
                // li r, op1.name
                rv_insts->push_back(flw_inst);
            } else {
                // std::cerr << "inst.op1:" << inst.op1.name << " \n";
                rvFREG frs1 = reg_allocator->fgetReg(inst.op1, time, 1);
                rvFREG frd = reg_allocator->fgetReg(inst.des, time, 0);

                rv_inst *mv_inst = new rv_inst();
                mv_inst->op = rvOPCODE::FMOV;
                mv_inst->frs1 = frs1;
                mv_inst->frd = frd;
                // li r, op1.name
                rv_insts->push_back(mv_inst);
            }
        } break;

        case Operator::mul:
        case Operator::div:
        case Operator::mod:
        case Operator::sub:
        case Operator::add: {
            // add rd, rs, rt
            rvREG rs1 = reg_allocator->getReg(inst.op1, time, 1);
            rvREG rs2 = reg_allocator->getReg(inst.op2, time, 1);
            rvREG rd = reg_allocator->getReg(inst.des, time, 0);

            rv_inst *op_inst = new rv_inst();
            switch(inst.op) {
                case Operator::add: op_inst->op = rvOPCODE::ADD; break;
                case Operator::sub: op_inst->op = rvOPCODE::SUB; break;
                case Operator::mul: op_inst->op = rvOPCODE::MUL; break;
                case Operator::div: op_inst->op = rvOPCODE::DIV; break;
                case Operator::mod: op_inst->op = rvOPCODE::REM; break;
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

        case Operator::fmul:
        case Operator::fdiv:
        case Operator::fsub:
        case Operator::fadd: {
            // add rd, rs, rt
            rvFREG frs1 = reg_allocator->fgetReg(inst.op1, time, 1);
            rvFREG frs2 = reg_allocator->fgetReg(inst.op2, time, 1);
            rvFREG frd = reg_allocator->fgetReg(inst.des, time, 0);

            rv_inst *op_inst = new rv_inst();
            switch(inst.op) {
                case Operator::fadd: op_inst->op = rvOPCODE::FADD; break;
                case Operator::fsub: op_inst->op = rvOPCODE::FSUB; break;
                case Operator::fmul: op_inst->op = rvOPCODE::FMUL; break;
                case Operator::fdiv: op_inst->op = rvOPCODE::FDIV; break;
                default:
                    assert(0);
                    break;
            }
            op_inst->frs1 = frs1;
            op_inst->frs2 = frs2;
            op_inst->frd = frd;
            // add rd, rs1, rs2
            rv_insts->push_back(op_inst);
        } break;
        
        case Operator::_return: {
            // store all temp
            std::vector<rvREG> store_regs;
            std::vector<rvFREG> store_fregs;
            for(auto reg_info : reg_allocator->reg_using) {
                    // std::cerr << (int) reg_info.second << "mp:" << reg_allocator->reg2op_map[(int) reg_info.second].name << "\n";
                if(reg_allocator->stack_var_map.find_operand(reg_allocator->reg2op_map[(int) reg_info.second]) == INT32_MAX) {
                    store_regs.push_back(reg_info.second);
                }
            }
            for(auto reg_info : reg_allocator->freg_using) {
                // if(reg_allocator->stack_var_map.find_operand(reg_allocator->freg2fop_map[(int) reg_info.second]) == INT32_MAX) {
                    store_fregs.push_back(reg_info.second);
                // }
            }
            for(auto reg : store_regs) {
                reg_allocator->spill(reg);
            }
            for(auto reg : store_fregs) {
                reg_allocator->spill(reg);
            }

            if(inst.op1.name == "null") {
                // do nothing
            } else if(inst.op1.type == Type::Int) {
                reg_allocator->spill(rvREG::X10);
                reg_allocator->load(rvREG::X10, inst.op1, time, 1);
            } else if(inst.op1.type == Type::Float) {
                reg_allocator->spill(rvFREG::F10);
                reg_allocator->load(rvFREG::F10, inst.op1, time, 1);
            } else if(inst.op1.type == Type::IntLiteral) {
                reg_allocator->spill(rvREG::X10);
                rv_inst* li_inst = new rv_inst();
                li_inst->op = rvOPCODE::LI;
                li_inst->rd = rvREG::X10;
                li_inst->imm = stoi(inst.op1.name);
                rv_insts->push_back(li_inst);
            } else if(inst.op1.type == Type::FloatLiteral) {
                reg_allocator->spill(rvFREG::F10);
                rv_inst* lw_inst = new rv_inst();
                lw_inst->op = rvOPCODE::FLW;
                lw_inst->frd = rvFREG::F10;
                lw_inst->symbol = float_const_table->find_float_const(stof(inst.op1.name));
                lw_inst->imm = INT32_MAX;
                rv_insts->push_back(lw_inst);
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
            ret_set->insert(spadd_inst);

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
                goto_inst->rs1 = reg_allocator->getReg(inst.op1, time, 0);
                // std::cerr << "goto condition:" << toString(goto_inst->rs1) << "\n";
            }
            goto_inst->label = ".L" + std::to_string(get_label_id(time + stoi(inst.des.name)));
            // store all temp
            std::vector<rvREG> store_regs;
            std::vector<rvFREG> store_fregs;
            // std::cerr << "reg_using.size():" << reg_allocator->reg_using.size() << "\n";
            for(auto reg_info : reg_allocator->reg_using) {
                    // std::cerr << (int) reg_info.second << "mp:" << reg_allocator->reg2op_map[(int) reg_info.second].name << "\n";
                store_regs.push_back(reg_info.second);
            }
            for(auto reg_info : reg_allocator->freg_using) {
                store_fregs.push_back(reg_info.second);
            }
            for(auto reg : store_regs) {
                reg_allocator->spill(reg);
            }
            for(auto reg : store_fregs) {
                reg_allocator->spill(reg);
            }
            rv_insts->push_back(goto_inst);
            // std::cerr << "goto:" << goto_inst->draw() << "\n";
        } break;

        case Operator::call: {
            CallInst *callinst = (CallInst*) &inst;
            // TODO: 这里不应该是-8，因为有Int和Float，但是测试点没有
            int intcnt = 0, floatcnt = 0;
            for(Operand op : callinst->argumentList) {
                if(op.type == Type::Int) intcnt++;
                else floatcnt++;
            }
            max_call_overflow_paras = std::max(max_call_overflow_paras, std::max(intcnt - 8, 0) + std::max(floatcnt - 8, 0));

            // store all temp
            std::vector<rvREG> store_regs;
            std::vector<rvFREG> store_fregs;
            for(auto reg_info : reg_allocator->reg_using) {
                store_regs.push_back(reg_info.second);
            }
            for(auto reg_info : reg_allocator->freg_using) {
                store_fregs.push_back(reg_info.second);
            }
            for(auto reg : store_regs) {
                reg_allocator->spill(reg);
            }
            for(auto reg : store_fregs) {
                reg_allocator->spill(reg);
            }

            // load arguments to a0~a7
            // abi get func parameters
            // 如果参数个数过多，则spill到栈上，此时不能随便使用a0~a7，因此统一使用t0来存取
            int apr = 10, fapr = 10, stackpr = 0;
            for(int i = 0; i < callinst->argumentList.size(); i++) {
                Operand op = callinst->argumentList[i];
                if(op.type == Type::Int || op.type == Type::IntPtr || op.type == Type::FloatPtr) {
                    if(apr <= 17) {
                        reg_allocator->load((rvREG) apr, op, time, 1);
                        apr++;
                    } else {
                        reg_allocator->load(rvREG::X5, op, time, 1);

                        rv_inst* sw_inst = new rv_inst();
                        sw_inst->op = rvOPCODE::SW;
                        sw_inst->rs2 = rvREG::X5;
                        sw_inst->rs1 = rvREG::X2;
                        sw_inst->imm = stackpr * 4;
                        rv_insts->push_back(sw_inst);

                        reg_allocator->spill(rvREG::X5);
                        // TODO: optimizer

                        stackpr++;
                    }
                } else {
                    if(fapr <= 17) {
                        reg_allocator->load((rvFREG) fapr, op, time, 1);
                        fapr++;
                    } else {
                        // assume not go to here
                        TODO;
                        stackpr++;
                    }
                }
            }

            // call function
            rv_inst* call_inst = new rv_inst();
            call_inst->op = rvOPCODE::CALL;
            call_inst->label = inst.op1.name;
            rv_insts->push_back(call_inst);

            // process return val
            Function calledFunction;
            int foundFunction = 0;
            for(Function f : program.functions) {
                if(f.name == inst.op1.name) {
                    calledFunction = f;
                    foundFunction = 1;
                    break;
                }
            }
            if(!foundFunction) {
                calledFunction = *frontend::get_lib_funcs()->find(inst.op1.name)->second;
            }
            reg_allocator->clearregs();
            if(calledFunction.returnType == Type::Int) {
                // std::cerr << "fuck\n";
                rv_inst* mv_inst = new rv_inst();
                mv_inst->op = rvOPCODE::MOV;
                mv_inst->rs1 = rvREG::X10;
                mv_inst->rd = reg_allocator->getReg(inst.des, time, 0);
                rv_insts->push_back(mv_inst);
                // reg_allocator->reg2op_map[(int) rvREG::X10] = inst.des;
                // reg_allocator->op2reg_map[inst.des] = rvREG::X10;
                // reg_allocator->available_regs.erase(rvREG::X10);
                // reg_allocator->reg_using.emplace(reg_allocator->reg_timestamp[(int) rvREG::X10], rvREG::X10);
            } else if(calledFunction.returnType == Type::Float) {
                rv_inst* mv_inst = new rv_inst();
                mv_inst->op = rvOPCODE::FMOV;
                mv_inst->frs1 = rvFREG::F10;
                mv_inst->frd = reg_allocator->fgetReg(inst.des, time, 0);
                rv_insts->push_back(mv_inst);
                // reg_allocator->freg2fop_map[(int) rvFREG::F10] = inst.des;
                // reg_allocator->fop2freg_map[inst.des] = rvFREG::F10;
                // reg_allocator->available_fregs.erase(rvFREG::F10);
                // reg_allocator->freg_using.emplace(reg_allocator->freg_timestamp[(int) rvFREG::F10], rvFREG::F10);
            }

            
        } break;

        case Operator::neq:
        case Operator::lss:
        case Operator::gtr:
        case Operator::eq: {
            // des = op1 == 0

            rvREG rs1 = reg_allocator->getReg(inst.op1, time, 1);
            rvREG rd = reg_allocator->getReg(inst.des, time, 0);

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

        case Operator::flss:
        case Operator::fleq:
        case Operator::feq: {
            // des = op1 == 0

            rvFREG frs1 = reg_allocator->fgetReg(inst.op1, time, 1);
            rvFREG frs2 = reg_allocator->fgetReg(inst.op2, time, 1);
            rvFREG frd = reg_allocator->fgetReg(inst.des, time, 0);

            rv_inst *op_inst = new rv_inst();
            switch(inst.op) {
                case Operator::flss: op_inst->op = rvOPCODE::FLSS; break;
                case Operator::fleq: op_inst->op = rvOPCODE::FLEQ; break;
                case Operator::feq: op_inst->op = rvOPCODE::FEQ; break;
                default:
                    assert(0);
                    break;
            }
            
            op_inst->frs1 = frs1;
            op_inst->frs2 = frs2;
            op_inst->rd = rvREG::X31;

            rv_inst *back_inst = new rv_inst();
            back_inst->op = rvOPCODE::FCVTSW;
            back_inst->frd = frd;
            back_inst->rs1 = rvREG::X31;
            // seqz des, op1
            rv_insts->push_back(op_inst);
            rv_insts->push_back(back_inst);

        } break;

        case Operator::cvt_f2i: {
            rv_inst *cvt_inst = new rv_inst();
            cvt_inst->op = rvOPCODE::FCVTWS;
            cvt_inst->rd = reg_allocator->getReg(inst.des, time, 0);
            cvt_inst->frs1 = reg_allocator->fgetReg(inst.op1, time, 1);
            rv_insts->push_back(cvt_inst);
        } break;

        case Operator::cvt_i2f: {
            rv_inst *cvt_inst = new rv_inst();
            cvt_inst->op = rvOPCODE::FCVTSW;
            cvt_inst->frd = reg_allocator->fgetReg(inst.des, time, 0);
            cvt_inst->rs1 = reg_allocator->getReg(inst.op1, time, 1);
            rv_insts->push_back(cvt_inst);
        } break;

        default:
            std::cerr << "Operatorid:" << (int) inst.op << "\n";
            assert(0 && "invalid operator");
            break;
    }

}