#include"front/syntax.h"

#include<iostream>
#include<cassert>

using frontend::Parser;
using frontend::CompUnit;
using frontend::AstNode;
using frontend::Decl;
using frontend::ConstDecl;
using frontend::BType;
using frontend::ConstDef;
using frontend::ConstInitVal;
using frontend::VarDecl;
using frontend::VarDef;
using frontend::InitVal;
using frontend::FuncDef;
using frontend::FuncType;
using frontend::FuncFParam;
using frontend::FuncFParams;
using frontend::Block;
using frontend::BlockItem;
using frontend::Stmt;
using frontend::Exp;
using frontend::Cond;
using frontend::LVal;
using frontend::Number;
using frontend::PrimaryExp;
using frontend::UnaryExp;
using frontend::UnaryOp;
using frontend::FuncRParams;
using frontend::MulExp;
using frontend::AddExp;
using frontend::RelExp;
using frontend::EqExp;
using frontend::LAndExp;
using frontend::LOrExp;
using frontend::ConstExp;
using frontend::Term;
using frontend::TokenType;

#define DEBUG_PARSER
#define TODO assert(0 && "todo")
#define CUR_TOKEN_IS(tk_type) (token_stream[index].type == tk_type)
#define NEXT_TOKEN_IS(tk_type) (index + 1 < token_stream.size() && token_stream[index + 1].type == tk_type)
#define NEXT_NEXT_TOKEN_IS(tk_type) (index + 2 < token_stream.size() && token_stream[index + 2].type == tk_type)
#define PARSE_TOKEN(tk_type) root->children.push_back(parseTerm(root, tk_type))
#define PARSE(name, type) auto name = new type(root); assert(parse##type(name)); root->children.push_back(name); 

Parser::Parser(const std::vector<frontend::Token>& tokens): index(0), token_stream(tokens) {}

Parser::~Parser() {}

frontend::CompUnit* Parser::get_abstract_syntax_tree() {
    CompUnit *root = new CompUnit();
    parseCompUnit(root);
    return root;
}

Term* Parser::parseTerm(AstNode* parent, TokenType expected) {
    assert(token_stream[index].type == expected);
    Term* ret = new Term(token_stream[index], parent);
    log(ret);
    index++;
    return ret;
}

bool Parser::parseCompUnit(CompUnit* root) {
    log(root);

    if(NEXT_NEXT_TOKEN_IS(TokenType::LPARENT)) {
        // use CompUnit -> FuncDef
        PARSE(funcdef, FuncDef);
    } else {
        // use FuncDef -> Decl
        PARSE(decl, Decl);
    }

    if(index + 1 < token_stream.size()) {
        PARSE(compUnit, CompUnit);
    }

    return true;
}

bool Parser::parseDecl(Decl* root) {
    log(root);

    if(CUR_TOKEN_IS(TokenType::CONSTTK)) {
        // use Decl -> ConstDecl
        PARSE(constDecl, ConstDecl);
    } else {
        // use Decl -> VarDecl;
        PARSE(varDecl, VarDecl);
    }

    return true;
}

bool Parser::parseConstDecl(ConstDecl* root) {
    log(root);

    // only ConstDecl -> 'const' BType ConstDef { ',' ConstDef } ';'

    assert(CUR_TOKEN_IS(TokenType::CONSTTK)); // can delete
    PARSE_TOKEN(TokenType::CONSTTK);

    PARSE(sType, BType);

    PARSE(constDef1, ConstDef);

    while(CUR_TOKEN_IS(TokenType::COMMA)) {
        PARSE_TOKEN(TokenType::COMMA);

        PARSE(constDef2, ConstDef);
    }

    assert(CUR_TOKEN_IS(TokenType::SEMICN));
    PARSE_TOKEN(TokenType::SEMICN);

    return true;
}

bool Parser::parseBType(BType* root) {
    log(root);

    if(CUR_TOKEN_IS(TokenType::INTTK)) {
        // use BType -> 'int'
        PARSE_TOKEN(TokenType::INTTK);
    } else if(CUR_TOKEN_IS(TokenType::FLOATTK)) {
        // use BType -> 'float'
        PARSE_TOKEN(TokenType::FLOATTK);
    } else {
        assert(0 && "not a valid token");
    }

    return true;
}

