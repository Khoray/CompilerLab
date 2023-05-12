#include"front/semantic.h"

#include<cassert>
#include<iostream>
#include<string>
#include<iomanip>

using ir::Instruction;
using ir::Function;
using ir::Operand;
using ir::Operator;

#define TODO assert(0 && "TODO");

#define GET_CHILD_PTR(node, type, index) auto node = dynamic_cast<type*>(root->children[index]); assert(node); 
#define ANALYSIS(node, type, index) auto node = dynamic_cast<type*>(root->children[index]); assert(node); analysis##type(node);
#define COPY_EXP_NODE(from, to) to->is_computable = from->is_computable; to->v = from->v; to->t = from->t;

// #define log(...) fprintf(stderr, "[%s:%d,function:%s]:", __FILE__, __LINE__, __FUNCTION__); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n")
#define log(...)


map<std::string,ir::Function*>* frontend::get_lib_funcs() {
    static map<std::string,ir::Function*> lib_funcs = {
        {"getint", new Function("getint", Type::Int)},
        {"getch", new Function("getch", Type::Int)},
        {"getfloat", new Function("getfloat", Type::Float)},
        {"getarray", new Function("getarray", {Operand("arr", Type::IntPtr)}, Type::Int)},
        {"getfarray", new Function("getfarray", {Operand("arr", Type::FloatPtr)}, Type::Int)},
        {"putint", new Function("putint", {Operand("i", Type::Int)}, Type::null)},
        {"putch", new Function("putch", {Operand("i", Type::Int)}, Type::null)},
        {"putfloat", new Function("putfloat", {Operand("f", Type::Float)}, Type::null)},
        {"putarray", new Function("putarray", {Operand("n", Type::Int), Operand("arr", Type::IntPtr)}, Type::null)},
        {"putfarray", new Function("putfarray", {Operand("n", Type::Int), Operand("arr", Type::FloatPtr)}, Type::null)},
    };
    return &lib_funcs;
}

void frontend::SymbolTable::add_scope(Block* node) {
    std::stringstream st;
    st << node;
    frontend::ScopeInfo si;
    // si.name = st.str().substr(2);
    si.name = std::to_string(scope_stack.size());
    scope_stack.push_back(si);
}

void frontend::SymbolTable::exit_scope() {
    scope_stack.pop_back();
}

string frontend::SymbolTable::get_scoped_name(string id) const {
    for(int i = scope_stack.size() - 1; i >= 0; i--) {
        if(scope_stack[i].table.count(id)) {
            return id + "_" + scope_stack[i].name;
        }
    }
    // 放过的都是临时变量！只在 PrimaryExp->LVal 不确定 LVal 是 ident 还是 临时变量的时候使用
    return id;
}

frontend::STE frontend::SymbolTable::get_ste(string id) const {
    for(int i = scope_stack.size() - 1; i >= 0; i--) {
        if(scope_stack[i].table.count(id)) {
            return scope_stack[i].table.find(id)->second;
        }
    }
    assert(0 && "gg");
    return STE();
}

frontend::Analyzer::Analyzer(): tmp_cnt(0), tmp_f_cnt(0), symbol_table(), current_func(nullptr) {
    ScopeInfo si;
    si.cnt = 0;
    si.name = "g";
    symbol_table.scope_stack.push_back(si);
}

ir::Program frontend::Analyzer::get_ir_program(CompUnit* root) {
    log("fuck");
    AnalyzeCompUnit(root);
    Function* globalFunc = new Function("global", ir::Type::null);
    for(auto i : g_init_inst) {
        globalFunc->addInst(i);
    }
    globalFunc->addInst(new Instruction(Operand(), Operand(), Operand(), Operator::_return));
    program.addFunction(*globalFunc);
    log("get_ir_program=%s", (program.functions[0].InstVec[0]->op2.name.c_str()));
    program.draw();
    return program;
}

std::string frontend::Analyzer::get_tmp_var() {
    return "t" + std::to_string(tmp_cnt++);
}

std::string frontend::Analyzer::get_tmp_f_var() {
    return "f" + std::to_string(tmp_f_cnt++);
}

void frontend::Analyzer::store_tmp() {
    tmp_stack.push_back(tmp_cnt);
    tmp_f_stack.push_back(tmp_f_cnt);
}

void frontend::Analyzer::restore_tmp() {
    tmp_cnt = tmp_stack.back();
    tmp_stack.pop_back();
    tmp_f_cnt = tmp_f_stack.back();
    tmp_f_stack.pop_back();
}

Operand frontend::Analyzer::literal_to_var(Operand op) {
    Operand des;
    if(op.type == Type::IntLiteral) {
        std::string temp = get_tmp_var();
        des = Operand(temp, Type::Int);
        insert_inst(new Instruction(
            op,
            Operand(),
            des,
            Operator::mov
        ));
    } else if(op.type == Type::FloatLiteral) {
        std::string temp = get_tmp_f_var();
        des = Operand(temp, Type::Float);
        insert_inst(new Instruction(
            op,
            Operand(),
            des,
            Operator::fmov
        ));
    }
    return des;
}

Operand frontend::Analyzer::float_to_int(Operand op) {
    assert(op.type == Type::Float && "in float to int, op type is not float");
    std::string temp = get_tmp_var();
    Operand des = Operand(temp, Type::Int);
    insert_inst(new Instruction(
        op,
        Operand(),
        des,
        Operator::cvt_f2i
    ));
    
    return des;
}

Operand frontend::Analyzer::int_to_float(Operand op) {
    assert(op.type == Type::Int && "in int to float, op type is not int");
    std::string temp = get_tmp_f_var();
    Operand des = Operand(temp, Type::Float);
    insert_inst(new Instruction(
        op,
        Operand(),
        des,
        Operator::cvt_i2f
    ));
    
    return des;
}


Operand frontend::Analyzer::convert_type(Operand op, Type target) {
    // assert(target == Type::Int || target == Type::Float);
    if(op.type == Type::Int) {
        if(target == Type::Int) {
            return op;
        } else if(target == Type::Float) {
            return int_to_float(op);
        } else {
            assert(0 && "invalid type");
        }
    } else if(op.type == Type::Float) {
        if(target == Type::Int) {
            return float_to_int(op);
        } else if(target == Type::Float) {
            return op;
        } else {
            assert(0 && "invalid type");
        }
    } else if(op.type == Type::IntLiteral) {
        if(target == Type::Int) {
            return literal_to_var(op);
        } else if(target == Type::Float) {
            return literal_to_var(Operand(op.name, Type::FloatLiteral));
        } else if(target == Type::IntLiteral) {
            return op;
        } else if(target == Type::FloatLiteral) {
            return Operand(op.name, Type::FloatLiteral);
        } else {
            assert(0 && "invalid type");
        }
    } else if(op.type == Type::FloatLiteral) {
        if(target == Type::Int) {
            int t = std::stof(op.name);
            return literal_to_var(Operand(std::to_string(t), Type::IntLiteral));
        } else if(target == Type::Float) {
            return literal_to_var(op);
        } else if(target == Type::IntLiteral) {
            int t = std::stof(op.name);
            return Operand(std::to_string(t), Type::IntLiteral);
        } else if(target == Type::FloatLiteral) {
            return op;
        } else {
            assert(0 && "invalid type");
        }
    } else {
        assert(0);
        return op;
    }
    assert(0);
    return op;
}

void frontend::Analyzer::insert_ste(std::string name, STE ste) {
    symbol_table.scope_stack.back().table[name] = ste;
    if(current_func == nullptr) {
        log("current_func == nullptr");
        ste.operand.name = ste.operand.name + "_g";
        if(ste.operand.type == Type::FloatPtr || ste.operand.type == Type::IntPtr) {
            int len = 1;
            for(int i : ste.dimension) {
                len *= i;
            }
            program.globalVal.emplace_back(ste.operand, len);
        } else if(ste.operand.type == Type::Float || ste.operand.type == Type::Int) {
            program.globalVal.emplace_back(ste.operand);
        } else {
            // do nothing
        }
    }
}

void frontend::Analyzer::insert_inst(Instruction* inst) {
    if(current_func == nullptr) {
        g_init_inst.push_back(inst);
    } else {
        current_func->addInst(inst);
    }
}

