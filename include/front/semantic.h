/**
 * @file semantic.h
 * @author Yuntao Dai (d1581209858@live.com)
 * @brief 
 * @version 0.1
 * @date 2023-01-06
 * 
 * a Analyzer should 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef SEMANTIC_H
#define SEMANTIC_H

#include"ir/ir.h"
#include"front/abstract_syntax_tree.h"

#include<map>
#include<string>
#include<vector>
using std::map;
using std::string;
using std::vector;

namespace frontend
{

// definition of symbol table entry
struct STE {
    ir::Operand operand;
    vector<int> dimension;
};

using map_str_ste = map<string, STE>;
// definition of scope infomation
struct ScopeInfo {
    int cnt;
    string name;
    map_str_ste table;
};

// surpport lib functions
map<std::string,ir::Function*>* get_lib_funcs();

// definition of symbol table
struct SymbolTable{
    vector<ScopeInfo> scope_stack;
    map<std::string,ir::Function*> functions;

    /**
     * @brief enter a new scope, record the infomation in scope stacks
     * @param node: a Block node, entering a new Block means a new name scope
     */
    void add_scope(Block*);

    /**
     * @brief exit a scope, pop out infomations
     */
    void exit_scope();

    /**
     * @brief Get the scoped name, to deal the same name in different scopes, we change origin id to a new one with scope infomation,
     * for example, we have these code:
     * "     
     * int a;
     * {
     *      int a; ....
     * }
     * "
     * in this case, we have two variable both name 'a', after change they will be 'a' and 'a_block'
     * @param id: origin id 
     * @return string: new name with scope infomations
     */
    string get_scoped_name(string id) const;

    /**
     * @brief get the right operand with the input name
     * @param id identifier name
     * @return Operand 
     */
    ir::Operand get_operand(string id) const;

    /**
     * @brief get the right ste with the input name
     * @param id identifier name
     * @return STE 
     */
    STE get_ste(string id) const;
};


// singleton class
struct Analyzer {
    int tmp_cnt;
    int tmp_f_cnt;
    vector<ir::Instruction*> g_init_inst;
    SymbolTable symbol_table;
    ir::Function* current_func;
    ir::Program program;
    Cond* last_cond;
    vector<Stmt*> last_while;
    vector<int> tmp_stack;
    vector<int> tmp_f_stack;

    /**
     * @brief constructor
     */
    Analyzer();
    std::string get_tmp_f_var();
    std::string get_tmp_var();
    
    void store_tmp();
    void restore_tmp();

    ir::Operand int_to_float(ir::Operand op);
    ir::Operand float_to_int(ir::Operand op);

    ir::Operand literal_to_var(ir::Operand op);
    ir::Operand convert_type(ir::Operand op, ir::Type target);


    // analysis functions
    ir::Program get_ir_program(CompUnit*);
    void insert_ste(std::string name, STE ste);
    void insert_inst(ir::Instruction* inst);
    void addRelInst(ir::Operand, ir::Operand, ir::Operand, TokenType);
    void addFRelInst(ir::Operand, ir::Operand, ir::Operand, TokenType);

    void AnalyzeCompUnit(CompUnit*);
    void AnalyzeDecl(Decl*);
    void AnalyzeConstDecl(ConstDecl*);
    void AnalyzeBType(BType*);
    void AnalyzeConstDef(ConstDef*);
    void AnalyzeConstInitVal(int &, STE&, int, int, ConstInitVal*);
    void AnalyzeVarDecl(VarDecl*);
    void AnalyzeVarDef(VarDef*);
    void AnalyzeInitVal(int&, STE&, int, int, InitVal*);
    void AnalyzeFuncDef(FuncDef*);
    void AnalyzeFuncType(FuncType*);
    void AnalyzeFuncFParam(FuncFParam*);
    void AnalyzeFuncFParams(FuncFParams*);
    void AnalyzeBlock(Block*);
    void AnalyzeBlockItem(BlockItem*);
    void AnalyzeStmt(Stmt*);
    void AnalyzeExp(Exp*);
    void AnalyzeCond(Cond*);
    void AnalyzeLVal(LVal*, int);
    void AnalyzeNumber(Number*);
    void AnalyzePrimaryExp(PrimaryExp*);
    void AnalyzeUnaryExp(UnaryExp*);
    void AnalyzeUnaryOp(UnaryOp*);
    void AnalyzeFuncRParams(std::vector<ir::Operand>&, FuncRParams*);
    void AnalyzeMulExp(MulExp*);
    void AnalyzeAddExp(AddExp*);
    void AnalyzeRelExp(RelExp*);
    void AnalyzeEqExp(EqExp*);
    void AnalyzeLAndExp(LAndExp*);
    void AnalyzeLOrExp(LOrExp*);
    void AnalyzeConstExp(ConstExp*);

    // reject copy & assignment
    Analyzer(const Analyzer&) = delete;
    Analyzer& operator=(const Analyzer&) = delete;
};

} // namespace frontend

#endif