bool Parser::parseConstDef(ConstDef* root) {
    log(root);

    // only ConstDef -> Ident { '[' ConstExp ']' } '=' ConstInitVal;
    assert(CUR_TOKEN_IS(TokenType::IDENFR));
    PARSE_TOKEN(TokenType::IDENFR);

    while(CUR_TOKEN_IS(TokenType::LBRACK)) {
        PARSE_TOKEN(TokenType::LBRACK);
        PARSE(constExp, ConstExp);
        PARSE_TOKEN(TokenType::RBRACK);
    }

    assert(CUR_TOKEN_IS(TokenType::ASSIGN));
    PARSE_TOKEN(TokenType::ASSIGN);

    PARSE(constInitVal, ConstInitVal);

    return true;
}

bool Parser::parseConstInitVal(ConstInitVal* root) {
    log(root);
    // ConstInitVal -> ConstExp | '{' [ ConstInitVal { ',' ConstInitVal } ] '}'

    if(CUR_TOKEN_IS(TokenType::LBRACE)) {
        // use ConstInitVal -> '{' [ ConstInitVal { ',' ConstInitVal } ] '}'

        PARSE_TOKEN(TokenType::LBRACE);
        if(CUR_TOKEN_IS(TokenType::RBRACE)) {
            // [ ConstInitVal { ',' ConstInitVal } ] is empty
            // do nothing

        } else {
            // [ ConstInitVal { ',' ConstInitVal } ] is not empty
            PARSE(constInitVal_out, ConstInitVal);
            while(CUR_TOKEN_IS(TokenType::COMMA)) {
                PARSE_TOKEN(TokenType::COMMA);
                PARSE(constInitVal_in, ConstInitVal);
            }
        }
        assert(CUR_TOKEN_IS(TokenType::RBRACE));
        PARSE_TOKEN(TokenType::RBRACE);
    } else {
        // use ConstInitVal -> ConstExp
        PARSE(constExp, ConstExp);
    }

    return true;
}

bool Parser::parseVarDecl(VarDecl* root) {
    log(root);
    // only VarDecl -> BType VarDef { ',' VarDef } ';'

    PARSE(sType, BType);
    PARSE(varDef_out, VarDef);

    while(CUR_TOKEN_IS(TokenType::COMMA)) {
        PARSE_TOKEN(TokenType::COMMA);
        PARSE(varDef_in, VarDef);
    }

    PARSE_TOKEN(TokenType::SEMICN);
    return true;
}

bool Parser::parseVarDef(VarDef* root) {
    log(root);
    // only VarDef -> Ident { '[' ConstExp ']' } [ '=' InitVal ]

    PARSE_TOKEN(TokenType::IDENFR);
    
    while(CUR_TOKEN_IS(TokenType::LBRACK)) {
        PARSE_TOKEN(TokenType::LBRACK);
        PARSE(constExp, ConstExp);
        PARSE_TOKEN(TokenType::RBRACK);
    }

    if(CUR_TOKEN_IS(TokenType::ASSIGN)) {
        PARSE_TOKEN(TokenType::ASSIGN);
        PARSE(initVal, InitVal);
    }
    return true;
}

bool Parser::parseInitVal(InitVal* root) {
    log(root);

    // InitVal -> Exp | '{' [ InitVal { ',' InitVal } ] '}'
    if(CUR_TOKEN_IS(TokenType::LBRACE)) {
        // use InitVal -> '{' [ InitVal { ',' InitVal } ] '}'
        PARSE_TOKEN(TokenType::LBRACE);

        if(CUR_TOKEN_IS(TokenType::RBRACE)) {
            //  [ InitVal { ',' InitVal } ] is empty
            // do nothing
        } else {
            PARSE(initVal_out, InitVal);
            while(CUR_TOKEN_IS(TokenType::COMMA)) {
                PARSE_TOKEN(TokenType::COMMA);
                PARSE(initVal_in, InitVal);
            }
        }
        PARSE_TOKEN(TokenType::RBRACE);
    } else {
        // use InitVal -> Exp
        PARSE(exp, Exp);
    }
    return true;
}

