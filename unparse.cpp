#include "ast.hpp"

namespace drewno_mars{

/*
doIndent is declared static, which means that it can 
only be called in this file (its symbol is not exported).
*/
static void doIndent(std::ostream& out, int indent){
	for (int k = 0 ; k < indent; k++){ out << "\t"; }
}

/*
In this code, the intention is that functions are grouped 
into files by purpose, rather than by class.
If you're used to having all of the functions of a class 
defined in the same file, this style may be a bit disorienting,
though it is legal. Thus, we can have
ProgramNode::unparse, which is the unparse method of ProgramNodes
defined in the same file as DeclNode::unparse, the unparse method
of DeclNodes.
*/


void ProgramNode::unparse(std::ostream& out, int indent){
	/* Oh, hey it's a for-each loop in C++!
	   The loop iterates over each element in a collection
	   without that gross i++ nonsense. 
	 */
	for (auto global : *myGlobals){
		/* The auto keyword tells the compiler
		   to (try to) figure out what the
		   type of a variable should be from 
		   context. here, since we're iterating
		   over a list of DeclNode *s, it's 
		   pretty clear that global is of 
		   type DeclNode *.
		*/
		global->unparse(out, indent);
	}
}

void VarDeclNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	this->myID->unparse(out, 0);
	out << " : ";
	this->myType->unparse(out, 0);
    if (this->myExp != nullptr){
        out << " = ";
        this->myExp->unparse(out, 0);
    }
	out << ";\n";
}

void ExpNode::nestedUnparse(std::ostream &out, int indent) {
    out << "(";
    unparse(out, 0);
    out << ")";
}

void IDNode::unparse(std::ostream& out, int indent){
	out << this->name;
}

void IntTypeNode::unparse(std::ostream& out, int indent){
	out << "int";
}

void BoolTypeNode::unparse(std::ostream &out, int indent) {
    out << "bool";
}

void VoidTypeNode::unparse(std::ostream &out, int indent) {
    out << "void";
}

void IntLitNode::unparse(std::ostream &out, int indent) {
    out << this->value;
}

void StrLitNode::unparse(std::ostream &out, int indent) {
    out << this->str;
}

void TrueNode::unparse(std::ostream &out, int indent) {
    out << "true";
}

void FalseNode::unparse(std::ostream &out, int indent) {
    out << "false";
}

void MagicNode::unparse(std::ostream &out, int indent) {
    out << "24Kmagic";
}

void MemberFieldExpNode::unparse(std::ostream &out, int indent) {
    this->loc->unparse(out, 0);
    out << "--";
    this->name->unparse(out, 0);
}

void CallExpNode::unparse(std::ostream &out, int indent) {
    functionName->unparse(out, 0);
    out << "(";

    bool firstArg = true;
    for(auto arg : *args){
        if (firstArg) {
            firstArg = false;
        }
        else {
            out << ", ";
        }
        arg->unparse(out, 0);
    }
    out << ")";
}

void AndNode::unparse(std::ostream &out, int indent) {
    this->lhs->nestedUnparse(out, 0);
    out << " and ";
    this->rhs->nestedUnparse(out,0);
}

void DivideNode::unparse(std::ostream &out, int indent) {
    this->lhs->nestedUnparse(out, 0);
    out << " / ";
    this->rhs->nestedUnparse(out,0);
}

void EqualsNode::unparse(std::ostream &out, int indent) {
    this->lhs->nestedUnparse(out, 0);
    out << " == ";
    this->rhs->nestedUnparse(out,0);
}

void GreaterEqNode::unparse(std::ostream &out, int indent) {
    this->lhs->nestedUnparse(out, 0);
    out << " >= ";
    this->rhs->nestedUnparse(out,0);
}

void GreaterNode::unparse(std::ostream &out, int indent) {
    this->lhs->nestedUnparse(out, 0);
    out << " > ";
    this->rhs->nestedUnparse(out,0);
}

void LessNode::unparse(std::ostream &out, int indent) {
    this->lhs->nestedUnparse(out, 0);
    out << " < ";
    this->rhs->nestedUnparse(out,0);
}

void LessEqNode::unparse(std::ostream &out, int indent) {
    this->lhs->nestedUnparse(out, 0);
    out << " <= ";
    this->rhs->nestedUnparse(out,0);
}

void MinusNode::unparse(std::ostream &out, int indent) {
    this->lhs->nestedUnparse(out, 0);
    out << " - ";
    this->rhs->nestedUnparse(out,0);
}

void  NotEqualsNode::unparse(std::ostream &out, int indent) {
    this->lhs->nestedUnparse(out, 0);
    out << " != ";
    this->rhs->nestedUnparse(out,0);
}

void OrNode::unparse(std::ostream &out, int indent) {
    this->lhs->nestedUnparse(out, 0);
    out << " or ";
    this->rhs->nestedUnparse(out,0);
}

void PlusNode::unparse(std::ostream &out, int indent) {
    this->lhs->nestedUnparse(out, 0);
    out << " + ";
    this->rhs->nestedUnparse(out,0);
}