void frontend::Analyzer::addRelInst(Operand op1, Operand op2, Operand des, TokenType op) {
    assert(des.type == Type::Int);
    store_tmp();
    Operand tempvar = Operand(get_tmp_var(), Type::Int);
    insert_inst(new Instruction(op1, op2, tempvar, Operator::sub));
    switch(op) {
        case TokenType::EQL: {
            insert_inst(new Instruction(tempvar, Operand("0", Type::IntLiteral), des, Operator::eq));
        } break;

        case TokenType::NEQ: {
            insert_inst(new Instruction(tempvar, Operand("0", Type::IntLiteral), des, Operator::neq));
        } break;

        case TokenType::LSS: {
            insert_inst(new Instruction(tempvar, Operand("0", Type::IntLiteral), des, Operator::lss));
        } break;

        case TokenType::GTR: {
            insert_inst(new Instruction(tempvar, Operand("0", Type::IntLiteral), des, Operator::gtr));
        } break;

        case TokenType::LEQ: {
            insert_inst(new Instruction(tempvar, Operand("0", Type::IntLiteral), des, Operator::gtr));
            insert_inst(new Instruction(des, Operand("0", Type::IntLiteral), des, Operator::eq));
        } break;

        case TokenType::GEQ: {
            insert_inst(new Instruction(tempvar, Operand("0", Type::IntLiteral), des, Operator::lss));
            insert_inst(new Instruction(des, Operand("0", Type::IntLiteral), des, Operator::eq));
        } break;
        
        default:
            TODO;
            break;
        }
        
    restore_tmp();
}

void frontend::Analyzer::addFRelInst(Operand op1, Operand op2, Operand des, TokenType op) {
    assert(des.type == Type::Float);
    store_tmp();
    switch(op) {
        case TokenType::EQL: {
            insert_inst(new Instruction(op1, op2, des, Operator::feq));
        } break;

        case TokenType::NEQ: {
            insert_inst(new Instruction(op1, op2, des, Operator::fneq));
        } break;

        case TokenType::LSS: {
            insert_inst(new Instruction(op1, op2, des, Operator::flss));
        } break;

        case TokenType::GTR: {
            insert_inst(new Instruction(op2, op1, des, Operator::flss));
        } break;

        case TokenType::LEQ: {
            insert_inst(new Instruction(op1, op2, des, Operator::fleq));
        } break;

        case TokenType::GEQ: {
            insert_inst(new Instruction(op2, op1, des, Operator::fleq));
        } break;
        
        default:
            TODO;
            break;
        }
        
    restore_tmp();
}

void frontend::Analyzer::AnalyzeCompUnit(CompUnit* root) {
    log("fuck");
    for(int i = 0; i < (int) root->children.size(); i++) {
        AstNode* chroot = root->children[i];
        if(chroot->type == NodeType::DECL) {
            AnalyzeDecl((Decl*) chroot);
        } else if(chroot->type == NodeType::FUNCDEF) {
            AnalyzeFuncDef((FuncDef*) chroot);
            log("funcdef ir program2=%p", (program.functions[0].InstVec[0]));
        } else {
            AnalyzeCompUnit((CompUnit*) chroot);
        }
    }
}

void frontend::Analyzer::AnalyzeDecl(Decl* root) {
    store_tmp();
    log("decl");
    for(int i = 0; i < (int) root->children.size(); i++) {
        AstNode* chroot = root->children[i];
        if(chroot->type == NodeType::CONSTDECL) {
            AnalyzeConstDecl((ConstDecl*) chroot);
        } else if(chroot->type == NodeType::VARDECL) {
            AnalyzeVarDecl((VarDecl*) chroot);
        }
    }
    restore_tmp();
}

void frontend::Analyzer::AnalyzeConstDecl(ConstDecl* root) {
    log("ConstDef");
    AnalyzeBType(((BType*) root->children[1]));
    root->t = ((BType*) root->children[1])->t;
    for(int i = 2; i < (int) root->children.size(); i += 2) {
        AstNode* chroot = root->children[i];
        ((ConstDef*) chroot)->t = ((BType*) root->children[1])->t;
        AnalyzeConstDef((ConstDef*) chroot);
    }
}

void frontend::Analyzer::AnalyzeBType(BType* root) {
    Term* chroot = (Term*) root->children[0];
    if(chroot->token.type == TokenType::INTTK) {
        root->t = Type::Int;
    } else if(chroot->token.type == TokenType::FLOATTK) {
        root->t = Type::Float;
    }
}

void frontend::Analyzer::AnalyzeConstDef(ConstDef* root) {
    bool is_ptr = ((Term*) root->children[1])->token.type == TokenType::LBRACK;
    int constExp_ptr = 2;
    std::vector<int> dimension;
    int len = 1;
    while(root->children[constExp_ptr]->type == NodeType::CONSTEXP) {
        AnalyzeConstExp((ConstExp*) root->children[constExp_ptr]);
        dimension.push_back(stoi(((ConstExp*) root->children[constExp_ptr])->v));
        constExp_ptr += 3;
    }
    log("ConstDef");
    // calc array len
    for(int i : dimension) {
        len *= i;
    }
    // insert ste
    STE ste;
    vector<int> dim; 
    int index = 0;
    if(is_ptr) {
        ste.operand = Operand(((Term*) root->children[0])->token.value, root->t == Type::Int ? Type::IntPtr : Type::FloatPtr);
        ste.dimension = dimension;
        // alloc memory to ste
        insert_ste(ste.operand.name, ste);
        Operand op1 = Operand(std::to_string(len), Type::IntLiteral);
        Operand op2 = Operand();
        Operand des = Operand(symbol_table.get_scoped_name(ste.operand.name), ste.operand.type);
        insert_inst(new Instruction(op1, op2, des, Operator::alloc));
        AnalyzeConstInitVal(index, ste, len, 0, (ConstInitVal*) root->children[constExp_ptr]);
    } else {
        ste.operand = Operand("assert", root->t == Type::Int ? Type::IntLiteral : Type::FloatLiteral);
        // 下面两句话顺序不能变，要先算出ste的值才能插入
        AnalyzeConstInitVal(index, ste, len, 0, (ConstInitVal*) root->children[constExp_ptr]);
        insert_ste(((Term*) root->children[0])->token.value, ste);
    }
    log("name=%s, type=%d, scope_name=%s, len=%d", ste.operand.name.c_str(), ste.operand.type, symbol_table.scope_stack.back().name.c_str(), len);

    // init val
    // TODO: array assign
}

void frontend::Analyzer::AnalyzeConstInitVal(int &index, STE& ste, int res, int level, ConstInitVal* root) {
    log("ConstInitVal, root->children[0]->type:%d", root->children[0]->type);
    store_tmp();
    if(root->children[0]->type == NodeType::TERMINAL) {
        // ConstInitVal -> { ConstInitVal* }
        int init_index = index;
        int ptr = 1;
        while(ptr < (int) root->children.size() && root->children[ptr]->type == NodeType::CONSTINITVAL) {
            AnalyzeConstInitVal(index, ste, res / ste.dimension[level], level + 1, (ConstInitVal*) root->children[ptr]);
            ptr += 2;
        }
        for(; index < init_index + res; index++) {
            Operand op1 = Operand(symbol_table.get_scoped_name(ste.operand.name), ste.operand.type);
            Operand op2 = Operand(std::to_string(index), Type::IntLiteral);
            Operand des = Operand("0", ste.operand.type == Type::IntPtr ? Type::IntLiteral : Type::FloatLiteral);
            store_tmp();
            des = literal_to_var(des);
            insert_inst(new Instruction(op1, op2, des, Operator::store));
            restore_tmp();
        }
    } else {
        // calc constexp val
        log("AnalyzeConstExp");
        ConstExp* constexp = ((ConstExp*) root->children[0]);
        AnalyzeConstExp(constexp);
        // store
        if(ste.operand.type == Type::IntPtr || ste.operand.type == Type::FloatPtr) {
            Operand op1 = Operand(symbol_table.get_scoped_name(ste.operand.name), ste.operand.type);
            Operand op2 = Operand(std::to_string(index), Type::IntLiteral);

            // store des should be int or float, should not be intliteral or float literal
            std::string temp = get_tmp_var();
            Operand des = Operand(constexp->v, constexp->t);
            store_tmp();
            des = literal_to_var(des);
            insert_inst(new Instruction(op1, op2, des, Operator::store));
            restore_tmp();
            index++;
        } else {
            ste.operand.name = ((ConstExp*) root->children[0])->v;
        }
    }
    restore_tmp();
}

void frontend::Analyzer::AnalyzeVarDecl(VarDecl* root) {
    log("VarDecl");
    store_tmp();
    BType* bc = (BType*) root->children[0];
    AnalyzeBType(bc);
    log("%d", bc->t);
    root->t = bc->t;
    for(int i = 1; i < (int) root->children.size(); i += 2) {
        VarDef* chroot = (VarDef*) root->children[i];
        chroot->t = bc->t;
        AnalyzeVarDef(chroot);
    }
    restore_tmp();
}