bool Parser::parseFuncDef(FuncDef* root) {
    log(root);

    // only FuncDef -> FuncType Ident '(' [FuncFParams] ')' Block
    PARSE(funcType, FuncType);
    PARSE_TOKEN(TokenType::IDENFR);
    PARSE_TOKEN(TokenType::LPARENT);

    if(CUR_TOKEN_IS(TokenType::RPARENT)) {
        // [FuncFParams] is empty
        // do nothing
    } else {
        PARSE(funcFParams, FuncFParams);
    }
    PARSE_TOKEN(TokenType::RPARENT);
    PARSE(slock, Block);
    return true;
}

bool Parser::parseFuncType(FuncType* root) {
    log(root);
    
    if(CUR_TOKEN_IS(TokenType::VOIDTK)) {
        // use FuncType -> 'void'
        PARSE_TOKEN(TokenType::VOIDTK);
    } else if(CUR_TOKEN_IS(TokenType::INTTK)) {
        // use FuncType -> 'int'
        PARSE_TOKEN(TokenType::INTTK);
    } else if(CUR_TOKEN_IS(TokenType::FLOATTK)) {
        // use FuncType -> 'float'
        PARSE_TOKEN(TokenType::FLOATTK);
    } else {
        assert(0 && "token type is invalid!");
    }

    return true;
}

bool Parser::parseFuncFParam(FuncFParam* root) {
    log(root);

    // only FuncFParam -> BType Ident ['[' ']' { '[' Exp ']' }]
    PARSE(sType, BType);
    PARSE_TOKEN(TokenType::IDENFR);

    if(CUR_TOKEN_IS(TokenType::LBRACK)) {
        PARSE_TOKEN(TokenType::LBRACK);
        PARSE_TOKEN(TokenType::RBRACK);
    }

    while(CUR_TOKEN_IS(TokenType::LBRACK)) {
        PARSE_TOKEN(TokenType::LBRACK);
        PARSE(exp, Exp);
        PARSE_TOKEN(TokenType::RBRACK);
    }

    return true;
}

bool Parser::parseFuncFParams(FuncFParams* root) {
    log(root);

    // only FuncFParams -> FuncFParam { ',' FuncFParam }

    PARSE(funcFParam_out, FuncFParam);
    while(CUR_TOKEN_IS(TokenType::COMMA)) {
        PARSE_TOKEN(TokenType::COMMA);
        PARSE(funcFParam_in, FuncFParam);
    }
    return true;
}

bool Parser::parseBlock(Block* root) {
    log(root);

    // only  '{' { BlockItem } '}'

    PARSE_TOKEN(TokenType::LBRACE);
    
    while(!CUR_TOKEN_IS(TokenType::RBRACE)) {
        PARSE(slockItem, BlockItem);
    }

    PARSE_TOKEN(TokenType::RBRACE);
    return true;
}

bool Parser::parseBlockItem(BlockItem* root) {
    log(root);

    if(CUR_TOKEN_IS(TokenType::CONSTTK) || CUR_TOKEN_IS(TokenType::INTTK) || CUR_TOKEN_IS(TokenType::FLOATTK)) {
        // use BlockItem -> Decl
        PARSE(decl, Decl);
    } else {
        // use BlockItem -> Stmt
        PARSE(stmt, Stmt);
    }
    return true;
}