void TimesNode::unparse(std::ostream &out, int indent) {
    this->lhs->nestedUnparse(out, 0);
    out << " * ";
    this->rhs->nestedUnparse(out,0);
}

void NegNode::unparse(std::ostream &out, int indent) {
    out << "-";
    this->exp->nestedUnparse(out, 0);
}

void NotNode::unparse(std::ostream &out, int indent) {
    out << "!";
    this->exp->nestedUnparse(out, 0);
}

void AssignStmtNode::unparse(std::ostream &out, int indent) {
    doIndent(out, indent);
    this->dest->unparse(out, 0);
    out << " = ";
    this->exp->unparse(out, 0);
    out << ";\n";
}

void CallStmtNode::unparse(std::ostream &out, int indent) {
    doIndent(out, indent);
    this->call->unparse(out, 0);
    out << ";\n";
}

void ExitStmtNode::unparse(std::ostream &out, int indent) {
    doIndent(out, indent);
    out << "today I don't feel like doing any work;\n";
}

void GiveStmtNode::unparse(std::ostream &out, int indent) {
    doIndent(out, indent);
    out << "give ";
    this->exp->unparse(out, 0);
    out << ";\n";
}

void PostDecStmtNode::unparse(std::ostream &out, int indent) {
    doIndent(out, indent);
    this->loc->unparse(out, 0);
    out << "--;\n";
}

void PostIncStmtNode::unparse(std::ostream &out, int indent) {
    doIndent(out, indent);
    this->loc->unparse(out, 0);
    out << "++;\n";
}

void ReturnStmtNode::unparse(std::ostream &out, int indent) {
    doIndent(out, indent);
    out << "return ";
    this->exp->unparse(out, 0);
    out << ";\n";
}

void TakeStmtNode::unparse(std::ostream &out, int indent) {
    doIndent(out, indent);
    out << "take ";
    this->loc->unparse(out, 0);
    out << ";\n";
}

void IfElseStmtNode::unparse(std::ostream &out, int indent) {
    doIndent(out, indent);
    out << "if (";
    this->condition->unparse(out, 0);
    out << ") {\n";
    for (auto stmt: *trueBranch) {
        stmt->unparse(out, indent+1);
    }
    doIndent(out, indent);
    out << "} else {\n";
    for (auto stmt: *falseBranch) {
        stmt->unparse(out, indent+1);
    }
    doIndent(out, indent);
    out << "}\n";
}

void IfStmtNode::unparse(std::ostream &out, int indent) {
    doIndent(out, indent);
    out << "if (";
    this->condition->unparse(out, 0);
    out << ") {\n";
    for (auto stmt: *stmts) {
        stmt->unparse(out, indent+1);
    }
    doIndent(out, indent);
    out << "}\n";
}

void WhileStmtNode::unparse(std::ostream &out, int indent) {
    doIndent(out, indent);
    out << "while (";
    this->exp->unparse(out, 0);
    out << ") {\n";
    for (auto stmt: *stmts) {
        stmt->unparse(out, indent+1);
    }
    doIndent(out, indent);
    out << "}\n";
}

void FormalDeclNode::unparse(std::ostream &out, int indent) {
    doIndent(out, indent);
    this->myID->unparse(out, 0);
    out << " : ";
    this->myType->unparse(out, 0);
}

void FnDeclNode::unparse(std::ostream &out, int indent) {
    doIndent(out, indent);
    this->id->unparse(out, 0);
    out << " : (";

    bool firstDecl = true;
    for(auto decl : *decls){
        if (firstDecl) {
            firstDecl = false;
        }
        else {
            out << ", ";
        }
        decl->unparse(out, 0);
    }
    out << ") ";

    this->type->unparse(out, 0);
    out << " {\n";

    for (auto stmt: *stmts) {
        stmt->unparse(out, indent+1);
    }
    doIndent(out, indent);
    out << "}\n";
}

void PerfectTypeNode::unparse(std::ostream &out, int indent) {
    out << "perfect ";
    this->type->unparse(out, indent);
}

void ClassDeclNode::unparse(std::ostream &out, int indent) {
    doIndent(out, indent);
    this->name->unparse(out, indent);
    out << " : class {\n";

    for(auto decl : *decls) {
        decl->unparse(out, indent+1);
    }

    doIndent(out, indent);
    out << "};\n";
}

void ClassTypeNode::unparse(std::ostream &out, int indent) {
    this->id->unparse(out, indent);
}

void IDNode::nestedUnparse(std::ostream &out, int indent) {
    unparse(out,0);
}

void CallExpNode::nestedUnparse(std::ostream &out, int indent) {
    unparse(out,0);
}

void FalseNode::nestedUnparse(std::ostream &out, int indent) {
    unparse(out,0);
}

void TrueNode::nestedUnparse(std::ostream &out, int indent) {
    unparse(out,0);
}

void MagicNode::nestedUnparse(std::ostream &out, int indent) {
    unparse(out,0);
}

void StrLitNode::nestedUnparse(std::ostream &out, int indent) {
    unparse(out,0);
}

void IntLitNode::nestedUnparse(std::ostream &out, int indent) {
    unparse(out,0);
}

} // End namespace drewno_mars