void frontend::Analyzer::AnalyzeVarDef(VarDef* root) {
    log("VarDef");
    store_tmp();
    bool is_ptr = root->children.size() > 1 && ((Term*) root->children[1])->token.type == TokenType::LBRACK;
    int constExp_ptr = 2;
    std::vector<int> dimension;
    int len = 1;
    while(constExp_ptr < (int) root->children.size() && root->children[constExp_ptr]->type == NodeType::CONSTEXP) {
        AnalyzeConstExp((ConstExp*) root->children[constExp_ptr]);
        dimension.push_back(std::stoi(((ConstExp*) root->children[constExp_ptr])->v));
        constExp_ptr += 3;
    }
    log("end of while");
    // calc array len
    for(int i : dimension) {
        len *= i;
    }
    // insert ste
    STE ste;
    if(is_ptr) {
        ste.operand = Operand(((Term*) root->children[0])->token.value, root->t == Type::Int ? Type::IntPtr : Type::FloatPtr);
        ste.dimension = dimension;
        insert_ste(ste.operand.name, ste);
        if(current_func != nullptr) {
            // 只有局部变量才需要alloc，全局变量不需要alloc
            Operand op1 = Operand(std::to_string(len), Type::IntLiteral);
            Operand op2 = Operand();
            Operand des = Operand(symbol_table.get_scoped_name(ste.operand.name), ste.operand.type);
            insert_inst(new Instruction(op1, op2, des, Operator::alloc));
        }
    } else {
        ste.operand = Operand(((Term*) root->children[0])->token.value, root->t);
        insert_ste(ste.operand.name, ste);
    }
    log("name=%s, type=%d, scope_name=%s", ste.operand.name.c_str(), ste.operand.type, symbol_table.scope_stack[1].name.c_str());

    // init val
    vector<int> dim;
    if(constExp_ptr < (int) root->children.size()) {
        int index = 0;
        AnalyzeInitVal(index, ste, len, 0, (InitVal*) root->children[constExp_ptr]);
    } else if(current_func == nullptr) {
        // 如果是全局变量，需要初始化成全0
        if(ste.operand.type == Type::IntPtr || ste.operand.type == Type::FloatPtr) {
            // 全局数组不需要初始化
            // for(int i = 0; i < len; i++) {
            //     Operand op1 = Operand(symbol_table.get_scoped_name(ste.operand.name), ste.operand.type);
            //     Operand op2 = Operand(std::to_string(i), Type::IntLiteral);
            //     Operand des = Operand("0", ste.operand.type == Type::IntPtr ? Type::IntLiteral : Type::FloatLiteral);
            //     store_tmp();
            //     des = literal_to_var(des);
            //     insert_inst(new Instruction(op1, op2, des, Operator::store));
            //     restore_tmp();
            // }
        } else {
            Operand op1 = Operand("0", ste.operand.type == Type::Int ? Type::IntLiteral : Type::FloatLiteral);
            Operand op2 = Operand();
            Operand des = Operand(symbol_table.get_scoped_name(ste.operand.name), ste.operand.type);
            insert_inst(new Instruction(op1, op2, des, Operator::def));
        }

    }
    
    // TODO: array assign
}

void frontend::Analyzer::AnalyzeInitVal(int& index, STE& ste, int res, int level, InitVal* root) {
    log("InitVal");
    store_tmp();
    if(root->children[0]->type == NodeType::TERMINAL) {
        int ptr = 1;
        int init_index = index;
        while(ptr < (int) root->children.size() && root->children[ptr]->type == NodeType::INITVAL) {
            AnalyzeInitVal(index, ste, res / ste.dimension[level], level + 1, (InitVal*) root->children[ptr]);
            ptr += 2;
        }
        for(; index < init_index + res; index++) {
            Operand op1 = Operand(symbol_table.get_scoped_name(ste.operand.name), ste.operand.type);
            Operand op2 = Operand(std::to_string(index), Type::IntLiteral);
            Operand des = Operand("0", ste.operand.type == Type::IntPtr ? Type::IntLiteral : Type::FloatLiteral);
            store_tmp();
            des = literal_to_var(des);
            insert_inst(new Instruction(op1, op2, des, Operator::store));
            restore_tmp();
        }
    } else {
        // calc constexp val
        
        Exp* ch = (Exp*) root->children[0];
        AnalyzeExp(ch);

        // store
        if(ste.operand.type == Type::IntPtr || ste.operand.type == Type::FloatPtr) {
            Operand op1 = Operand(symbol_table.get_scoped_name(ste.operand.name), ste.operand.type);
            Operand op2 = Operand(std::to_string(index), Type::IntLiteral);
            if(ch->is_computable) {
                // store des should be int or float, should not be intliteral or float literal

                Operand des = Operand(ch->v, ste.operand.type == Type::IntPtr ? Type::IntLiteral : Type::FloatLiteral);
                store_tmp();
                des = literal_to_var(des);

                insert_inst(new Instruction(op1, op2, des, Operator::store));
                restore_tmp();
            } else {
                Operand des = Operand(ch->v, ste.operand.type == Type::IntPtr ? Type::Int : Type::Float);
                insert_inst(new Instruction(op1, op1, des, Operator::store));
            }
            index++;
        } else {
            // 初始化
            Operand des = Operand(symbol_table.get_scoped_name(ste.operand.name), ste.operand.type);
            Operand op2 = Operand();
            if(ch->is_computable) {
                if(ste.operand.type == Type::Int) {
                    Operand op1 = Operand(ch->v, Type::IntLiteral);
                    insert_inst(new Instruction(op1, op2, des, Operator::def));
                } else {
                    Operand op1 = Operand(ch->v, Type::FloatLiteral);
                    insert_inst(new Instruction(op1, op2, des, Operator::fdef));
                }
            } else {
                if(ch->t == Type::Int) {
                    if(ste.operand.type == Type::Int) {
                        // int -> int
                        Operand op1 = Operand(ch->v, Type::Int);
                        insert_inst(new Instruction(op1, op2, des, Operator::def));
                    } else {
                        // int -> float
                        Operand op1 = Operand(ch->v, Type::Int);
                        op1 = int_to_float(op1);
                        insert_inst(new Instruction(op1, op2, des, Operator::fdef));
                    }
                } else {
                    if(ste.operand.type == Type::Int) {
                        // float -> int
                        Operand op1 = Operand(ch->v, Type::Float);
                        op1 = float_to_int(op1);
                        insert_inst(new Instruction(op1, op2, des, Operator::def));
                    } else {
                        // float -> float
                        Operand op1 = Operand(ch->v, Type::Float);
                        insert_inst(new Instruction(op1, op2, des, Operator::fdef));
                    }
                }
            }
        }
    }
    restore_tmp();
}

void frontend::Analyzer::AnalyzeFuncDef(FuncDef* root) {
    store_tmp();
    log("funcDef");
    FuncType* funcTypech = (FuncType*) root->children[0];
    AnalyzeFuncType(funcTypech);
    std::string funcname = ((Term*) root->children[1])->token.value;
    Type functype = funcTypech->t;
    Function *fun = new Function(funcname, functype);
    if(funcname == "main") {
        ir::CallInst* callGlobal = new ir::CallInst(ir::Operand("global",ir::Type::null), ir::Operand("t0",ir::Type::null));
        fun->addInst(callGlobal);
        
    }
    symbol_table.functions[funcname] = fun;
    current_func = fun;
    Block* funcBlock = (Block*) root->children.back();
    symbol_table.add_scope(funcBlock);
    if(root->children[3]->type != NodeType::TERMINAL) {
        // has [FuncFParams]
        AnalyzeFuncFParams((FuncFParams*) root->children[3]);
    }
    AnalyzeBlock(funcBlock);
    symbol_table.exit_scope();
    current_func->addInst(new Instruction(
        (functype == Type::null ? 
        Operand() 
        : functype == Type::Int ? 
        Operand("0", Type::IntLiteral) 
        : Operand("0", Type::FloatLiteral)),
        Operand(),
        Operand(),
        Operator::_return
    ));
    current_func = nullptr;
    program.addFunction(*fun);
    restore_tmp();
}

void frontend::Analyzer::AnalyzeFuncType(FuncType* root) {
    Term* chroot = (Term*) root->children[0];
    if(chroot->token.type == TokenType::INTTK) {
        root->t = Type::Int;
    } else if(chroot->token.type == TokenType::FLOATTK) {
        root->t = Type::Float;
    } else {
        root->t = Type::null;
    }
}

void frontend::Analyzer::AnalyzeFuncFParam(FuncFParam* root) {
    store_tmp();
    BType* Btypech = (BType*) root->children[0];
    AnalyzeBType(Btypech);
    std::string id = ((Term*) root->children[1])->token.value;
    bool is_ptr = false;
    STE ste;
    if(root->children.size() > 2) {
        is_ptr = true;
        ste.dimension.push_back(0);
        int exppr = 5;
        while(exppr < (int) root->children.size() && root->children[exppr]->type == NodeType::EXP) {
            Exp* exp = (Exp*) root->children[exppr];
            AnalyzeExp(exp);
            assert(exp->is_computable);
            ste.dimension.push_back(std::stoi(exp->v));
            exppr += 3;
        }
    }
    ste.operand = Operand(id, is_ptr ? Btypech->t == Type::Int ? Type::IntPtr : Type::FloatPtr : Btypech->t);
    insert_ste(ste.operand.name, ste);
    current_func->ParameterList.emplace_back(symbol_table.get_scoped_name(id), ste.operand.type);
    restore_tmp();
}

