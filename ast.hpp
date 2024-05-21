#ifndef DREWNO_MARS_AST_HPP
#define DREWNO_MARS_AST_HPP

#include <ostream>
#include <list>
#include "tokens.hpp"
#include <cassert>


/* You'll probably want to add a bunch of ASTNode subclasses */

namespace drewno_mars{

/* You may find it useful to forward declare AST subclasses
   here so that you can use a class before it's full definition
*/
class DeclNode;
class TypeNode;
class StmtNode;
class IDNode;
class LocNode;

/** 
* \class ASTNode
* Base class for all other AST Node types
**/
class ASTNode{
public:
	ASTNode(const Position * p) : myPos(p){ }
	virtual void unparse(std::ostream& out, int indent) = 0;
	const Position * pos() { return myPos; }
	std::string posStr() { return pos()->span(); }
protected:
	const Position * myPos = nullptr;
};

/** 
* \class ProgramNode
* Class that contains the entire abstract syntax tree for a program.
* Note the list of declarations encompasses all global declarations
* which includes (obviously) all global variables and struct declarations
* and (perhaps less obviously), all function declarations
**/
class ProgramNode : public ASTNode{
public:
	ProgramNode(std::list<DeclNode *> * globalsIn) ;
	void unparse(std::ostream& out, int indent) override;
private:
	std::list<DeclNode * > * myGlobals;
};

/**  \class ExpNode
* Superclass for expression nodes (i.e. nodes that can be used as
* part of an expression).  Nodes that are part of an expression
* should inherit from this abstract superclass.
**/
class ExpNode : public ASTNode{
protected:
    ExpNode(const Position * p) : ASTNode(p){ }
public:
    virtual void unparse(std::ostream& out, int indent) = 0;
    virtual void nestedUnparse(std::ostream& out, int indent);
};

class CallExpNode : public ExpNode {
public:
    CallExpNode(const Position * p, LocNode * nameIn, std::list<ExpNode *> * argsIn) : ExpNode(p), functionName(nameIn), args(argsIn) { }
    void unparse(std::ostream& out, int indent) override;
    void nestedUnparse(std::ostream& out, int indent) override;
private:
    LocNode * functionName;
    std::list<ExpNode *> * args;
};

class FalseNode : public ExpNode {
public:
    FalseNode(const Position * p) : ExpNode(p) { }
    void unparse(std::ostream& out, int indent) override;
    void nestedUnparse(std::ostream& out, int indent) override;
};

class TrueNode : public ExpNode {
public:
    TrueNode(const Position * p) : ExpNode(p) { }
    void unparse(std::ostream& out, int indent) override;
    void nestedUnparse(std::ostream& out, int indent) override;
};

class MagicNode : public ExpNode {
public:
    MagicNode(const Position * p) : ExpNode(p) { }
    void unparse(std::ostream& out, int indent) override;
    void nestedUnparse(std::ostream& out, int indent) override;
};

class IntLitNode : public ExpNode {
public:
    IntLitNode(const Position * p, int valueIn) : ExpNode(p), value(valueIn) { }
    void unparse(std::ostream& out, int indent) override;
    void nestedUnparse(std::ostream& out, int indent) override;

private:
    int value;
};

class StrLitNode: public ExpNode {
public:
    StrLitNode(const Position * p, std::string strIn) : ExpNode(p), str(strIn) { }
    void unparse(std::ostream& out, int indent) override;
    void nestedUnparse(std::ostream& out, int indent) override;
private:
    std::string str;
};

/** A memory location. LocNodes subclass ExpNode
 * because they can be used as part of an expression.
**/
class LocNode : public ExpNode{
public:
    LocNode(const Position * p) : ExpNode(p) {}
    virtual void unparse(std::ostream& out, int indent) override = 0;
};

/** An identifier. Note that IDNodes subclass
 * LocNode because they are a type of memory location.
**/
class IDNode : public LocNode{
public:
    IDNode(const Position * p, std::string functionIn) : LocNode(p), name(functionIn){ }
    void unparse(std::ostream& out, int indent) override;
    void nestedUnparse(std::ostream& out, int indent) override;
private:
    /** The name of the identifier **/
    std::string name;
};

class MemberFieldExpNode : public LocNode {
public:
    MemberFieldExpNode(const Position * p, LocNode * locIn, IDNode * nameIn) : LocNode(p), loc(locIn), name(nameIn) { }
    void unparse(std::ostream& out, int indent) override;
private:
    LocNode * loc;
    IDNode * name;
};

class UnaryExpNode : public ExpNode {
public:
    UnaryExpNode(const Position * p, ExpNode * expIn) : ExpNode(p), exp(expIn) { }
    virtual void unparse(std::ostream& out, int indent) = 0;

protected:
    ExpNode * exp;
};

class NegNode : public UnaryExpNode {
public:
    NegNode(const Position * p, ExpNode * exp) : UnaryExpNode(p, exp) { }
    void unparse(std::ostream& out, int indent) override;
};

class NotNode : public UnaryExpNode {
public:
    NotNode(const Position * p, ExpNode * exp) : UnaryExpNode(p, exp) { }
    void unparse(std::ostream& out, int indent) override;
};

class BinaryExpNode : public ExpNode {
public:
    BinaryExpNode(const Position * p, ExpNode * lhsIn, ExpNode * rhsIn) : ExpNode(p), lhs(lhsIn), rhs(rhsIn) { }
    virtual void unparse(std::ostream& out, int indent) = 0;

protected:
    ExpNode * lhs;
    ExpNode * rhs;
};

class AndNode : public BinaryExpNode {
public:
    AndNode(const Position * p, ExpNode * lhs, ExpNode * rhs) : BinaryExpNode(p,lhs,rhs) { }
    void unparse(std::ostream& out, int indent) override;
};

class DivideNode : public BinaryExpNode {
public:
    DivideNode(const Position * p, ExpNode * lhs, ExpNode * rhs) : BinaryExpNode(p,lhs,rhs) { }
    void unparse(std::ostream& out, int indent) override;
};

class EqualsNode : public BinaryExpNode {
public:
    EqualsNode(const Position * p, ExpNode * lhs, ExpNode * rhs) : BinaryExpNode(p,lhs,rhs) { }
    void unparse(std::ostream& out, int indent) override;
};

class GreaterEqNode : public BinaryExpNode {
public:
    GreaterEqNode(const Position * p, ExpNode * lhs, ExpNode * rhs) : BinaryExpNode(p,lhs,rhs) { }
    void unparse(std::ostream& out, int indent) override;
};

class GreaterNode : public BinaryExpNode {
public:
    GreaterNode(const Position * p, ExpNode * lhs, ExpNode * rhs) : BinaryExpNode(p,lhs,rhs) { }
    void unparse(std::ostream& out, int indent) override;
};

class LessNode : public BinaryExpNode {
public:
    LessNode(const Position * p, ExpNode * lhs, ExpNode * rhs) : BinaryExpNode(p,lhs,rhs) { }
    void unparse(std::ostream& out, int indent) override;
};

class LessEqNode : public BinaryExpNode {
public:
    LessEqNode(const Position * p, ExpNode * lhs, ExpNode * rhs) : BinaryExpNode(p,lhs,rhs) { }
    void unparse(std::ostream& out, int indent) override;
};

class MinusNode : public BinaryExpNode {
public:
    MinusNode(const Position * p, ExpNode * lhs, ExpNode * rhs) : BinaryExpNode(p,lhs,rhs) { }
    void unparse(std::ostream& out, int indent) override;
};

class NotEqualsNode : public BinaryExpNode {
public:
    NotEqualsNode(const Position * p, ExpNode * lhs, ExpNode * rhs) : BinaryExpNode(p,lhs,rhs) { }
    void unparse(std::ostream& out, int indent) override;
};

class OrNode : public BinaryExpNode {
public:
    OrNode(const Position * p, ExpNode * lhs, ExpNode * rhs) : BinaryExpNode(p,lhs,rhs) { }
    void unparse(std::ostream& out, int indent) override;
};

class PlusNode : public BinaryExpNode {
public:
    PlusNode(const Position * p, ExpNode * lhs, ExpNode * rhs) : BinaryExpNode(p,lhs,rhs) { }
    void unparse(std::ostream& out, int indent) override;
};

class TimesNode : public BinaryExpNode {
public:
    TimesNode(const Position * p, ExpNode * lhs, ExpNode * rhs) : BinaryExpNode(p,lhs,rhs) { }
    void unparse(std::ostream& out, int indent) override;
};

class StmtNode : public ASTNode{
public:
	StmtNode(const Position * p) : ASTNode(p){ }
	void unparse(std::ostream& out, int indent) override = 0;
    void nestedUnparse(std::ostream& out, int indent);
};

class AssignStmtNode : public StmtNode {
public:
    AssignStmtNode(const Position * p, LocNode * destIn, ExpNode * expIn) : StmtNode(p), dest(destIn), exp(expIn) { }
    void unparse(std::ostream& out, int indent) override;
private:
    LocNode * dest;
    ExpNode * exp;
};

class CallStmtNode : public StmtNode {
public:
    CallStmtNode(const Position * p, CallExpNode * callIn) : StmtNode(p), call(callIn) { }
    void unparse(std::ostream& out, int indent) override;
private:
    CallExpNode * call;
};

class ExitStmtNode : public StmtNode {
public:
    ExitStmtNode(const Position * p) : StmtNode(p) { }
    void unparse(std::ostream& out, int indent) override;
};

class GiveStmtNode : public StmtNode {
public:
    GiveStmtNode(const Position * p, ExpNode * expIn) : StmtNode(p), exp(expIn) { }
    void unparse(std::ostream& out, int indent) override;
private:
    ExpNode * exp;
};

class IfElseStmtNode : public StmtNode {
public:
    IfElseStmtNode(const Position * p, ExpNode * conIn, std::list<StmtNode *> * trueIn, std::list<StmtNode *> * falseIn)
    : StmtNode(p), condition(conIn), trueBranch(trueIn), falseBranch(falseIn) { }
    void unparse(std::ostream& out, int indent) override;
private:
    ExpNode * condition;
    std::list<StmtNode *> * trueBranch;
    std::list<StmtNode *> * falseBranch;
};

class IfStmtNode : public StmtNode {
public:
    IfStmtNode(const Position * p, ExpNode * conIn, std::list<StmtNode *> * stmtsIn)
    : StmtNode(p), condition(conIn), stmts(stmtsIn) { }
    void unparse(std::ostream& out, int indent) override;
private:
    ExpNode * condition;
    std::list<StmtNode *> * stmts;
};

class PostDecStmtNode : public StmtNode {
public:
    PostDecStmtNode(const Position * p, LocNode * locIn) : StmtNode(p), loc(locIn) { }
    void unparse(std::ostream& out, int indent) override;
private:
    LocNode * loc;
};

class PostIncStmtNode : public StmtNode {
public:
    PostIncStmtNode(const Position * p, LocNode * locIn) : StmtNode(p), loc(locIn) { }
    void unparse(std::ostream& out, int indent) override;
private:
    LocNode * loc;
};

class ReturnStmtNode : public StmtNode {
public:
    ReturnStmtNode(const Position * p, ExpNode * expIn) : StmtNode(p), exp(expIn) { }
    void unparse(std::ostream& out, int indent) override;
private:
    ExpNode * exp;
};

class TakeStmtNode : public StmtNode {
public:
    TakeStmtNode(const Position * p, LocNode * locIn) : StmtNode(p), loc(locIn) { }
    void unparse(std::ostream& out, int indent) override;
private:
    LocNode * loc;
};

class WhileStmtNode : public StmtNode {
public:
    WhileStmtNode(const Position * p, ExpNode * expIn, std::list<StmtNode *> * stmtsIn) : StmtNode(p), exp(expIn), stmts(stmtsIn) { }
    void unparse(std::ostream& out, int indent) override;
private:
    ExpNode * exp;
    std::list <StmtNode *> * stmts;
};

/** \class DeclNode
* Superclass for declarations (i.e. nodes that can be used to 
* declare a struct, function, variable, etc).  This base class will 
**/
class DeclNode : public StmtNode{
public:
	DeclNode(const Position * p) : StmtNode(p) { }
	virtual void unparse(std::ostream& out, int indent) override = 0;
};

class ClassDeclNode : public DeclNode {
public:
    ClassDeclNode(const Position * p, IDNode * nameIn, std::list<DeclNode *> * declsIn) : DeclNode(p), name(nameIn), decls(declsIn) { }
    void unparse(std::ostream& out, int indent) override;
private:
    IDNode * name;
    std::list<DeclNode *> * decls;
};

/** A variable declaration.
**/

class VarDeclNode : public DeclNode{
public:
    VarDeclNode(const Position * p, IDNode * inID, TypeNode * inType, ExpNode * expIn = nullptr)
    : DeclNode(p), myID(inID), myType(inType), myExp(expIn){
        assert (myType != nullptr);
        assert (myID != nullptr);
    }
    void unparse(std::ostream& out, int indent);

protected:
    IDNode * myID;
    TypeNode * myType;
    ExpNode * myExp;
};

class FormalDeclNode : public VarDeclNode {
public:
    FormalDeclNode(const Position * p, IDNode * id, TypeNode * type) : VarDeclNode(p,id,type) {}
    void unparse(std::ostream& out, int indent) override;
};

class FnDeclNode : public DeclNode {
public:
    FnDeclNode(const Position * p, TypeNode * typeIn, IDNode * idIn, std::list<FormalDeclNode *> * declsIn, std::list<StmtNode *> * stmtsIn)
    : DeclNode(p), type(typeIn), id(idIn), decls(declsIn), stmts(stmtsIn) { }
    void unparse(std::ostream& out, int indent);
private:
    TypeNode * type;
    IDNode * id;
    std::list <FormalDeclNode *> * decls;
    std::list <StmtNode *> * stmts;
};

/**  \class TypeNode
* Superclass of nodes that indicate a data type. For example, in
* the declaration "int a", the int part is the type node (a is an IDNode
* and the whole thing is a DeclNode).
**/
class TypeNode : public ASTNode{
protected:
	TypeNode(const Position * p) : ASTNode(p){
	}
public:
	virtual void unparse(std::ostream& out, int indent) = 0;
};

class IntTypeNode : public TypeNode{
public:
	IntTypeNode(const Position * p) : TypeNode(p){ }
	void unparse(std::ostream& out, int indent);
};

class BoolTypeNode : public TypeNode{
public:
    BoolTypeNode(const Position * p) : TypeNode(p){ }
    void unparse(std::ostream& out, int indent);
};

class ClassTypeNode : public TypeNode{
public:
    ClassTypeNode(const Position * p, IDNode * idIn) : TypeNode(p), id(idIn) { }
    void unparse(std::ostream& out, int indent);
private:
    IDNode * id;
};

class PerfectTypeNode : public TypeNode{
public:
    PerfectTypeNode(const Position * p, TypeNode * typeIn) : TypeNode(p), type(typeIn) { }
    void unparse(std::ostream& out, int indent);
private:
    TypeNode * type;
};

class VoidTypeNode : public TypeNode{
public:
    VoidTypeNode(const Position * p) : TypeNode(p){ }
    void unparse(std::ostream& out, int indent);
};

} //End namespace drewno_mars

#endif
