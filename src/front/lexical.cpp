#include"front/lexical.h"

#include<map>
#include<cassert>
#include<string>

using frontend::TokenType;

#define TODO assert(0 && "todo")

// #define DEBUG_DFA
// #define DEBUG_SCANNER

std::string frontend::toString(State s) {
    switch (s) {
    case State::Empty: return "Empty";
    case State::Ident: return "Ident";
    case State::IntLiteral: return "IntLiteral";
    case State::FloatLiteral: return "FloatLiteral";
    case State::op: return "op";
    default:
        assert(0 && "invalid State");
    }
    return "";
}

std::set<std::string> frontend::keywords= {
    "const", "int", "float", "if", "else", "while", "continue", "break", "return", "void"
};

frontend::DFA::DFA(): cur_state(frontend::State::Empty), cur_str() {}

frontend::DFA::~DFA() {}

enum CharType {
    Ident,
    Number,
    Op,
    FloatDot,
    Space
};

CharType get_char_type(char c) {
    if(islower(c) || isupper(c) || c == '_') {
        return CharType::Ident;
    } else if(isdigit(c)) {
        return CharType::Number;
    } else if(c == '.') {
        return CharType::FloatDot;
    } else if(c == ' ' || c == '\r' || c == '\n' || c == '\t') {
        return CharType::Space;
    } else {
        return CharType::Op;
    }
}

TokenType get_op_type(std::string s)  {
    if(s == "+") {
        return TokenType::PLUS;
    } else if(s == "-") {
        return TokenType::MINU;
    } else if(s == "(") {
        return TokenType::LPARENT;
    } else if(s == ")") {
        return TokenType::RPARENT;
    } else if(s == "*") {
        return TokenType::MULT;
    } else if(s == "/") {
        return TokenType::DIV;
    } else if(s == "%") {
        return TokenType::MOD;
    } else if(s == "<") {
        return TokenType::LSS;
    } else if(s == ">") {
        return TokenType::GTR;
    } else if(s == ":") {
        return TokenType::COLON;
    } else if(s == "=") {
        return TokenType::ASSIGN;
    } else if(s == ";") {
        return TokenType::SEMICN;
    } else if(s == ",") {
        return TokenType::COMMA;
    } else if(s == "[") {
        return TokenType::LBRACK;
    } else if(s == "]") {
        return TokenType::RBRACK;
    } else if(s == "{") {
        return TokenType::LBRACE;
    } else if(s == "}") {
        return TokenType::RBRACE;
    } else if(s == "!") {
        return TokenType::NOT;
    } else if(s == "<=") {
        return TokenType::LEQ;
    } else if(s == ">=") {
        return TokenType::GEQ;
    } else if(s == "==") {
        return TokenType::EQL;
    } else if(s == "!=") {
        return TokenType::NEQ;
    } else if(s == "&&") {
        return TokenType::AND;
    } else if(s == "||") {
        return TokenType::OR;
    } else {
        return TokenType::NOTTOKEN;
    }
}