void frontend::Analyzer::AnalyzeFuncFParams(FuncFParams* root) {
    store_tmp();
    for(int i = 0; i < (int) root->children.size(); i += 2) {
        FuncFParam* ch = (FuncFParam*) root->children[i];
        AnalyzeFuncFParam(ch);
    }
    restore_tmp();
}

void frontend::Analyzer::AnalyzeBlock(Block* root) {
    log("Block");
    store_tmp();
    for(int i = 1; i < (int) root->children.size() - 1; i++) {
        AnalyzeBlockItem((BlockItem*) root->children[i]);
    }
    restore_tmp();
}

void frontend::Analyzer::AnalyzeBlockItem(BlockItem* root) {
    log("BlockItem");
    if(root->children[0]->type == NodeType::DECL) {
        AnalyzeDecl((Decl*) root->children[0]);
    } else {
        AnalyzeStmt((Stmt*) root->children[0]);
    }
}

void frontend::Analyzer::AnalyzeStmt(Stmt* root) {
    store_tmp();
    if(root->children[0]->type == NodeType::BLOCK) {
        Block* block = (Block*) root->children[0];
        symbol_table.add_scope(block);
        AnalyzeBlock(block);
        symbol_table.exit_scope();
    } else if(root->children[0]->type == NodeType::TERMINAL) {
        // break;
        if(((Term*) root->children[0])->token.type == TokenType::BREAKTK) {
            // break
            Operand op1 = Operand();
            Operand op2 = Operand();
            Operand des = Operand(std::to_string(current_func->InstVec.size()), Type::IntLiteral);
            Instruction* goto_inst = new Instruction(op1, op2, des, Operator::_goto);
            current_func->addInst(goto_inst);
            last_while.back()->jump_eow.insert(goto_inst);
        } else if(((Term*) root->children[0])->token.type == TokenType::CONTINUETK) {
            // continue
            Operand op1 = Operand();
            Operand op2 = Operand();
            Operand des = Operand(std::to_string(current_func->InstVec.size()), Type::IntLiteral);
            Instruction* goto_inst = new Instruction(op1, op2, des, Operator::_goto);
            current_func->addInst(goto_inst);
            last_while.back()->jump_bow.insert(goto_inst);
        } else if(((Term*) root->children[0])->token.type == TokenType::IFTK) {
            // if
            // calc cond
            Cond* cond = (Cond*) root->children[2];
            last_cond = cond;
            AnalyzeCond(cond);
            last_cond = nullptr;
            Instruction* elseinst = nullptr;
            Instruction* breakelseinst = nullptr;
            {
                Operand op1 = Operand();
                Operand op2 = Operand();
                Operand des = Operand(std::to_string(current_func->InstVec.size()), Type::IntLiteral);
                elseinst = new Instruction(op1, op2, des, Operator::_goto);
                current_func->addInst(elseinst);
                // goto [pc, else]
            }

            // process goto_in
            for(Instruction *inst : cond->jump_in) {
                inst->des.name = std::to_string(current_func->InstVec.size() - std::stoi(inst->des.name));
            }

            AnalyzeStmt((Stmt*) root->children[4]);
            if(root->children.size() >= 6) {
                {
                    // end of if
                    Operand op1 = Operand();
                    Operand op2 = Operand();
                    Operand des = Operand(std::to_string(current_func->InstVec.size()), Type::IntLiteral);
                    breakelseinst = new Instruction(op1, op2, des, Operator::_goto);
                    current_func->addInst(breakelseinst);
                }
            }
            elseinst->des.name = std::to_string(current_func->InstVec.size() - std::stoi(elseinst->des.name));
            if(root->children.size() >= 6) {
                AnalyzeStmt((Stmt*) root->children[6]);
                breakelseinst->des.name = std::to_string(current_func->InstVec.size() - std::stoi(breakelseinst->des.name));
            }
        } else if(((Term*) root->children[0])->token.type == TokenType::WHILETK) {
            last_while.push_back(root);
            Cond* cond = (Cond*) root->children[2];
            last_cond = cond;

            int condpos = current_func->InstVec.size();
            AnalyzeCond(cond);
            last_cond = nullptr;
            Instruction* elseinst = nullptr;

            {
                Operand op1 = Operand();
                Operand op2 = Operand();
                Operand des = Operand(std::to_string(current_func->InstVec.size()), Type::IntLiteral);
                elseinst = new Instruction(op1, op2, des, Operator::_goto);
                current_func->addInst(elseinst);
                // goto [pc, else]
            }

            for(Instruction *inst : cond->jump_in) {
                inst->des.name = std::to_string(current_func->InstVec.size() - std::stoi(inst->des.name));
            }
            

            AnalyzeStmt((Stmt*) root->children[4]);
            insert_inst(new Instruction(
                Operand(), 
                Operand(), 
                Operand(std::to_string(condpos - (int) current_func->InstVec.size()), Type::IntLiteral), 
                Operator::_goto
            ));
            elseinst->des.name = std::to_string(current_func->InstVec.size() - std::stoi(elseinst->des.name));

            for(Instruction* inst : root->jump_eow) {
                inst->des.name = std::to_string(current_func->InstVec.size() - std::stoi(inst->des.name));
            }

            for(Instruction* inst : root->jump_bow) {
                inst->des.name = std::to_string(condpos - std::stoi(inst->des.name));
            }
            // end of while
            last_while.pop_back();
        } else if(((Term*) root->children[0])->token.type == TokenType::RETURNTK) {
            if(root->children[1]->type == NodeType::TERMINAL) {
                current_func->addInst(new Instruction(Operand(), Operand(), Operand(), Operator::_return));
            } else {
                Exp* exp = (Exp*) root->children[1];
                AnalyzeExp(exp);
                if(exp->t == Type::IntLiteral || exp->t == Type::FloatLiteral) {
                    Operand ret = Operand(exp->v, exp->t);
                    ret = convert_type(ret, current_func->returnType == Type::Int ? Type::IntLiteral : Type::FloatLiteral);
                    current_func->addInst(new Instruction(ret, Operand(), Operand(), Operator::_return));
                } else if(exp->t == current_func->returnType) {
                    current_func->addInst(new Instruction(Operand(exp->v, current_func->returnType), Operand(), Operand(), Operator::_return));
                } else {
                    if(exp->t == Type::Int) {
                        Operand retval = int_to_float(Operand(exp->v, exp->t));
                        current_func->addInst(new Instruction(retval, Operand(), Operand(), Operator::_return));
                    } else if(exp->t == Type::Float) {
                        Operand retval = float_to_int(Operand(exp->v, exp->t));
                        current_func->addInst(new Instruction(retval, Operand(), Operand(), Operator::_return));
                    }
                }
            }
        } else if(((Term*) root->children[0])->token.type == TokenType::SEMICN) {
            // do nothing
        }
    } else if(root->children[0]->type == NodeType::EXP) {
        AnalyzeExp((Exp*) root->children[0]);
    } else if(root->children[0]->type == NodeType::LVAL) {
        LVal* lval = (LVal*) root->children[0];
        AnalyzeLVal(lval, 0);
        Exp* exp = (Exp*) root->children[2];
        AnalyzeExp(exp);
        if(lval->i != "") {
            // lval是一个指针 case2.1

            // 目前只有int

            Operand op1 = Operand(symbol_table.get_scoped_name(lval->id), lval->t == Type::Int ? Type::IntPtr : Type::FloatPtr);
            Operand op2 = Operand(lval->i, Type::Int);
            Operand des = Operand(exp->v, exp->t);
            store_tmp();
            des = convert_type(des, lval->t);
            current_func->addInst(new Instruction(op1, op2, des, Operator::store));
            restore_tmp();
        } else {
            Operand op1 = Operand(exp->v, exp->t);
            Operand op2 = Operand();
            Operand des = Operand(symbol_table.get_scoped_name(lval->v), lval->t);
            current_func->addInst(new Instruction(op1, op2, des, lval->t == Type::Int ? Operator::mov : Operator::fmov));
        }
    }
    restore_tmp();
}

void frontend::Analyzer::AnalyzeExp(Exp* root) {
    AddExp* addExp = (AddExp*) root->children[0];
    AnalyzeAddExp(addExp);

    COPY_EXP_NODE(addExp, root);
}

void frontend::Analyzer::AnalyzeCond(Cond* root) {
    LOrExp* lorexp = (LOrExp*) root->children[0];
    AnalyzeLOrExp(lorexp);

    COPY_EXP_NODE(lorexp, root);
}

