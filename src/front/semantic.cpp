#include"front/semantic.h"

#include<cassert>
#include<iostream>
#include<string>

using ir::Instruction;
using ir::Function;
using ir::Operand;
using ir::Operator;

#define TODO assert(0 && "TODO");

#define GET_CHILD_PTR(node, type, index) auto node = dynamic_cast<type*>(root->children[index]); assert(node); 
#define ANALYSIS(node, type, index) auto node = dynamic_cast<type*>(root->children[index]); assert(node); analysis##type(node);
#define COPY_EXP_NODE(from, to) to->is_computable = from->is_computable; to->v = from->v; to->t = from->t;

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
    si.name = st.str().substr(2);
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
    assert(0 && "global");
    return id;
}

Operand frontend::SymbolTable::get_operand(string id) const {
    for(int i = scope_stack.size() - 1; i >= 0; i--) {
        if(scope_stack[i].table.count(id)) {
            return scope_stack[i].table.find(id)->second.operand;
        }
    }
    assert(0 && "global");
}

frontend::STE frontend::SymbolTable::get_ste(string id) const {
    for(int i = scope_stack.size() - 1; i >= 0; i--) {
        if(scope_stack[i].table.count(id)) {
            return scope_stack[i].table.find(id)->second;
        }
    }
    assert(0 && "global");
}

frontend::Analyzer::Analyzer(): tmp_cnt(0), symbol_table() {
    TODO;
}

ir::Program frontend::Analyzer::get_ir_program(CompUnit* root) {
    TODO;
}

std::string frontend::Analyzer::get_tmp_var() {
    return "t" + std::to_string(tmp_cnt++);
}

void frontend::Analyzer::release_tmp_var(int cnt) {
    tmp_cnt -= cnt;
}

void frontend::Analyzer::insert_ste(STE ste) {
    if(current_func == nullptr) {
        if(ste.operand.type == Type::FloatPtr || ste.operand.type == Type::IntPtr) {
            int len = 1;
            for(int i : ste.dimension) {
                len *= i;
            }
            program.globalVal.emplace_back(ste.operand, len);
        } else {
            program.globalVal.emplace_back(ste.operand);
        }
    } else {
        symbol_table.scope_stack.back().table[ste.operand.name] = ste;
    }
}

void frontend::Analyzer::insert_inst(Instruction* inst) {
    if(current_func == nullptr) {
        g_init_inst.push_back(inst);
    } else {
        current_func->addInst(inst);
    }
}

void frontend::Analyzer::AnalyzeCompUnit(CompUnit* root) {
    for(int i = 0; i < root->children.size(); i++) {
        AstNode* chroot = root->children[i];
        if(chroot->type == NodeType::DECL) {
            AnalyzeDecl((Decl*) chroot);
        } else if(chroot->type == NodeType::FUNCDEF) {
            AnalyzeFuncDef((FuncDef*) chroot);
        } else {
            AnalyzeCompUnit((CompUnit*) chroot);
        }
    }
}

void frontend::Analyzer::AnalyzeDecl(Decl* root) {
    for(int i = 0; i < root->children.size(); i++) {
        AstNode* chroot = root->children[i];
        if(chroot->type == NodeType::CONSTDECL) {
            AnalyzeConstDecl((ConstDecl*) chroot);
        } else if(chroot->type == NodeType::VARDECL) {
            AnalyzeVarDecl((VarDecl*) chroot);
        }
    }
}