TokenType get_keyword_type(std::string s) {
    if(s == "const") {
        return TokenType::CONSTTK;
    } else if(s == "int") {
        return TokenType::INTTK;
    } else if(s == "float") {
        return TokenType::FLOATTK;
    } else if(s == "if") {
        return TokenType::IFTK;
    } else if(s == "else") {
        return TokenType::ELSETK;
    } else if(s == "while") {
        return TokenType::WHILETK;
    } else if(s == "continue") {
        return TokenType::CONTINUETK;
    } else if(s == "break") {
        return TokenType::BREAKTK;
    } else if(s == "return") {
        return TokenType::RETURNTK;
    } else if(s == "void") {
        return TokenType::VOIDTK;
    } else {
        return TokenType::IDENFR;
    }
}
#define DEBUG_DFA
bool frontend::DFA::next(char input, Token& buf) {
#ifdef DEBUG_DFA
#include<iostream>
    std::cout << "in state [" << toString(cur_state) << "], input = \'" << input << "\', str = " << cur_str << "\t";
#endif
    int ope = 0b00;
    CharType ctype = get_char_type(input);
    State last_state = cur_state;

    switch(cur_state) {
    case State::Empty:
        ope = 0b01;

        switch(ctype) {
        case CharType::Ident:
            cur_state = State::Ident;
            break;

        case CharType::Number:
            cur_state = State::IntLiteral;
            break;
        
        case CharType::FloatDot:
            cur_state = State::op;
            break;

        case CharType::Space:
            cur_state = State::Empty;
            break;

        case CharType::Op:
            cur_state = State::op;
            break;

        default:
            assert(0 && "State::Empty input is invalid.");
            break;
        }
        break;

    case State::IntLiteral:

        switch(ctype) {
        case CharType::Number:
            cur_state = State::IntLiteral;
            break;

        case CharType::FloatDot:
            cur_state = State::FloatLiteral;
            break;
        
        case CharType::Op:
            ope = 0b11;
            cur_state = State::op;
            break;
        
        case CharType::Space:
            ope = 0b11;
            cur_state = State::Empty;
            break;
        
        case CharType::Ident: // only when hex or oct
            cur_state = State::IntLiteral;
            break;
        
        default:
            assert(0 && "State::IntLiteral input is invalid.");
            break;
        }
        break;
    
    case State::FloatLiteral:

        switch(ctype) {
        case CharType::Number:
            cur_state = State::FloatLiteral;
            break;

        case CharType::FloatDot:
            cur_state = State::FloatLiteral;
            break;
        
        case CharType::Op:
            ope = 0b11;
            cur_state = State::op;
            break;
        
        case CharType::Space:
            ope = 0b11;
            cur_state = State::Empty;
            break;

        case CharType::Ident: // only when input is exponent sign "e" or "E"
            cur_state = State::FloatLiteral;
            break;
        
        default:
            assert(0 && "State::FloatLiteral input is invalid.");
            break;
        }
        break;
    
    case State::Ident:

        switch(ctype) {
        case CharType::Number:
            cur_state = State::Ident;
            break;
        
        case CharType::Op:
            ope = 0b11;
            cur_state = State::op;
            break;
        
        case CharType::Space:
            ope = 0b11;
            cur_state = State::Empty;
            break;

        case CharType::Ident: // only when input is exponent sign "e" or "E"
            cur_state = State::Ident;
            break;
        
        default:
            assert(0 && "State::Ident input is invalid.");
            break;
        }
        break;
    
    case State::op:

        switch(ctype) {
        case CharType::Number:
            ope = 0b11;
            cur_state = State::IntLiteral;
            break;
        
        case CharType::Op: { // TODO
            std::string attach_str = cur_str + input;
            TokenType op_type = get_op_type(attach_str);

            cur_state = State::op;
            if(op_type != TokenType::NOTTOKEN) {

            } else {
                ope = 0b11;
            }
            break;
        }
        
        case CharType::Space:
            ope = 0b11;
            cur_state = State::Empty;
            break;

        case CharType::Ident:
            ope = 0b11;
            cur_state = State::Ident;
            break;

        case CharType::FloatDot:
            ope = 0b11;
            cur_state = State::FloatLiteral;
            break;
        
        default:
            assert(0 && "State::Ident input is invalid.");
            break;
        }
        break;
    
    default:
        break;
    }
    // std::cerr << input << " " << toString(cur_state) << " isd:" << isd(input) << " ope:" << ope << "\n";


    // out
    if(ope >> 1 & 1) {
        buf.value = cur_str;

        if(last_state == State::IntLiteral) {
            buf.type = TokenType::INTLTR;
        } else if(last_state == State::FloatLiteral) {
            buf.type = TokenType::FLOATLTR;
        } else if(last_state == State::op) {
            buf.type = get_op_type(cur_str);
        } else if(last_state == State::Ident) {
            buf.type = get_keyword_type(cur_str);
        } else {
            assert(0 && "should output but state is empty");
        }
    }

    // clear
    if(ope & 1) {
        cur_str = "";
    }
    cur_str += input;
    int ret = (ope >> 1 & 1);



#ifdef DEBUG_DFA
    std::cout << "next state is [" << toString(cur_state) << "], next str = " << cur_str << "\t, ret = " << ret << std::endl;
#endif
    return ret;
}

void frontend::DFA::reset() {
    cur_state = State::Empty;
    cur_str = "";
}

frontend::Scanner::Scanner(std::string filename): fin(filename) {
    if(!fin.is_open()) {
        assert(0 && "in Scanner constructor, input file cannot open");
    }
}

frontend::Scanner::~Scanner() {
    fin.close();
}

/**
 * @brief delete comments of original input
 */
std::string preprocess(std::ifstream& fin) {
    std::string ret;
    bool is_in_comment = false;
    while(fin) {
        std::string temp;
        getline(fin, temp);
        
        for(int i = 0; i < (int) temp.size(); i++) {
            if(is_in_comment) {
                if(temp[i] == '*' && i + 1 < (int) temp.size() && temp[i + 1] == '/') {
                    // /**/ comments
                    is_in_comment = false;
                    i++;
                }
                continue;
            }
            if(temp[i] == '/' && i + 1 < (int) temp.size() && temp[i + 1] == '*') {
                // /**/ comments
                is_in_comment = true;
                i++;
                continue;
            }
            if(temp[i] == '/' && i + 1 < (int) temp.size() && temp[i + 1] == '/') {
                // comments
                break;
            }
            if(temp[i] == '\t') {
                temp[i] = ' ';
            }
            ret += temp[i];
        }
        ret += ' ';
    }

    return ret;
}

std::vector<frontend::Token> frontend::Scanner::run() {
    std::vector<Token> ret;
    std::string input = preprocess(fin);
    DFA dfa;
    Token tk;

    for(char c : input) {
        if(dfa.next(c, tk)) {
            ret.push_back(tk);
        }
    }
    return ret;
#ifdef DEBUG_SCANNER
#include<iostream>
            std::cout << "token: " << toString(tk.type) << "\t" << tk.value << std::endl;
#endif
}