void frontend::Analyzer::AnalyzeLVal(LVal* root, int needload) {
    log("LVal");
    Term* idt = (Term*) root->children[0];
    bool is_ptr = root->children.size() > 1;

    log("LVal idt->token.value:%s", idt->token.value.c_str());
    STE ste = symbol_table.get_ste(idt->token.value);
    root->id = idt->token.value;
    
    if(is_ptr) {
        // case1.1 and case2.1
        // root->v 
        std::string tmp_int = get_tmp_var(), tmp_float = get_tmp_f_var();
        // root->v = get_tmp_var(); 先不赋值
        root->t = ste.operand.type == Type::IntPtr ? Type::Int : Type::Float;
        std::string index = get_tmp_var();
        store_tmp();
        Instruction *assignIndex = new Instruction(Operand("0", Type::IntLiteral), Operand(), Operand(index, Type::Int), Operator::mov);
        current_func->addInst(assignIndex);

        std::string tmp = get_tmp_var();
        for(int i = 2; i < (int) root->children.size(); i += 3) {
            Exp* exp = (Exp*) root->children[i];
            AnalyzeExp(exp);

            Instruction *movInst = new Instruction(Operand(exp->v, exp->t), Operand(), Operand(tmp, Type::Int), Operator::mov);
            current_func->addInst(movInst);

            Instruction *addInst = new Instruction(Operand(index, Type::Int), Operand(tmp, Type::Int), Operand(index, Type::Int), Operator::add);
            current_func->addInst(addInst);

            if(i + 3 < (int) root->children.size()) {
                Instruction *movInst2 = new Instruction(Operand(std::to_string(ste.dimension[(i + 1) / 3]), Type::IntLiteral), Operand(), Operand(tmp, Type::Int), Operator::mov);
                current_func->addInst(movInst2);

                Instruction *mulInst = new Instruction(Operand(index, Type::Int), Operand(tmp, Type::Int), Operand(index, Type::Int), Operator::mul);
                current_func->addInst(mulInst);
            }
        }
        if(needload) { 
            if(ste.operand.type == Type::IntPtr) {
                Instruction *loadInst = new Instruction(Operand(symbol_table.get_scoped_name(idt->token.value), Type::IntPtr), Operand(index, Type::Int), Operand(tmp_int, Type::Int), Operator::load);
                current_func->addInst(loadInst);
                root->v = tmp_int;
                root->t = Type::Int;
            } else {
                Instruction *loadInst = new Instruction(Operand(symbol_table.get_scoped_name(idt->token.value), Type::FloatPtr), Operand(index, Type::Int), Operand(tmp_float, Type::Float), Operator::load);
                current_func->addInst(loadInst);
                root->v = tmp_float;
                root->t = Type::Float;
            }

        }

        root->i = index;
        restore_tmp();

    } else {
        if(ste.operand.type == Type::IntLiteral) {
            // case 3
            root->is_computable = true;
            root->t = Type::IntLiteral;
            root->v = ste.operand.name;
        } else if(ste.operand.type == Type::Int) {
            // case1.2 and case2.2
            root->is_computable = false;
            root->t = Type::Int;
            root->v = ste.operand.name;
        } else if(ste.operand.type == Type::IntPtr) {
            root->is_computable = false;
            root->t = Type::IntPtr;
            root->v = ste.operand.name;
        }
        if(ste.operand.type == Type::FloatLiteral) {
            // case 3
            root->is_computable = true;
            root->t = Type::FloatLiteral;
            root->v = ste.operand.name;
        } else if(ste.operand.type == Type::Float) {
            // case1.2 and case2.2
            root->is_computable = false;
            root->t = Type::Float;
            root->v = ste.operand.name;
        } else if(ste.operand.type == Type::FloatPtr) {
            root->is_computable = false;
            root->t = Type::FloatPtr;
            root->v = ste.operand.name;
        }
    }
}

void frontend::Analyzer::AnalyzeNumber(Number* root) {
    Term* chroot = (Term*) root->children[0];
    if(chroot->token.type == TokenType::INTLTR) {
        root->t = Type::IntLiteral;
        root->v = chroot->token.value;
        std::istringstream is(root->v);
        int value = 0;
        if(root->v.find("0b")) {
            is >> std::setbase(2) >> value;
        } else if(root->v.find("0x")) {
            is >> std::setbase(16) >> value;
        } else if(root->v[0] == '0' && root->v.length() > 1) {
            is >> std::setbase(8) >> value;
        } else {
            is >> std::setbase(10) >> value;
        }
        root->v = std::to_string(value);
    } else if(chroot->token.type == TokenType::FLOATLTR) {
        root->t = Type::FloatLiteral;
        root->v = chroot->token.value;
    }
}

void frontend::Analyzer::AnalyzePrimaryExp(PrimaryExp* root) {
    log("primaryExp");
    if(root->children[0]->type == NodeType::TERMINAL) {
        Exp* exp = (Exp*) root->children[1];
        AnalyzeExp(exp);
        COPY_EXP_NODE(exp, root);
    } else if(root->children[0]->type == NodeType::NUMBER) {
        Number* number = (Number*) root->children[0];
        AnalyzeNumber(number);
        COPY_EXP_NODE(number, root); 
    } else {
        std::string tmp_int = get_tmp_var(), tmp_float = get_tmp_f_var();
        // root->v = ; 先不赋值
        store_tmp();
        LVal* lval = (LVal*) root->children[0];
        AnalyzeLVal(lval, 1);
        if(lval->t == Type::IntLiteral || lval->t == Type::FloatLiteral || lval->t == Type::IntPtr || lval->t == Type::FloatPtr) {
            COPY_EXP_NODE(lval, root); 
        } else if(lval->t == Type::Int) {
            root->v = tmp_int;
            // 现在只有int
            root->t = Type::Int;
            root->is_computable = false;
            Instruction* movinst = new Instruction(Operand(symbol_table.get_scoped_name(lval->v), lval->t), Operand(), Operand(root->v, root->t), Operator::mov);
            insert_inst(movinst);
            log(movinst->draw().c_str());
        } else if(lval->t == Type::Float) {
            root->v = tmp_float;
            root->t = Type::Float;
            root->is_computable = false;
            Instruction* movinst = new Instruction(Operand(symbol_table.get_scoped_name(lval->v), lval->t), Operand(), Operand(root->v, root->t), Operator::fmov);
            insert_inst(movinst);
            log(movinst->draw().c_str());
        }
        restore_tmp();
    }
}

void frontend::Analyzer::AnalyzeUnaryExp(UnaryExp* root) {
    log("UnaryExp");
    if(root->children[0]->type == NodeType::PRIMARYEXP) {
        PrimaryExp* primaryExp = (PrimaryExp*) root->children[0];
        AnalyzePrimaryExp(primaryExp);
        COPY_EXP_NODE(primaryExp, root);
    } else if(root->children[0]->type == NodeType::UNARYOP) {
        UnaryOp* unaryOp = (UnaryOp*) root->children[0];
        AnalyzeUnaryOp(unaryOp);
        UnaryExp* unaryExp = (UnaryExp*) root->children[1];
        AnalyzeUnaryExp(unaryExp);
        COPY_EXP_NODE(unaryExp, root); 
        if(unaryExp->is_computable == true) {
            if(unaryOp->op == TokenType::NOT) {
                root->v = root->v == "0" ? "1" : "0";
                root->t = Type::IntLiteral;
            } else if(unaryOp->op == TokenType::MINU) {
                if(root->t == Type::Float) {
                    root->v = std::to_string(-std::stof(root->v));
                } else {
                    root->v = std::to_string(-std::stoi(root->v));
                }
            }
        } else {
            log("UnaryExp not computable, func:%s, optype:%d", current_func->name.c_str(), unaryOp->op);
            if(unaryOp->op == TokenType::NOT) {
                insert_inst(new Instruction(Operand(root->v, root->t), Operand(), Operand(root->v, root->t), Operator::_not));
            } else if(unaryOp->op == TokenType::MINU) {
                store_tmp();
                if(root->t == Type::Int) {
                    std::string zero = get_tmp_var();
                    insert_inst(new Instruction(Operand("0", Type::IntLiteral), Operand(), Operand(zero, Type::Int), Operator::mov));
                    insert_inst(new Instruction(Operand(zero, Type::Int), Operand(root->v, root->t), Operand(root->v, root->t), Operator::sub));
                } else {
                    std::string zero = get_tmp_f_var();
                    insert_inst(new Instruction(Operand("0", Type::FloatLiteral), Operand(), Operand(zero, Type::Float), Operator::fmov));
                    insert_inst(new Instruction(Operand(zero, Type::Float), Operand(root->v, root->t), Operand(root->v, root->t), Operator::fsub));
                }
                restore_tmp();
            }
        }
    } else {
        std::string funcname = ((Term*) root->children[0])->token.value;
        log("[UnaryExp->ident(paras)]funcname=%s", funcname.c_str());
        std::vector<Operand> paras;
        std::string tmp_int = get_tmp_var(), tmp_float = get_tmp_f_var();
        std::string ret_tmp; // 先不赋值
        store_tmp();
        Function *func = nullptr;
        if(symbol_table.functions.count(funcname)) {
            func = symbol_table.functions[funcname];
        } else {
            func = get_lib_funcs()->find(funcname)->second;
        }
        Operand retOp;
        if(func->returnType == Type::Int || func->returnType == Type::null) {
            ret_tmp = tmp_int;
            retOp = Operand(ret_tmp, Type::Int);
        } else {
            ret_tmp = tmp_float;
            retOp = Operand(ret_tmp, Type::Float);
        }
        if(root->children[2]->type == NodeType::TERMINAL) {
            current_func->addInst(new ir::CallInst(Operand(funcname, func->returnType), retOp));
            
        } else {
            FuncRParams* funcRParams = (FuncRParams*) root->children[2];
            AnalyzeFuncRParams(paras, funcRParams);

            for(int i = 0; i < (int) paras.size(); i++) {
                Operand sc = func->ParameterList[i];
                Operand xc = paras[i];

                if(sc.type == Type::IntPtr || sc.type == Type::FloatPtr) {
                    // donothing
                } else {
                    paras[i] = convert_type(xc, sc.type);
                }
            }
            current_func->addInst(new ir::CallInst(Operand(funcname, func->returnType), paras, Operand(ret_tmp, func->returnType)));
        }
        root->t = func->returnType;
        root->v = ret_tmp;
        root->is_computable = false;
        restore_tmp();
    }
}

