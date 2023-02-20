#ifndef IROP_H
#define IROP_H

#include <string>


namespace ir {

// FIXME
// dyt: 我们如果可以支持 int/float 字面量, 即在 type 上支持了立即数, 其实就可以增加 IR 的通用性, 如
//      add IRinstruction 里面可以放一个 type 为 intliteral 的 operand, 就达到了 addi 的作用
// 那 addi 拿来干啥
// 我觉得这里可以讨论一下 
enum class Operator {
    _return,    // return   op1
    _goto,      // goto     [op1=condition],   des = offset

    call,       // call     op1=func des=t1(returnVal)  /* func.name = function, func.type = return type*/
    alloc,    //用于数组声明   op1 = length(数组长度，各维相乘), des = 数组名
    store,  //a[2] = 3;  store des(a) op1(3) op2(2)    op2为下标 -> 偏移量  des为store的块名(数组名)
    load, //a = b[1];  load des(a) op1(b) op2(1)    op2为下标 -> 偏移量  op1为load的块名(数组名)

    def,    //立即数给变量赋值，如 a = 1;
    fdef,
    mov,   //变量给变量赋值，如 a = t1;
    fmov,
    cvt_i2f,    // convert [Int]op1 to [Float]des 
    cvt_f2i,    // convert [Float]op1 to [Int]des
    add,
    addi,
    fadd,
    sub,
    subi,
    fsub,
    // rsbi,    // 用 sub,0,a 代表 -a
    // frsbi,
    mul,
    fmul,
    div,
    fdiv,
    mod,
    lss,
    flss,
    // lssi,
    leq,
    fleq,
    // leqi,
    gtr,
    fgtr,
    // gtri,
    geq,
    fgeq,
    // geqi,
    eq,
    feq,
    // eqi,
    neq,
    fneq,
    // neqi,
    _not,
    _and,
    _or
};

std::string toString(Operator t);

}
#endif
