#include"front/syntax.h"

#include<iostream>
#include<cassert>

using frontend::Parser;

// #define DEBUG_PARSER
#define TODO assert(0 && "todo")
#define CUR_TOKEN_IS(tk_type) (token_stream[index].type == tk_type)
#define NEXT_TOKEN_IS(tk_type) (index + 1 < (int) token_stream.size() && token_stream[index + 1].type == tk_type)
#define NEXT_NEXT_TOKEN_IS(tk_type) (index + 2 < (int) token_stream.size() && token_stream[index + 2].type == tk_type)
#define PARSE_TOKEN(tk_type) root->children.push_back(parseTerm(root, tk_type))
#define PARSE(name, type) auto name = new type(root); assert(parse##type(name)); root->children.push_back(name); 


Parser::Parser(const std::vector<frontend::Token>& tokens): index(0), token_stream(tokens) {}

Parser::~Parser() {}

frontend::CompUnit* Parser::get_abstract_syntax_tree(){
    TODO;
}

bool Parser::parseCompUnit(CompUnit* root) {
    log(root);

    if(NEXT_NEXT_TOKEN_IS(TokenType::LPARENT)) {
        // use CompUnit -> FuncDef
        PARSE(FuncDef, FuncDef);
    } else {
        // use FuncDef -> Decl
        PARSE(Decl, Decl);
    }

    if(index + 1 < (int) token_stream.size()) {
        PARSE(CompUnit, CompUnit);
    }

    return true;
}

bool Parser::parseDecl(Decl* root) {
    log(root);

    if(CUR_TOKEN_IS(TokenType::CONSTTK)) {
        // use Decl -> ConstDecl
        PARSE(ConstDecl, ConstDecl);
    } else {
        // use Decl -> VarDecl;
        PARSE(VarDecl, VarDecl);
    }

    return true;
}

bool Parser::parseConstDecl(ConstDecl* root) {
    log(root);

    // only ConstDecl -> 'const' BType ConstDef { ',' ConstDef } ';'

    assert(CUR_TOKEN_IS(TokenType::CONSTTK)); // can delete
    PARSE_TOKEN(TokenType::CONSTTK);

    PARSE(BType, BType);

    PARSE(ConstDef1, ConstDef);

    while(CUR_TOKEN_IS(TokenType::COMMA)) {
        PARSE_TOKEN(TokenType::COMMA);

        PARSE(ConstDef2, ConstDef);
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
        PARSE(ConstExp, ConstExp);
        PARSE_TOKEN(TokenType::RBRACK);
    }

    assert(CUR_TOKEN_IS(TokenType::ASSIGN));
    PARSE_TOKEN(TokenType::ASSIGN);

    PARSE(ConstInitVal, ConstInitVal);

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
            PARSE(ConstInitVal_out, ConstInitVal);
            while(CUR_TOKEN_IS(TokenType::COMMA)) {
                PARSE_TOKEN(TokenType::COMMA);
                PARSE(ConstInitVal_in, ConstInitVal);
            }
        }
        assert(CUR_TOKEN_IS(TokenType::RBRACE));
        PARSE_TOKEN(TokenType::RBRACE);
    } else {
        // use ConstInitVal -> ConstExp
        PARSE(ConstExp, ConstExp);
    }

    return true;
}

bool Parser::parseVarDecl(VarDecl* root) {
    log(root);
    // only VarDecl -> BType VarDef { ',' VarDef } ';'

    PARSE(BType, BType);
    PARSE(VarDef_out, VarDef);

    while(CUR_TOKEN_IS(TokenType::COMMA)) {
        PARSE_TOKEN(TokenType::COMMA);
        PARSE(VarDef_in, VarDef);
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
        PARSE(ConstExp, ConstExp);
        PARSE_TOKEN(TokenType::RBRACK);
    }

    if(CUR_TOKEN_IS(TokenType::ASSIGN)) {
        PARSE_TOKEN(TokenType::ASSIGN);
        PARSE(InitVal, InitVal);
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
            PARSE(InitVal_out, InitVal);
            while(CUR_TOKEN_IS(TokenType::COMMA)) {
                PARSE(InitVal_in, InitVal);
            }
        }
        PARSE_TOKEN(TokenType::RBRACE);
    } else {
        // use InitVal -> Exp
        PARSE(Exp, Exp);
    }
    return true;
}

bool Parser::parseFuncDef(FuncDef* root) {
    log(root);

    // only FuncDef -> FuncType Ident '(' [FuncFParams] ')' Block
    PARSE(FuncType, FuncType);
    PARSE_TOKEN(TokenType::IDENFR);
    PARSE_TOKEN(TokenType::LPARENT);

    if(CUR_TOKEN_IS(TokenType::RPARENT)) {
        // [FuncFParams] is empty
        // do nothing
    } else {
        PARSE(FuncFParams, FuncFParams);
    }
    PARSE_TOKEN(TokenType::RPARENT);
    PARSE(Block, Block);
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
    PARSE(BType, BType);
    PARSE_TOKEN(TokenType::IDENFR);

    if(CUR_TOKEN_IS(TokenType::LBRACK)) {
        PARSE_TOKEN(TokenType::LBRACK);
        PARSE_TOKEN(TokenType::RBRACK);
    }

    while(CUR_TOKEN_IS(TokenType::LBRACK)) {
        PARSE_TOKEN(TokenType::LBRACK);
        PARSE(Exp, Exp);
        PARSE_TOKEN(TokenType::RBRACK);
    }

    return true;
}