void frontend::Analyzer::AnalyzeUnaryOp(UnaryOp* root) {
    Term* chroot = (Term*) root->children[0];
    root->op = chroot->token.type;
    log("UnaryOp.type:%d", chroot->token.type);
}

void frontend::Analyzer::AnalyzeFuncRParams(std::vector<Operand> &paras, FuncRParams* root) {
    int exppr = 0;
    while(exppr < (int) root->children.size() && root->children[exppr]->type == NodeType::EXP) {
        Exp* exp = (Exp*) root->children[exppr];
        AnalyzeExp(exp);
        if(exp->t == Type::FloatPtr || exp->t == Type::IntPtr) {
            paras.emplace_back(symbol_table.get_scoped_name(exp->v), exp->t);
        } else {
            paras.emplace_back(exp->v, exp->t);
            // if(exp->t == Type::IntLiteral || exp->t == Type::FloatLiteral) {
            //     paras.emplace_back(literal_to_var(Operand(exp->v, exp->t)));
            // } else {
            //     paras.emplace_back(Operand(exp->v, exp->t));
            // }
        }
        exppr += 2;
    }
}

void frontend::Analyzer::AnalyzeMulExp(MulExp* root) {
    log("MulExp");
    if(root->children.size() == 1) { // 只有一个儿子 直接继承
        UnaryExp* unaryexp = (UnaryExp*) root->children[0];
        AnalyzeUnaryExp(unaryexp);
        COPY_EXP_NODE(unaryexp, root);
        return;
    }
    root->is_computable = true;
    std::string tmp_int = get_tmp_var(), tmp_float = get_tmp_f_var();
    // root->v = get_tmp_var(); 先不赋值
    store_tmp();
    int is_int = 1;
    for(int i = 0; i < (int) root->children.size(); i += 2) {
        UnaryExp* unaryexp = (UnaryExp*) root->children[i];
        AnalyzeUnaryExp(unaryexp);
        root->is_computable &= unaryexp->is_computable;
        is_int &= (unaryexp->t == Type::Int || unaryexp->t == Type::IntLiteral);
    }
    if(root->is_computable) {
        if(is_int) {
            int temp = std::stoi(((UnaryExp*) root->children[0])->v);
            for(int i = 2; i < (int) root->children.size(); i += 2) {
                UnaryExp* unaryexp = (UnaryExp*) root->children[i];
                Term* sign = (Term*) root->children[i - 1];
                int nowval = std::stoi(unaryexp->v);
                if(sign->token.type == TokenType::MULT) {
                    temp *= nowval;
                } else if(sign->token.type == TokenType::DIV) {
                    temp /= nowval;
                } else if(sign->token.type == TokenType::MOD) {
                    temp %= nowval;
                }
            }
            root->v = std::to_string(temp);
            root->t = Type::IntLiteral;
        } else {
            float temp = std::stof(((UnaryExp*) root->children[0])->v);
            for(int i = 2; i < (int) root->children.size(); i += 2) {
                UnaryExp* unaryexp = (UnaryExp*) root->children[i];
                Term* sign = (Term*) root->children[i - 1];
                float nowval = std::stof(unaryexp->v);
                if(sign->token.type == TokenType::MULT) {
                    temp *= nowval;
                } else if(sign->token.type == TokenType::DIV) {
                    temp /= nowval;
                } else if(sign->token.type == TokenType::MOD) {
                    assert(0 && "float can't mod");
                }
            }
            root->v = std::to_string(temp);
            root->t = Type::FloatLiteral;
        }
    } else {
        if(is_int) {
            root->v = tmp_int;
            root->t = Type::Int;
            UnaryExp* initu = (UnaryExp*) root->children[0];
            Instruction* movinst = new Instruction(Operand(initu->v, initu->t), Operand(), Operand(root->v, root->t), Operator::mov);
            insert_inst(movinst);

            for(int i = 2; i < (int) root->children.size(); i += 2) {
                UnaryExp* unaryexp = (UnaryExp*) root->children[i];
                Term* sign = (Term*) root->children[i - 1];
                std::string uid;
                if(unaryexp->t == Type::IntLiteral) {
                    uid = get_tmp_var();
                    current_func->addInst(new Instruction(Operand(unaryexp->v, unaryexp->t), Operand(), Operand(uid, Type::Int), Operator::mov));
                } else {
                    uid = unaryexp->v;
                }
                if(sign->token.type == TokenType::MULT) {
                    current_func->addInst(new Instruction(Operand(root->v, root->t), Operand(uid, Type::Int), Operand(root->v, root->t), Operator::mul));
                } else if(sign->token.type == TokenType::DIV) {
                    current_func->addInst(new Instruction(Operand(root->v, root->t), Operand(uid, Type::Int), Operand(root->v, root->t), Operator::div));
                } else if(sign->token.type == TokenType::MOD) {
                    current_func->addInst(new Instruction(Operand(root->v, root->t), Operand(uid, Type::Int), Operand(root->v, root->t), Operator::mod));
                }
            }
        } else {
            root->v = tmp_float;
            root->t = Type::Float;

            for(int i = 0; i < (int) root->children.size(); i += 2) {
                UnaryExp* unaryexp = (UnaryExp*) root->children[i];
                Term* sign = (Term*) root->children[i - 1];
                std::string uid;
                if(unaryexp->t == Type::FloatLiteral || unaryexp->t == Type::IntLiteral) {
                    uid = i == 0 ? tmp_float : get_tmp_f_var();
                    current_func->addInst(new Instruction(Operand(unaryexp->v, Type::FloatLiteral), Operand(), Operand(uid, Type::Float), Operator::fmov));
                } else if(unaryexp->t == Type::Float) {
                    uid = unaryexp->v;
                } else {
                    uid = int_to_float(Operand(unaryexp->v, unaryexp->t)).name;
                }
                if(i == 0) {
                    root->v = uid;
                    tmp_f_stack.back() = tmp_f_cnt; // TODO
                    continue;
                }
                if(sign->token.type == TokenType::MULT) {
                    current_func->addInst(new Instruction(Operand(root->v, root->t), Operand(uid, Type::Float), Operand(root->v, root->t), Operator::fmul));
                } else if(sign->token.type == TokenType::DIV) {
                    current_func->addInst(new Instruction(Operand(root->v, root->t), Operand(uid, Type::Float), Operand(root->v, root->t), Operator::fdiv));
                }
            }
        }
    }
    restore_tmp();
}