bool Parser::parseStmt(Stmt* root) {
    log(root);

    // change to Stmt -> LVal '=' Exp ';' | Block | 'if' '(' Cond ')' Stmt [ 'else' Stmt ] | 'while' '(' Cond ')' Stmt | 'break' ';' | 'continue' ';' | 'return' [Exp] ';' | ';' | Exp ';'

    

    if(CUR_TOKEN_IS(TokenType::LBRACE)) {
        // use Stmt -> Block
        PARSE(slock, Block);
    } else if(CUR_TOKEN_IS(TokenType::IFTK)) {
        // use Stmt -> 'if'
        PARSE_TOKEN(TokenType::IFTK);
        PARSE_TOKEN(TokenType::LPARENT);
        PARSE(cond, Cond);
        PARSE_TOKEN(TokenType::RPARENT);
        PARSE(stmt, Stmt);
        if(CUR_TOKEN_IS(TokenType::ELSETK)) {
            //  [ 'else' Stmt ] is not empty
            PARSE_TOKEN(TokenType::ELSETK);
            PARSE(stmt, Stmt);
        }
    } else if(CUR_TOKEN_IS(TokenType::WHILETK)) {
        // use Stmt -> 'while'
        PARSE_TOKEN(TokenType::WHILETK);
        PARSE_TOKEN(TokenType::LPARENT);
        PARSE(cond, Cond);
        PARSE_TOKEN(TokenType::RPARENT);
        PARSE(stmt, Stmt);
    } else if(CUR_TOKEN_IS(TokenType::BREAKTK)) {
        // use Stmt -> 'break'
        PARSE_TOKEN(TokenType::BREAKTK);
        PARSE_TOKEN(TokenType::SEMICN);
    } else if(CUR_TOKEN_IS(TokenType::CONTINUETK)) {
        // use Stmt -> 'continue'
        PARSE_TOKEN(TokenType::CONTINUETK);
        PARSE_TOKEN(TokenType::SEMICN);
    } else if(CUR_TOKEN_IS(TokenType::RETURNTK)) {
        // use Stmt -> 'return'
        PARSE_TOKEN(TokenType::RETURNTK);
        if(!CUR_TOKEN_IS(TokenType::SEMICN)) {
            // [Exp] is not empty
            PARSE(exp, Exp);
        }
        PARSE_TOKEN(TokenType::SEMICN);
    } else if(CUR_TOKEN_IS(TokenType::SEMICN)) {
        // use Stmt -> ';'
        PARSE_TOKEN(TokenType::SEMICN);
    } else {
        // Stmt -> LVal '=' Exp ';' | Exp ';'
        // first is "var[][]...[] = Exp"
        // second is "Exp"
        // '=' token exists in first but not exists in second
        // exist ';' in both
        // search for next ';' and check whether exist '=' token

        uint32_t assign_token_pos = index;
        bool exist_assign_token = false;
        while(assign_token_pos < token_stream.size() && token_stream[assign_token_pos].type != TokenType::SEMICN) {
            if(token_stream[assign_token_pos].type == TokenType::ASSIGN) {
                exist_assign_token = true;
                break;
            }
            assign_token_pos++;
        }
        if(exist_assign_token) {
            // use Stmt -> LVal '=' Exp ';'
            PARSE(lVal, LVal);
            PARSE_TOKEN(TokenType::ASSIGN);
            PARSE(exp, Exp);
            PARSE_TOKEN(TokenType::SEMICN);
        } else {
            // use Stmt -> Exp ';'
            PARSE(exp, Exp);
            PARSE_TOKEN(TokenType::SEMICN);
        }
    }
    return true;
}

bool Parser::parseExp(Exp* root) {
    log(root);

    // only Exp -> AddExp
    PARSE(addExp, AddExp);

    return true;
}

bool Parser::parseCond(Cond* root) {
    log(root);
    // only Cond -> LOrExp

    PARSE(lOrExp, LOrExp);

    return true;
}

bool Parser::parseLVal(LVal* root) {
    log(root);

    // only LVal -> Ident {'[' Exp ']'}
    PARSE_TOKEN(TokenType::IDENFR);
    while(CUR_TOKEN_IS(TokenType::LBRACK)) {
        PARSE_TOKEN(TokenType::LBRACK);
        PARSE(exp, Exp);
        PARSE_TOKEN(TokenType::RBRACK);
    }
    return true;
}

