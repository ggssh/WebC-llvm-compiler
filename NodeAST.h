// AST语法树结点
// Created by 金韬 on 2020/9/21.
//

#ifndef LLVM_KALEIDOSCOPE_NODEAST_H
#define LLVM_KALEIDOSCOPE_NODEAST_H

#include <string>
#include <utility>
#include <vector>

#include "Global.h"

using namespace std;
using namespace llvm;

enum ASTType {
    TopAST = 0,
};

//////////////////// 基类结点 ///////////////////////

/// 基结点
class NodeAST {
public:

    virtual ~NodeAST() = default;

    virtual llvm::Value *codegen() = 0;
};

/// 表达式，如数字、代码块、赋值语句、二元操作语句
class ExpressionAST : public NodeAST {
};

/// 声明，如变量声明、函数声明
class StatementAST : public NodeAST {
};

class ExpressionStatementAST : public NodeAST {
    ExpressionAST* expr;
public:
    ExpressionStatementAST(ExpressionAST *expr);

    Value *codegen() override;
};

/// 块结点，一个块含有多种声明，也可作为一个程序的入口点
class BlockAST : public NodeAST {
public:
    vector<StatementAST *> statements;

    Value *codegen() override;
};

//////////////////// 用于存储数字的结点 ///////////////////////

// 标识符
class IdentifierExprAST : public ExpressionAST {
public:
    std::string identifier;

    explicit IdentifierExprAST(std::string identifier) : identifier(std::move(identifier)) {}

    /// codegen（）方法表示为该AST节点发出IR及其依赖的所有内容，并且它们都返回一个LLVM Value对象。
    /// “Value”是用于表示 LLVM中的“ 静态单一分配（SSA）寄存器”或“SSA值”的类。
    /// SSA值的最独特之处在于它们的值是在相关指令执行时计算的，并且在指令重新执行之前（以及如果）它不会获得新值。
    /// 换句话说，没有办法“改变”SSA值。
    llvm::Value *codegen() override;
};

// 变量声明语句
class VariableDeclarationAST : public ExpressionAST {
public:
    // 变量类型
    std::string type;
    // 变量名
    std::string identifier;
    // 可能有赋值
    ExpressionAST* expr;

    VariableDeclarationAST(const string &type, const string &identifier);

    /// codegen（）方法表示为该AST节点发出IR及其依赖的所有内容，并且它们都返回一个LLVM Value对象。
    /// “Value”是用于表示 LLVM中的“ 静态单一分配（SSA）寄存器”或“SSA值”的类。
    /// SSA值的最独特之处在于它们的值是在相关指令执行时计算的，并且在指令重新执行之前（以及如果）它不会获得新值。
    /// 换句话说，没有办法“改变”SSA值。
    llvm::Value *codegen() override;
};

// 变量声明语句
class VariableAssignmentAST : public ExpressionAST {
public:
    // 变量名
    std::string identifier;
    // 可能有赋值
    ExpressionAST* expr;

    VariableAssignmentAST(const string &identifier, ExpressionAST *expr);

    /// codegen（）方法表示为该AST节点发出IR及其依赖的所有内容，并且它们都返回一个LLVM Value对象。
    /// “Value”是用于表示 LLVM中的“ 静态单一分配（SSA）寄存器”或“SSA值”的类。
    /// SSA值的最独特之处在于它们的值是在相关指令执行时计算的，并且在指令重新执行之前（以及如果）它不会获得新值。
    /// 换句话说，没有办法“改变”SSA值。
    llvm::Value *codegen() override;
};


/// 数值语法结点
class DoubleExprAST : public ExpressionAST {
public:
    double Val;

    explicit DoubleExprAST(double val) : Val(val) {}

    /// codegen（）方法表示为该AST节点发出IR及其依赖的所有内容，并且它们都返回一个LLVM Value对象。
    /// “Value”是用于表示 LLVM中的“ 静态单一分配（SSA）寄存器”或“SSA值”的类。
    /// SSA值的最独特之处在于它们的值是在相关指令执行时计算的，并且在指令重新执行之前（以及如果）它不会获得新值。
    /// 换句话说，没有办法“改变”SSA值。
    llvm::Value *codegen() override;
};