void frontend::Analyzer::AnalyzeAddExp(AddExp* root) {
    log("AddExp");
    if(root->children.size() == 1) { // 只有一个儿子 直接继承
        MulExp* mulexp = (MulExp*) root->children[0];
        AnalyzeMulExp(mulexp);
        COPY_EXP_NODE(mulexp, root);
        return;
    }
    root->is_computable = true;
    std::string tmp_int = get_tmp_var(), tmp_float = get_tmp_f_var();
    // root->v = get_tmp_var(); 先不赋值
    store_tmp();
    int is_int = 1;
    for(int i = 0; i < (int) root->children.size(); i += 2) {
        MulExp* mulexp = (MulExp*) root->children[i];
        AnalyzeMulExp(mulexp);
        root->is_computable &= mulexp->is_computable;
        is_int &= (mulexp->t == Type::Int || mulexp->t == Type::IntLiteral);
    }
    if(root->is_computable) {
        if(is_int) {
            int temp = std::stoi(((MulExp*) root->children[0])->v);
            for(int i = 2; i < (int) root->children.size(); i += 2) {
                MulExp* mulexp = (MulExp*) root->children[i];
                Term* sign = (Term*) root->children[i - 1];
                int nowval = std::stoi(mulexp->v);
                if(sign->token.type == TokenType::PLUS) {
                    temp += nowval;
                } else if(sign->token.type == TokenType::MINU) {
                    temp -= nowval;
                }
            }
            root->v = std::to_string(temp);
            root->t = Type::IntLiteral;
        } else {
            float temp = std::stof(((MulExp*) root->children[0])->v);
            for(int i = 2; i < (int) root->children.size(); i += 2) {
                MulExp* mulexp = (MulExp*) root->children[i];
                Term* sign = (Term*) root->children[i - 1];
                float nowval = std::stof(mulexp->v);
                if(sign->token.type == TokenType::PLUS) {
                    temp += nowval;
                } else if(sign->token.type == TokenType::MINU) {
                    temp -= nowval;
                }
            }
            root->v = std::to_string(temp);
            root->t = Type::FloatLiteral;
        }
    } else {
        if(is_int) {
            root->v = tmp_int;
            root->t = Type::Int;
            MulExp* initu = (MulExp*) root->children[0];
            Instruction* movinst = new Instruction(Operand(initu->v, initu->t), Operand(), Operand(root->v, root->t), Operator::mov);
            insert_inst(movinst);

            for(int i = 2; i < (int) root->children.size(); i += 2) {
                MulExp* mulexp = (MulExp*) root->children[i];
                Term* sign = (Term*) root->children[i - 1];
                std::string uid;
                if(mulexp->t == Type::IntLiteral) {
                    uid = get_tmp_var();
                    current_func->addInst(new Instruction(Operand(mulexp->v, mulexp->t), Operand(), Operand(uid, Type::Int), Operator::mov));
                } else {
                    uid = mulexp->v;
                }
                if(sign->token.type == TokenType::PLUS) {
                    current_func->addInst(new Instruction(Operand(root->v, root->t), Operand(uid, Type::Int), Operand(root->v, root->t), Operator::add));
                } else if(sign->token.type == TokenType::MINU) {
                    current_func->addInst(new Instruction(Operand(root->v, root->t), Operand(uid, Type::Int), Operand(root->v, root->t), Operator::sub));
                }
            }
        } else {
            root->v = tmp_float;
            root->t = Type::Float;

            for(int i = 0; i < (int) root->children.size(); i += 2) {
                MulExp* mulexp = (MulExp*) root->children[i];
                Term* sign = (Term*) root->children[i - 1];
                std::string uid;
                if(mulexp->t == Type::FloatLiteral || mulexp->t == Type::IntLiteral) {
                    uid = i == 0 ? tmp_float : get_tmp_f_var();
                    current_func->addInst(new Instruction(Operand(mulexp->v, Type::FloatLiteral), Operand(), Operand(uid, Type::Float), Operator::fmov));
                } else if(mulexp->t == Type::Float) {
                    uid = mulexp->v;
                } else {
                    uid = int_to_float(Operand(mulexp->v, mulexp->t)).name;
                }
                if(i == 0) {
                    root->v = uid;
                    tmp_f_stack.back() = tmp_f_cnt; // TODO
                    continue;
                }
                if(sign->token.type == TokenType::PLUS) {
                    current_func->addInst(new Instruction(Operand(root->v, root->t), Operand(uid, Type::Float), Operand(root->v, root->t), Operator::fadd));
                } else if(sign->token.type == TokenType::MINU) {
                    current_func->addInst(new Instruction(Operand(root->v, root->t), Operand(uid, Type::Float), Operand(root->v, root->t), Operator::fsub));
                }
            }
        }
    }
    restore_tmp();
}

void frontend::Analyzer::AnalyzeRelExp(RelExp* root) {
    log("RelExp");
    if(root->children.size() == 1) { // 只有一个儿子 直接继承
        AddExp* addexp = (AddExp*) root->children[0];
        AnalyzeAddExp(addexp);
        COPY_EXP_NODE(addexp, root);
        return;
    }
    root->is_computable = true;
    std::string tmp_int = get_tmp_var(), tmp_float = get_tmp_f_var();
    // root->v = get_tmp_var(); 先不赋值
    store_tmp();
    int is_int = 1;
    for(int i = 0; i < (int) root->children.size(); i += 2) {
        AddExp* addexp = (AddExp*) root->children[i];
        AnalyzeAddExp(addexp);
        root->is_computable &= addexp->is_computable;
        is_int &= (addexp->t == Type::Int || addexp->t == Type::IntLiteral);
    }
    if(root->is_computable) {
        if(is_int) {
            int temp = std::stoi(((AddExp*) root->children[0])->v);
            for(int i = 2; i < (int) root->children.size(); i += 2) {
                AddExp* addexp = (AddExp*) root->children[i];
                Term* sign = (Term*) root->children[i - 1];
                int nowval = std::stoi(addexp->v);
                if(sign->token.type == TokenType::LSS) {
                    temp = (temp < nowval);
                } else if(sign->token.type == TokenType::GTR) {
                    temp = (temp > nowval);
                } else if(sign->token.type == TokenType::LEQ) {
                    temp = (temp <= nowval);
                } else if(sign->token.type == TokenType::GEQ) {
                    temp = (temp >= nowval);
                } 
            }
            root->v = std::to_string(temp);
            root->t = Type::IntLiteral;
        } else {
            float temp = std::stof(((AddExp*) root->children[0])->v);
            for(int i = 2; i < (int) root->children.size(); i += 2) {
                AddExp* addexp = (AddExp*) root->children[i];
                Term* sign = (Term*) root->children[i - 1];
                float nowval = std::stof(addexp->v);
                if(sign->token.type == TokenType::LSS) {
                    temp = (temp < nowval);
                } else if(sign->token.type == TokenType::GTR) {
                    temp = (temp > nowval);
                } else if(sign->token.type == TokenType::LEQ) {
                    temp = (temp <= nowval);
                } else if(sign->token.type == TokenType::GEQ) {
                    temp = (temp >= nowval);
                } 
            }
            root->v = std::to_string(temp);
            root->t = Type::FloatLiteral;
        }
    } else {
        if(is_int) {
            root->v = tmp_int;
            root->t = Type::Int;
            AddExp* initu = (AddExp*) root->children[0];
            Instruction* movinst = new Instruction(Operand(initu->v, initu->t), Operand(), Operand(root->v, root->t), Operator::mov);
            insert_inst(movinst);

            for(int i = 2; i < (int) root->children.size(); i += 2) {
                AddExp* addexp = (AddExp*) root->children[i];
                Term* sign = (Term*) root->children[i - 1];
                std::string uid;
                if(addexp->t == Type::IntLiteral) {
                    uid = get_tmp_var();
                    current_func->addInst(new Instruction(Operand(addexp->v, addexp->t), Operand(), Operand(uid, Type::Int), Operator::mov));
                } else {
                    uid = addexp->v;
                }
                // if(sign->token.type == TokenType::LSS) {
                //     current_func->addInst(new Instruction(Operand(root->v, root->t), Operand(uid, Type::Int), Operand(root->v, root->t), Operator::lss));
                // } else if(sign->token.type == TokenType::GTR) {
                //     current_func->addInst(new Instruction(Operand(root->v, root->t), Operand(uid, Type::Int), Operand(root->v, root->t), Operator::gtr));
                // } else if(sign->token.type == TokenType::LEQ) {
                //     current_func->addInst(new Instruction(Operand(root->v, root->t), Operand(uid, Type::Int), Operand(root->v, root->t), Operator::leq));
                // } else if(sign->token.type == TokenType::GEQ) {
                //     current_func->addInst(new Instruction(Operand(root->v, root->t), Operand(uid, Type::Int), Operand(root->v, root->t), Operator::geq));
                // }
                addRelInst(Operand(root->v, root->t), Operand(uid, Type::Int), Operand(root->v, root->t), sign->token.type);
            }
        } else {
            root->v = tmp_float;
            root->t = Type::Float;

            for(int i = 0; i < (int) root->children.size(); i += 2) {
                AddExp* addexp = (AddExp*) root->children[i];
                Term* sign = (Term*) root->children[i - 1];
                std::string uid;
                if(addexp->t == Type::FloatLiteral || addexp->t == Type::IntLiteral) {
                    uid = i == 0 ? tmp_float : get_tmp_f_var();
                    current_func->addInst(new Instruction(Operand(addexp->v, Type::FloatLiteral), Operand(), Operand(uid, Type::Float), Operator::fmov));
                } else if(addexp->t == Type::Float) {
                    uid = addexp->v;
                } else {
                    uid = int_to_float(Operand(addexp->v, addexp->t)).name;
                }
                if(i == 0) {
                    root->v = uid;
                    tmp_f_stack.back() = tmp_f_cnt; // TODO
                    continue;
                }
                // if(sign->token.type == TokenType::LSS) {
                //     current_func->addInst(new Instruction(Operand(root->v, root->t), Operand(uid, Type::Float), Operand(root->v, root->t), Operator::flss));
                // } else if(sign->token.type == TokenType::GTR) {
                //     current_func->addInst(new Instruction(Operand(root->v, root->t), Operand(uid, Type::Float), Operand(root->v, root->t), Operator::fgtr));
                // } else if(sign->token.type == TokenType::LEQ) {
                //     current_func->addInst(new Instruction(Operand(root->v, root->t), Operand(uid, Type::Float), Operand(root->v, root->t), Operator::fleq));
                // } else if(sign->token.type == TokenType::GEQ) {
                //     current_func->addInst(new Instruction(Operand(root->v, root->t), Operand(uid, Type::Float), Operand(root->v, root->t), Operator::fgeq));
                // }
                addFRelInst(Operand(root->v, root->t), Operand(uid, Type::Float), Operand(root->v, root->t), sign->token.type);
            }
        }
    }
    restore_tmp();
}