bool Parser::parseNumber(Number* root) {
    log(root);

    if(CUR_TOKEN_IS(TokenType::INTLTR)) {
        // use Number -> IntConst
        PARSE_TOKEN(TokenType::INTLTR);
    } else if(CUR_TOKEN_IS(TokenType::FLOATLTR)) {
        // use Number -> floatConst
        PARSE_TOKEN(TokenType::FLOATLTR);
    } else {
        assert(0 && "invalid token type");
    }
    return true;
}

bool Parser::parsePrimaryExp(PrimaryExp* root) {
    log(root);
    // PrimaryExp -> '(' Exp ')' | LVal | Number
    if(CUR_TOKEN_IS(TokenType::LPARENT)) {
        // use PrimaryExp -> '(' Exp ')'
        PARSE_TOKEN(TokenType::LPARENT);
        PARSE(exp, Exp);
        PARSE_TOKEN(TokenType::RPARENT);
    } else if(CUR_TOKEN_IS(TokenType::IDENFR)) {
        // use PrimaryExp -> LVal
        PARSE(lVal, LVal);
    } else if(CUR_TOKEN_IS(TokenType::INTLTR) || CUR_TOKEN_IS(TokenType::FLOATLTR)) {
        // use PrimaryExp -> Number
        PARSE(number, Number);
    }

    return true;
}

bool Parser::parseUnaryExp(UnaryExp* root) {
    log(root);
    // UnaryExp -> PrimaryExp | Ident '(' [FuncRParams] ')' | UnaryOp UnaryExp

    if(CUR_TOKEN_IS(TokenType::IDENFR) && NEXT_TOKEN_IS(TokenType::LPARENT)) {
        // use UnaryExp -> Ident '(' [FuncRParams] ')'
        PARSE_TOKEN(TokenType::IDENFR);
        PARSE_TOKEN(TokenType::LPARENT);

        if(!CUR_TOKEN_IS(TokenType::RPARENT)) {
            // [FuncRParams] is not empty
            PARSE(funcRParams, FuncRParams);
        }
        PARSE_TOKEN(TokenType::RPARENT);
    } else if(CUR_TOKEN_IS(TokenType::PLUS) || CUR_TOKEN_IS(TokenType::MINU) || CUR_TOKEN_IS(TokenType::NOT)) {
        // use UnaryExp -> UnaryOp UnaryExp
        PARSE(unaryOp, UnaryOp);
        PARSE(unaryExp, UnaryExp);
    } else {
        // use UnaryExp -> PrimaryExp
        PARSE(primaryExp, PrimaryExp);
    }
    return true;
}

bool Parser::parseUnaryOp(UnaryOp* root) {
    log(root);

    if(CUR_TOKEN_IS(TokenType::PLUS)) {
        // use UnaryOp -> '+'
        PARSE_TOKEN(TokenType::PLUS);
    } else if(CUR_TOKEN_IS(TokenType::MINU)) {
        // use UnaryOp -> '-'
        PARSE_TOKEN(TokenType::MINU);
    } else if(CUR_TOKEN_IS(TokenType::NOT)) {
        // use UnaryOp -> '!'
        PARSE_TOKEN(TokenType::NOT);
    } else {
        assert(0 && "invalid token type");
    }
    return true;
}

bool Parser::parseFuncRParams(FuncRParams* root) {
    log(root);
    // only FuncRParams -> Exp { ',' Exp }
    PARSE(exp_out, Exp);
    while(CUR_TOKEN_IS(TokenType::COMMA)) {
        PARSE_TOKEN(TokenType::COMMA);
        PARSE(exp_in, Exp);
    }
    return true;
}

bool Parser::parseMulExp(MulExp* root) {
    log(root);
    // MulExp -> UnaryExp { ('*' | '/' | '%') UnaryExp }
    PARSE(unaryExp_out, UnaryExp);
    while(CUR_TOKEN_IS(TokenType::MULT) || CUR_TOKEN_IS(TokenType::DIV) || CUR_TOKEN_IS(TokenType::MOD)) {
        if(CUR_TOKEN_IS(TokenType::MULT)) {
            PARSE_TOKEN(TokenType::MULT);
        } else if(CUR_TOKEN_IS(TokenType::DIV)) {
            PARSE_TOKEN(TokenType::DIV);
        } else if(CUR_TOKEN_IS(TokenType::MOD)) {
            PARSE_TOKEN(TokenType::MOD);
        }
        PARSE(unaryExp_in, UnaryExp);
    }
    return true;
}