/// 数值语法结点
class IntegerExprAST : public ExpressionAST {
public:
    int Val;

    explicit IntegerExprAST(int val) : Val(val) {}

    /// codegen（）方法表示为该AST节点发出IR及其依赖的所有内容，并且它们都返回一个LLVM Value对象。
    /// “Value”是用于表示 LLVM中的“ 静态单一分配（SSA）寄存器”或“SSA值”的类。
    /// SSA值的最独特之处在于它们的值是在相关指令执行时计算的，并且在指令重新执行之前（以及如果）它不会获得新值。
    /// 换句话说，没有办法“改变”SSA值。
    llvm::Value *codegen() override;
};

/// 变量结点
class VariableExprAST : public NodeAST {
    std::string Name;
public:
    explicit VariableExprAST(std::string &Name) : Name(Name) {}

    llvm::Value *codegen() override;
};

/// 二元操作结点
class BinaryExprAST : public ExpressionAST {
    // 操作符
    char Oper;
    ExpressionAST* LEA;
    ExpressionAST* REA;
public:
    BinaryExprAST(char oper, ExpressionAST *lea, const ExpressionAST &rea);

    llvm::Value *codegen() override;
};

/// 函数调用结点
class CallExprAST : public ExpressionAST {
    std::string callName;
    std::vector<ExpressionAST*> args;

public:
    CallExprAST(std::string callName, std::vector<ExpressionAST*> args) : callName(std::move(callName)),
                                                                                    args(std::move(args)) {};

    llvm::Value *codegen() override;
};

/// 函数描述结点
/// 用于描述函数名字，参数名称，参数个数
class PrototypeAST : public NodeAST {
    std::string returnType;
    std::string name;
    std::vector<VariableDeclarationAST> args;

public:
    PrototypeAST(const string &returnType, const string &name, const vector<VariableDeclarationAST> &args);

    llvm::Function *codegen() override;

    const std::string &getName() const;
};

/// 函数结点
class FunctionAST : public NodeAST {
    PrototypeAST* Proto;
    BlockAST* Body;
public:
    FunctionAST(PrototypeAST *proto, BlockAST *body);

    llvm::Function *codegen() override;
};

/// 一元结点 - Expression class for a unary operator.
/// 从JIT弄来的代码，目前不需要
class UnaryExprAST : public NodeAST {
    char Opcode;
    std::unique_ptr<NodeAST> Operand;

public:
    UnaryExprAST(char Opcode, std::unique_ptr<NodeAST> Operand)
            : Opcode(Opcode), Operand(std::move(Operand)) {}
};


/// 条件结点 TODO 加入一个exit，可以退出条件结点
class ConditionAST : public NodeAST {
    std::unique_ptr<NodeAST> if_cond;
    std::unique_ptr<NodeAST> if_stmt;
    std::unique_ptr<NodeAST> else_stmt;
public:

    ConditionAST(std::unique_ptr<NodeAST> if_cond, std::unique_ptr<NodeAST> else_cond) : if_cond(std::move(if_cond)),
                                                                                         else_stmt(std::move(
                                                                                                 else_cond)) {}

    llvm::Value *codegen() override;
};

/// For结点
class ForExprAST : public NodeAST {
    std::string VarName;
    // 意思为：for (Start,End,Step){Body}
    std::unique_ptr<NodeAST> Start, End, Step, Body;

public:
    ForExprAST(const std::string &VarName, std::unique_ptr<NodeAST> Start,
               std::unique_ptr<NodeAST> End, std::unique_ptr<NodeAST> Step,
               std::unique_ptr<NodeAST> Body)
            : VarName(VarName), Start(std::move(Start)), End(std::move(End)),
              Step(std::move(Step)), Body(std::move(Body)) {}

    llvm::Value *codegen() override;
};

#endif //LLVM_KALEIDOSCOPE_NODEAST_H