void frontend::Analyzer::AnalyzeEqExp(EqExp* root) {
    log("EqExp");
    if(root->children.size() == 1) { // 只有一个儿子 直接继承
        RelExp* relexp = (RelExp*) root->children[0];
        AnalyzeRelExp(relexp);
        COPY_EXP_NODE(relexp, root);
        return;
    }
    root->is_computable = true;
    std::string tmp_int = get_tmp_var(), tmp_float = get_tmp_f_var();
    // root->v = get_tmp_var(); 先不赋值
    store_tmp();
    int is_int = 1;
    for(int i = 0; i < (int) root->children.size(); i += 2) {
        RelExp* relexp = (RelExp*) root->children[i];
        AnalyzeRelExp(relexp);
        root->is_computable &= relexp->is_computable;
        is_int &= (relexp->t == Type::Int || relexp->t == Type::IntLiteral);
    }
    if(root->is_computable) {
        if(is_int) {
            int temp = std::stoi(((RelExp*) root->children[0])->v);
            for(int i = 2; i < (int) root->children.size(); i += 2) {
                RelExp* relexp = (RelExp*) root->children[i];
                Term* sign = (Term*) root->children[i - 1];
                int nowval = std::stoi(relexp->v);
                if(sign->token.type == TokenType::EQL) {
                    temp = (temp == nowval);
                } else if(sign->token.type == TokenType::NEQ) {
                    temp = (temp != nowval);
                }
            }
            root->v = std::to_string(temp);
            root->t = Type::IntLiteral;
        } else {
            float temp = std::stof(((RelExp*) root->children[0])->v);
            for(int i = 2; i < (int) root->children.size(); i += 2) {
                RelExp* relexp = (RelExp*) root->children[i];
                Term* sign = (Term*) root->children[i - 1];
                float nowval = std::stof(relexp->v);
                if(sign->token.type == TokenType::EQL) {
                    temp = (temp == nowval);
                } else if(sign->token.type == TokenType::NEQ) {
                    temp = (temp != nowval);
                }
            }
            root->v = std::to_string(temp);
            root->t = Type::FloatLiteral;
        }
    } else {
        if(is_int) {
            root->v = tmp_int;
            root->t = Type::Int;
            RelExp* initu = (RelExp*) root->children[0];
            Instruction* movinst = new Instruction(Operand(initu->v, initu->t), Operand(), Operand(root->v, root->t), Operator::mov);
            insert_inst(movinst);

            for(int i = 2; i < (int) root->children.size(); i += 2) {
                RelExp* relexp = (RelExp*) root->children[i];
                Term* sign = (Term*) root->children[i - 1];
                std::string uid;
                if(relexp->t == Type::IntLiteral) {
                    uid = get_tmp_var();
                    current_func->addInst(new Instruction(Operand(relexp->v, relexp->t), Operand(), Operand(uid, Type::Int), Operator::mov));
                } else {
                    uid = relexp->v;
                }
                // if(sign->token.type == TokenType::EQL) {
                //     current_func->addInst(new Instruction(Operand(root->v, root->t), Operand(uid, Type::Int), Operand(root->v, root->t), Operator::eq));
                // } else if(sign->token.type == TokenType::NEQ) {
                //     current_func->addInst(new Instruction(Operand(root->v, root->t), Operand(uid, Type::Int), Operand(root->v, root->t), Operator::neq));
                // }
                addRelInst(Operand(root->v, root->t), Operand(uid, Type::Int), Operand(root->v, root->t), sign->token.type);
            }
        } else {
            root->v = tmp_float;
            root->t = Type::Float;

            for(int i = 0; i < (int) root->children.size(); i += 2) {
                RelExp* relexp = (RelExp*) root->children[i];
                Term* sign = (Term*) root->children[i - 1];
                std::string uid;
                if(relexp->t == Type::FloatLiteral || relexp->t == Type::IntLiteral) {
                    uid = i == 0 ? tmp_float : get_tmp_f_var();
                    current_func->addInst(new Instruction(Operand(relexp->v, Type::FloatLiteral), Operand(), Operand(uid, Type::Float), Operator::fmov));
                } else if(relexp->t == Type::Float) {
                    uid = relexp->v;
                } else {
                    uid = int_to_float(Operand(relexp->v, relexp->t)).name;
                }
                if(i == 0) {
                    root->v = uid;
                    tmp_f_stack.back() = tmp_f_cnt; // TODO
                    continue;
                }
                // if(sign->token.type == TokenType::EQL) {
                //     current_func->addInst(new Instruction(Operand(root->v, root->t), Operand(uid, Type::Float), Operand(root->v, root->t), Operator::feq));
                // } else if(sign->token.type == TokenType::NEQ) {
                //     current_func->addInst(new Instruction(Operand(root->v, root->t), Operand(uid, Type::Float), Operand(root->v, root->t), Operator::fneq));
                // }
                addFRelInst(Operand(root->v, root->t), Operand(uid, Type::Float), Operand(root->v, root->t), sign->token.type);
            }
        }
    }
    restore_tmp();
}

void frontend::Analyzer::AnalyzeLAndExp(LAndExp* root) {
    store_tmp();
    EqExp* eqexp = (EqExp*) root->children[0];
    AnalyzeEqExp(eqexp);

    if(eqexp->is_computable) {
        if(eqexp->v == "0" || std::stof(eqexp->v) == 0) {
            return;
        } else {
            if(root->children.size() > 1) {
                LAndExp *landexp = (LAndExp*) root->children[2];
                AnalyzeLAndExp(landexp);
            } else {
                // the last eqexp goto in of if/while
                Instruction *goto_in = new Instruction(
                    Operand(),
                    Operand(),
                    Operand(std::to_string(current_func->InstVec.size()), Type::IntLiteral),
                    Operator::_goto
                );
                last_cond->jump_in.insert(goto_in);
                insert_inst(goto_in);
            }
        } 
    } else {
        // goto next or
        Operand goto_checker;
        if(eqexp->t == Type::Int) {
            insert_inst(new Instruction(
                Operand(eqexp->v, eqexp->t),
                Operand("0", Type::IntLiteral),
                Operand(eqexp->v, eqexp->t),
                Operator::eq
            ));
            goto_checker = Operand(eqexp->v, eqexp->t);
        } else {
            store_tmp();
            goto_checker = Operand(get_tmp_f_var(), Type::Float);
            Operand op2 = convert_type(Operand("0", Type::FloatLiteral), Type::Float);
            insert_inst(new Instruction(
                Operand(eqexp->v, eqexp->t),
                op2,
                goto_checker,
                Operator::feq
            ));
            restore_tmp();
            goto_checker = float_to_int(goto_checker);
        }

        int pos = current_func->InstVec.size();
        Instruction *goto_next_or = new Instruction(
            goto_checker,
            Operand(),
            Operand("assert", Type::IntLiteral),
            Operator::_goto
        );
        insert_inst(goto_next_or);
        if(root->children.size() > 1) {
            LAndExp *landexp = (LAndExp*) root->children[2];
            AnalyzeLAndExp(landexp);
        } else {
            // the last eqexp goto in of if/while
            Instruction *goto_in = new Instruction(
                Operand(),
                Operand(),
                Operand(std::to_string(current_func->InstVec.size()), Type::IntLiteral),
                Operator::_goto
            );
            last_cond->jump_in.insert(goto_in);
            insert_inst(goto_in);
        }
        // 跳到下一个or，也就是现在的位置
        goto_next_or->des.name = std::to_string(current_func->InstVec.size() - pos);
    }
    restore_tmp();
}

void frontend::Analyzer::AnalyzeLOrExp(LOrExp* root) {
    store_tmp();
    LAndExp* landexp = (LAndExp*) root->children[0];
    AnalyzeLAndExp(landexp);

    if(root->children.size() > 1) {
        LOrExp* lorexp = (LOrExp*) root->children[2];
        AnalyzeLOrExp(lorexp);
    }
    restore_tmp();
}

void frontend::Analyzer::AnalyzeConstExp(ConstExp* root) {
    log("ConstExp");
    AddExp* addexp = (AddExp*) root->children[0];
    AnalyzeAddExp(addexp);
    COPY_EXP_NODE(addexp, root);
    assert(root->is_computable);
}