bool Parser::parseAddExp(AddExp* root) {
    log(root);
    // only AddExp -> MulExp { ('+' | '-') MulExp }
    PARSE(mulExp_out, MulExp);
    while(CUR_TOKEN_IS(TokenType::PLUS) || CUR_TOKEN_IS(TokenType::MINU)) {
        if(CUR_TOKEN_IS(TokenType::PLUS)) {
            PARSE_TOKEN(TokenType::PLUS);
        } else if(CUR_TOKEN_IS(TokenType::MINU)) {
            PARSE_TOKEN(TokenType::MINU);
        }
        PARSE(mulExp_in, MulExp);
    }
    return true;
}

bool Parser::parseRelExp(RelExp* root) {
    log(root);
    // RelExp -> AddExp { ('<' | '>' | '<=' | '>=') AddExp }
    PARSE(addExp_out, AddExp);
    while(CUR_TOKEN_IS(TokenType::LEQ) || CUR_TOKEN_IS(TokenType::GEQ) || CUR_TOKEN_IS(TokenType::GTR) || CUR_TOKEN_IS(TokenType::LSS)) {
        if(CUR_TOKEN_IS(TokenType::LEQ)) {
            PARSE_TOKEN(TokenType::LEQ);
        } else if(CUR_TOKEN_IS(TokenType::GEQ)) {
            PARSE_TOKEN(TokenType::GEQ);
        } else if(CUR_TOKEN_IS(TokenType::GTR)) {
            PARSE_TOKEN(TokenType::GTR);
        } else if(CUR_TOKEN_IS(TokenType::LSS)) {
            PARSE_TOKEN(TokenType::LSS);
        }
        PARSE(addExp_in, AddExp);
    }
    return true;
}

bool Parser::parseEqExp(EqExp* root) {
    log(root);
    // EqExp -> RelExp { ('==' | '!=') RelExp }
    PARSE(relExp_out, RelExp);
    while(CUR_TOKEN_IS(TokenType::EQL) || CUR_TOKEN_IS(TokenType::NEQ)) {
        if(CUR_TOKEN_IS(TokenType::EQL)) {
            PARSE_TOKEN(TokenType::EQL);
        } else if(CUR_TOKEN_IS(TokenType::NEQ)) {
            PARSE_TOKEN(TokenType::NEQ);
        }
        PARSE(relExp_in, RelExp);
    }
    return true;
}

bool Parser::parseLAndExp(LAndExp* root) {
    log(root);
    // LAndExp -> EqExp [ '&&' LAndExp ]
    PARSE(eqExp, EqExp);
    if(CUR_TOKEN_IS(TokenType::AND)) {
        // [ '&&' LAndExp ] is not empty
        PARSE_TOKEN(TokenType::AND);
        PARSE(lAndExp, LAndExp);
    }
    return true;
}

bool Parser::parseLOrExp(LOrExp* root) {
    log(root);
    // LOrExp -> LAndExp [ '||' LOrExp ]
    PARSE(lAndExp, LAndExp);
    if(CUR_TOKEN_IS(TokenType::OR)) {
        // [ '||' LOrExp ]
        PARSE_TOKEN(TokenType::OR);
        PARSE(lOrExp, LOrExp);
    }
    return true;
}

bool Parser::parseConstExp(ConstExp* root) {
    log(root);

    // only ConstExp -> AddExp
    PARSE(addExp, AddExp);

    return true;
}

void Parser::log(AstNode* node){
#ifdef DEBUG_PARSER
        std::cout << "in parse" << toString(node->type) << ", cur_token_type::" << toString(token_stream[index].type) << ", token_val::" << token_stream[index].value << '\n';
#endif
}