bool Parser::parseFuncFParams(FuncFParams* root) {
    log(root);

    // only FuncFParams -> FuncFParam { ',' FuncFParam }

    PARSE(FuncFParam_out, FuncFParam);
    while(CUR_TOKEN_IS(TokenType::COMMA)) {
        PARSE_TOKEN(TokenType::COMMA);
        PARSE(FuncFParam_in, FuncFParam);
    }
    return true;
}

bool Parser::parseBlock(Block* root) {
    log(root);

    // only  '{' { BlockItem } '}'

    PARSE_TOKEN(TokenType::LBRACE);
    
    while(!CUR_TOKEN_IS(TokenType::RBRACE)) {
        PARSE(BlockItem, BlockItem);
    }

    PARSE_TOKEN(TokenType::RBRACE);
    return true;
}

bool Parser::parseBlockItem(BlockItem* root) {
    log(root);

    if(CUR_TOKEN_IS(TokenType::CONSTTK) || CUR_TOKEN_IS(TokenType::INTTK) || CUR_TOKEN_IS(TokenType::FLOATTK)) {
        // use BlockItem -> Decl
        PARSE(Decl, Decl);
    } else {
        // use BlockItem -> Stmt
        PARSE(Stmt, Stmt);
    }
    return true;
}

bool Parser::parseStmt(Stmt* root) {
    log(root);

    // change to Stmt -> LVal '=' Exp ';' | Block | 'if' '(' Cond ')' Stmt [ 'else' Stmt ] | 'while' '(' Cond ')' Stmt | 'break' ';' | 'continue' ';' | 'return' [Exp] ';' | ';' | Exp ';'

    

    if(CUR_TOKEN_IS(TokenType::LBRACE)) {
        // use Stmt -> Block
        PARSE(Block, Block);
    } else if(CUR_TOKEN_IS(TokenType::IFTK)) {
        // use Stmt -> 'if'
        PARSE_TOKEN(TokenType::IFTK);
        PARSE_TOKEN(TokenType::LPARENT);
        PARSE(Cond, Cond);
        PARSE_TOKEN(TokenType::RPARENT);
        PARSE(Stmt, Stmt);
        if(CUR_TOKEN_IS(TokenType::ELSETK)) {
            //  [ 'else' Stmt ] is not empty
            PARSE_TOKEN(TokenType::ELSETK);
            PARSE(Stmt, Stmt);
        }
    } else if(CUR_TOKEN_IS(TokenType::WHILETK)) {
        // use Stmt -> 'while'
        PARSE_TOKEN(TokenType::WHILETK);
        PARSE_TOKEN(TokenType::LPARENT);
        PARSE(Cond, Cond);
        PARSE_TOKEN(TokenType::RPARENT);
        PARSE(Stmt, Stmt);
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
            PARSE(Exp, Exp);
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

        uint32_t assgin_token_pos = index;
        bool exist_assign_token = false;
        while(assgin_token_pos < (int) token_stream.size() && token_stream[assign_token_pos].type != TokenType::SEMICN) {
            if(token_stream[assign_token_pos].type == TokenType::ASSIGN) {
                exist_assign_token = true;
                break;
            }
        }
        if(exist_assign_token) {
            // use Stmt -> LVal '=' Exp ';'
            PARSE(LVal, LVal);
            PARSE_TOKEN(TokenType::ASSIGN);
            PARSE(Exp, Exp);
            PARSE_TOKEN(TokenType::SEMICN);
        } else {
            // use Stmt -> Exp ';'
            PARSE(Exp, Exp);
            PARSE_TOKEN(TokenType::SEMICN);
        }
    }
    return true;
}

bool Parser::parseExp(Exp* root) {
    log(root);

    // only Exp -> AddExp
    PARSE(AddExp, AddExp);

    return true;
}

bool Parser::parseCond(Cond* root) {
    log(root);
    // only Cond -> LOrExp

    PARSE(LOrExp, LOrExp);

    return true;
}

bool Parser::parseLVal(LVal* root) {
    log(root);
}

bool Parser::parseNumber(Number* root) {
    TODO;
}

bool Parser::parsePrimaryExp(PrimaryExp* root) {
    TODO;
}

bool Parser::parseUnaryExp(UnaryExp* root) {
    TODO;
}

bool Parser::parseUnaryOp(UnaryOp* root) {
    TODO;
}

bool Parser::parseFuncRParams(FuncRParams* root) {
    TODO;
}

bool Parser::parseMulExp(MulExp* root) {
    TODO;
}

bool Parser::parseAddExp(AddExp* root) {
    TODO;
}

bool Parser::parseRelExp(RelExp* root) {
    TODO;
}

bool Parser::parseEqExp(EqExp* root) {
    TODO;
}

bool Parser::parseLAndExp(LAndExp* root) {
    TODO;
}

bool Parser::parseLOrExp(LOrExp* root) {
    TODO;
}

bool Parser::parseConstExp(ConstExp* root) {
    TODO;
}



void Parser::log(AstNode* node){
#ifdef DEBUG_PARSER
        std::cout << "in parse" << toString(node->type) << ", cur_token_type::" << toString(token_stream[index].type) << ", token_val::" << token_stream[index].value << '\n';
#endif
}