void frontend::Analyzer::AnalyzeConstDecl(ConstDecl* root) {
    AnalyzeBType(((BType*) root->children[1]));
    root->t = ((BType*) root->children[1])->t;
    for(int i = 2; i < root->children.size(); i += 2) {
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
    int constExp_ptr = 1;
    std::vector<int> dimension;
    int len = 1;
    while(root->children[constExp_ptr]->type == NodeType::CONSTEXP) {
        AnalyzeConstExp((ConstExp*) root->children[constExp_ptr]);
        dimension.push_back(stoi(((ConstExp*) root->children[constExp_ptr])->v));
        constExp_ptr += 2;
    }
    // calc array len
    for(int i : dimension) {
        len *= i;
    }
    // insert ste
    STE ste;
    if(is_ptr) {
        ste.operand = Operand(((Term*) root->children[0])->token.value, root->t == Type::Int ? Type::IntPtr : Type::FloatPtr);
        ste.dimension = dimension;
        // alloc memory to ste
        Operand op1 = Operand(std::to_string(len), Type::IntLiteral);
        Operand op2 = Operand();
        Operand des = Operand(symbol_table.get_scoped_name(ste.operand.name), ste.operand.type);
        insert_inst(new Instruction(op1, op2, des, Operator::alloc));
    } else {
        ste.operand = Operand(((Term*) root->children[0])->token.value, root->t == Type::Int ? Type::IntLiteral : Type::FloatLiteral);
    }
    insert_ste(ste);

    // init val
    vector<int> dim;
    AnalyzeConstInitVal(ste, dim, (ConstInitVal*) root->children[constExp_ptr]);
    // TODO: array assign
}

void frontend::Analyzer::AnalyzeConstInitVal(STE& ste, std::vector<int>& dim, ConstInitVal* root) {
    if(root->children[0]->type == NodeType::TERMINAL) {
        dim.push_back(0);
        int ptr = 1;
        while(ptr < root->children.size() && root->children[ptr]->type == NodeType::CONSTINITVAL) {
            AnalyzeConstInitVal(ste, dim, (ConstInitVal*) root->children[ptr]);
            ptr += 2;
            dim.back()++;
        }
        dim.pop_back();
    } else {
        // calc constexp val
        AnalyzeConstExp((ConstExp*) root->children[0]);

        // calc index
        int index = 0;
        vector<int>& dimension = ste.dimension;
        assert(dimension.size() == dim.size());
        int base = 1;
        for(int i = dimension.size() - 1; i >= 0; i--) {
            index += dim[i] * base;
            base *= dimension[i];
        }

        // store
        if(ste.operand.type == Type::IntPtr || ste.operand.type == Type::FloatPtr) {
            Operand op1 = Operand(symbol_table.get_scoped_name(ste.operand.name), ste.operand.type);
            Operand op2 = Operand(std::to_string(index), Type::IntLiteral);
            Operand des = Operand(((ConstExp*) root->children[0])->v, ste.operand.type == Type::IntPtr ? Type::IntLiteral : Type::FloatLiteral);
            insert_inst(new Instruction(op1, op2, des, Operator::store));
        } else {
            Operand op1 = Operand(symbol_table.get_scoped_name(ste.operand.name), ste.operand.type);
            Operand op2 = Operand();
            Operand des = Operand(((ConstExp*) root->children[0])->v, ste.operand.type);
            insert_inst(new Instruction(op1, op2, des, Operator::def));
        }
    }
}

void frontend::Analyzer::AnalyzeVarDecl(VarDecl* root) {
    BType* bc = (BType*) root->children[0];
    AnalyzeBType(bc);
    root->t = bc->t;
    for(int i = 1; i < root->children.size(); i += 2) {
        VarDef* chroot = (VarDef*) root->children[i];
        chroot->t = bc->t;
        AnalyzeVarDef(chroot);
    }
}

void frontend::Analyzer::AnalyzeVarDef(VarDef* root) {
    bool is_ptr = root->children.size() > 1 && ((Term*) root->children[1])->token.type == TokenType::LBRACK;
    int constExp_ptr = 1;
    std::vector<int> dimension;
    int len = 1;
    while(constExp_ptr < root->children.size() && root->children[constExp_ptr]->type == NodeType::CONSTEXP) {
        AnalyzeConstExp((ConstExp*) root->children[constExp_ptr]);
        dimension.push_back(std::stoi(((ConstExp*) root->children[constExp_ptr])->v));
        constExp_ptr += 2;
    }
    // calc array len
    for(int i : dimension) {
        len *= i;
    }
    // insert ste
    STE ste;
    if(is_ptr) {
        ste.operand = Operand(((Term*) root->children[0])->token.value, root->t == Type::Int ? Type::IntPtr : Type::FloatPtr);
        ste.dimension = dimension;
        Operand op1 = Operand(std::to_string(len), Type::IntLiteral);
        Operand op2 = Operand();
        Operand des = Operand(symbol_table.get_scoped_name(ste.operand.name), ste.operand.type);
        insert_inst(new Instruction(op1, op2, des, Operator::alloc));
    } else {
        ste.operand = Operand(((Term*) root->children[0])->token.value, root->t == Type::Int ? Type::Int : Type::Float);
    }
    insert_ste(ste);

    // init val
    vector<int> dim;
    if(constExp_ptr < root->children.size()) {
        AnalyzeInitVal(ste, dim, (InitVal*) root->children[constExp_ptr]);
    }
    // TODO: array assign
}

void frontend::Analyzer::AnalyzeInitVal(STE& ste, std::vector<int>& dim, InitVal* root) {
    if(root->children[0]->type == NodeType::TERMINAL) {
        dim.push_back(0);
        int ptr = 1;
        while(ptr < root->children.size() && root->children[ptr]->type == NodeType::INITVAL) {
            AnalyzeInitVal(ste, dim, (InitVal*) root->children[ptr]);
            ptr += 2;
            dim.back()++;
        }
        dim.pop_back();
    } else {
        // calc constexp val
        AnalyzeExp((Exp*) root->children[0]);

        // calc index
        int index = 0;
        vector<int>& dimension = ste.dimension;
        assert(dimension.size() == dim.size());
        int base = 1;
        for(int i = dimension.size() - 1; i >= 0; i--) {
            index += dim[i] * base;
            base *= dimension[i];
        }

        // store
        Exp* ch = (Exp*) root->children[0];
        if(ste.operand.type == Type::IntPtr || ste.operand.type == Type::FloatPtr) {
            Operand op1 = Operand(symbol_table.get_scoped_name(ste.operand.name), ste.operand.type);
            Operand op2 = Operand(std::to_string(index), Type::IntLiteral);
            if(ch->is_computable) {
                Operand des = Operand(ch->v, ste.operand.type == Type::IntPtr ? Type::IntLiteral : Type::FloatLiteral);
                insert_inst(new Instruction(op1, op2, des, Operator::store));
            } else {
                Operand des = Operand(ch->v, ste.operand.type == Type::IntPtr ? Type::Int : Type::Float);
                insert_inst(new Instruction(op1, op1, des, Operator::store));
            }
        } else {
            Operand op1 = Operand(symbol_table.get_scoped_name(ste.operand.name), ste.operand.type);
            Operand op2 = Operand();
            if(ch->is_computable) {
                Operand des = Operand(ch->v, ste.operand.type == Type::Int ? Type::IntLiteral : Type::FloatLiteral);
                insert_inst(new Instruction(op1, op2, des, Operator::def));
            } else {
                Operand des = Operand(ch->v, ste.operand.type);
                insert_inst(new Instruction(op1, op2, des, Operator::def));
            }
        }
    }
}

void frontend::Analyzer::AnalyzeFuncDef(FuncDef* root) {
    FuncType* funcTypech = (FuncType*) root->children[0];
    AnalyzeFuncType(funcTypech);
    std::string funcname = ((Term*) root->children[1])->token.value;
    Type functype = funcTypech->t;
    Function fun(funcname, functype);
    current_func = &fun;
    Block* funcBlock = (Block*) root->children.back();
    symbol_table.add_scope(funcBlock);
    if(root->children[3]->type != NodeType::TERMINAL) {
        // has [FuncFParams]
        AnalyzeFuncFParams((FuncFParams*) root->children[3]);
    }
    AnalyzeBlock(funcBlock);
    current_func = nullptr;
    program.addFunction(fun);
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
    BType* Btypech = (BType*) root->children[0];
    std::string id = ((Term*) root->children[1])->token.value;
    bool is_ptr = false;
    if(root->children.size() > 2) {
        is_ptr = true;
    }
    STE ste;
    ste.operand = Operand(id, is_ptr ? Btypech->t == Type::Int ? Type::IntPtr : Type::FloatPtr : Btypech->t);
    insert_ste(ste);
    current_func->ParameterList.emplace_back(symbol_table.get_scoped_name(id), ste.operand.type);
}

void frontend::Analyzer::AnalyzeFuncFParams(FuncFParams* root) {
    for(int i = 0; i < root->children.size(); i += 2) {
        FuncFParam* ch = (FuncFParam*) root->children[i];
        AnalyzeFuncFParam(ch);
    }
}

void frontend::Analyzer::AnalyzeBlock(Block* root) {
    for(int i = 0; i < root->children.size(); i++) {
        AnalyzeBlockItem((BlockItem*) root->children[i]);
    }
}

void frontend::Analyzer::AnalyzeBlockItem(BlockItem* root) {
    if(root->children[0]->type == NodeType::DECL) {
        AnalyzeDecl((Decl*) root->children[0]);
    } else {
        AnalyzeStmt((Stmt*) root->children[0]);
    }
}

void frontend::Analyzer::AnalyzeStmt(Stmt* root) {
    if(root->children[0]->type == NodeType::BLOCK) {
        AnalyzeBlock((Block*) root->children[0]);
    } else if(root->children[0]->type == NodeType::TERMINAL) {
        // break;
        if(((Term*) root->children[0])->token.type == TokenType::BREAKTK) {
            // break
            Operand op1 = Operand();
            Operand op2 = Operand();
            Operand des = Operand(std::to_string(current_func->InstVec.size()), Type::IntLiteral);
            Instruction* goto_inst = new Instruction(op1, op2, des, Operator::_goto);
            current_func->addInst(goto_inst);
            last_while->jump_eow.insert(goto_inst);
        } else if(((Term*) root->children[0])->token.type == TokenType::CONTINUETK) {
            // continue
            Operand op1 = Operand();
            Operand op2 = Operand();
            Operand des = Operand(std::to_string(current_func->InstVec.size()), Type::IntLiteral);
            Instruction* goto_inst = new Instruction(op1, op2, des, Operator::_goto);
            current_func->addInst(goto_inst);
            last_while->jump_bow.insert(goto_inst);
        } else if(((Term*) root->children[0])->token.type == TokenType::IFTK) {
            // if
            // calc cond
            Cond* cond = (Cond*) root->children[2];
            AnalyzeCond(cond);
            Instruction* elseinst = nullptr;
            std::string ifvar = get_tmp_var();
            {
                // copy cond to temp
                Operand op1 = Operand(cond->v, cond->t);
                Operand op2 = Operand();
                Operand des = Operand(ifvar, Type::Int);
                current_func->addInst(new Instruction(op1, op2, des, Operator::def));
            }
            {
                // cond if
                Operand op1 = Operand(ifvar, Type::Int);
                Operand op2 = Operand();
                Operand des = Operand("2", Type::IntLiteral);
                current_func->addInst(new Instruction(op1, op2, des, Operator::_goto));
                // if tx goto [pc, 2]
            }
            {
                Operand op1 = Operand();
                Operand op2 = Operand();
                Operand des = Operand(std::to_string(current_func->InstVec.size()), Type::IntLiteral);
                elseinst = new Instruction(op1, op2, des, Operator::_goto);
                current_func->addInst(elseinst);
                // goto [pc, else]
            }
            AnalyzeStmt((Stmt*) root->children[4]);
            elseinst->des.name = std::to_string(current_func->InstVec.size() - std::stoi(elseinst->des.name));
            {
                // end of if
                Operand op1 = Operand();
                Operand op2 = Operand();
                Operand des = Operand("0", Type::IntLiteral);
                current_func->addInst(new Instruction(op1, op2, des, Operator::_goto));
            }
        }
    }
}

void frontend::Analyzer::AnalyzeExp(Exp*) {
TODO;
}

void frontend::Analyzer::AnalyzeCond(Cond*) {
TODO;
}

void frontend::Analyzer::AnalyzeLVal(LVal*) {
TODO;
}

void frontend::Analyzer::AnalyzeNumber(Number*) {
TODO;
}

void frontend::Analyzer::AnalyzePrimaryExp(PrimaryExp*) {
TODO;
}

void frontend::Analyzer::AnalyzeUnaryExp(UnaryExp*) {
TODO;
}

void frontend::Analyzer::AnalyzeUnaryOp(UnaryOp*) {
TODO;
}

void frontend::Analyzer::AnalyzeFuncRParams(FuncRParams*) {
TODO;
}

void frontend::Analyzer::AnalyzeMulExp(MulExp*) {
TODO;
}

void frontend::Analyzer::AnalyzeAddExp(AddExp*) {
TODO;
}

void frontend::Analyzer::AnalyzeRelExp(RelExp*) {
TODO;
}

void frontend::Analyzer::AnalyzeEqExp(EqExp*) {
TODO;
}

void frontend::Analyzer::AnalyzeLAndExp(LAndExp*) {
TODO;
}

void frontend::Analyzer::AnalyzeLOrExp(LOrExp*) {
TODO;
}

void frontend::Analyzer::AnalyzeConstExp(ConstExp*) {
TODO;
}