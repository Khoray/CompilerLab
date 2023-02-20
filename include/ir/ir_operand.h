#ifndef IROPERAND_H
#define IROPERAND_H

#include <string>


namespace ir {

enum class Type {
    Int,
    Float,
    IntLiteral,
    FloatLiteral,
    IntPtr,
    FloatPtr,
    null
};

std::string toString(Type t);

struct Operand {
    std::string name;
    Type type;
    Operand();
    // FIXME 
    // dyt:这个玩意儿就不应该支持啊 怎么可以初始化一个没有类型的但是有名字的东西呢
    // Operand(const std::string&);     
    Operand(std::string , Type);
    // FIXME
    // dyt: 这里是不是该返回个引用啥的 为啥允许 operator= 嘞
    Operand& operator=(const Operand&);
};

}
#endif
