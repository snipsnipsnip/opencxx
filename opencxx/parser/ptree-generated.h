#ifndef included_ptree_generated_h
#define included_ptree_generated_h
#include <opencxx/parser/NonLeaf.h>
#include <opencxx/parser/LeafReserved.h>
#include <opencxx/parser/token-names.h>
// DO NOT EDIT!!! Generated with `./ptree-gen '


namespace Opencxx
{

class Ptree;
class AbstractTranslatingWalker;
class AbstractTypingWalker;
class TypeInfo;

class PtreeCommaExpr : public NonLeaf {
public:
    PtreeCommaExpr(Ptree* p, Ptree* q) : NonLeaf(p, q) {}
    int What() { return ntCommaExpr; }
    Ptree* Translate(AbstractTranslatingWalker* w);
    void Typeof(AbstractTypingWalker*, TypeInfo&);
};

}




namespace Opencxx
{

class Ptree;
class AbstractTranslatingWalker;
class AbstractTypingWalker;
class TypeInfo;

class PtreeAssignExpr : public NonLeaf {
public:
    PtreeAssignExpr(Ptree* p, Ptree* q) : NonLeaf(p, q) {}
    int What() { return ntAssignExpr; }
    Ptree* Translate(AbstractTranslatingWalker* w);
    void Typeof(AbstractTypingWalker*, TypeInfo&);
};

}




namespace Opencxx
{

class Ptree;
class AbstractTranslatingWalker;
class AbstractTypingWalker;
class TypeInfo;

class PtreeCondExpr : public NonLeaf {
public:
    PtreeCondExpr(Ptree* p, Ptree* q) : NonLeaf(p, q) {}
    int What() { return ntCondExpr; }
    Ptree* Translate(AbstractTranslatingWalker* w);
    void Typeof(AbstractTypingWalker*, TypeInfo&);
};

}




namespace Opencxx
{

class Ptree;
class AbstractTranslatingWalker;
class AbstractTypingWalker;
class TypeInfo;

class PtreeInfixExpr : public NonLeaf {
public:
    PtreeInfixExpr(Ptree* p, Ptree* q) : NonLeaf(p, q) {}
    int What() { return ntInfixExpr; }
    Ptree* Translate(AbstractTranslatingWalker* w);
    void Typeof(AbstractTypingWalker*, TypeInfo&);
};

}




namespace Opencxx
{

class Ptree;
class AbstractTranslatingWalker;
class AbstractTypingWalker;
class TypeInfo;

class PtreePmExpr : public NonLeaf {
public:
    PtreePmExpr(Ptree* p, Ptree* q) : NonLeaf(p, q) {}
    int What() { return ntPmExpr; }
    Ptree* Translate(AbstractTranslatingWalker* w);
    void Typeof(AbstractTypingWalker*, TypeInfo&);
};

}




namespace Opencxx
{

class Ptree;
class AbstractTranslatingWalker;
class AbstractTypingWalker;
class TypeInfo;

class PtreeCastExpr : public NonLeaf {
public:
    PtreeCastExpr(Ptree* p, Ptree* q) : NonLeaf(p, q) {}
    int What() { return ntCastExpr; }
    Ptree* Translate(AbstractTranslatingWalker* w);
    void Typeof(AbstractTypingWalker*, TypeInfo&);
};

}




namespace Opencxx
{

class Ptree;
class AbstractTranslatingWalker;
class AbstractTypingWalker;
class TypeInfo;

class PtreeUnaryExpr : public NonLeaf {
public:
    PtreeUnaryExpr(Ptree* p, Ptree* q) : NonLeaf(p, q) {}
    int What() { return ntUnaryExpr; }
    Ptree* Translate(AbstractTranslatingWalker* w);
    void Typeof(AbstractTypingWalker*, TypeInfo&);
};

}




namespace Opencxx
{

class Ptree;
class AbstractTranslatingWalker;
class AbstractTypingWalker;
class TypeInfo;

class PtreeThrowExpr : public NonLeaf {
public:
    PtreeThrowExpr(Ptree* p, Ptree* q) : NonLeaf(p, q) {}
    int What() { return ntThrowExpr; }
    Ptree* Translate(AbstractTranslatingWalker* w);
    void Typeof(AbstractTypingWalker*, TypeInfo&);
};

}




namespace Opencxx
{

class Ptree;
class AbstractTranslatingWalker;
class AbstractTypingWalker;
class TypeInfo;

class PtreeSizeofExpr : public NonLeaf {
public:
    PtreeSizeofExpr(Ptree* p, Ptree* q) : NonLeaf(p, q) {}
    int What() { return ntSizeofExpr; }
    Ptree* Translate(AbstractTranslatingWalker* w);
    void Typeof(AbstractTypingWalker*, TypeInfo&);
};

}




namespace Opencxx
{

class Ptree;
class AbstractTranslatingWalker;
class AbstractTypingWalker;
class TypeInfo;

class PtreeTypeidExpr : public NonLeaf {
public:
    PtreeTypeidExpr(Ptree* p, Ptree* q) : NonLeaf(p, q) {}
    int What() { return ntTypeidExpr; }
    Ptree* Translate(AbstractTranslatingWalker* w);
    void Typeof(AbstractTypingWalker*, TypeInfo&);
};

}




namespace Opencxx
{

class Ptree;
class AbstractTranslatingWalker;
class AbstractTypingWalker;
class TypeInfo;

class PtreeTypeofExpr : public NonLeaf {
public:
    PtreeTypeofExpr(Ptree* p, Ptree* q) : NonLeaf(p, q) {}
    int What() { return ntTypeofExpr; }
    Ptree* Translate(AbstractTranslatingWalker* w);
    void Typeof(AbstractTypingWalker*, TypeInfo&);
};

}




namespace Opencxx
{

class Ptree;
class AbstractTranslatingWalker;
class AbstractTypingWalker;
class TypeInfo;

class PtreeNewExpr : public NonLeaf {
public:
    PtreeNewExpr(Ptree* p, Ptree* q) : NonLeaf(p, q) {}
    int What() { return ntNewExpr; }
    Ptree* Translate(AbstractTranslatingWalker* w);
    void Typeof(AbstractTypingWalker*, TypeInfo&);
};

}




namespace Opencxx
{

class Ptree;
class AbstractTranslatingWalker;
class AbstractTypingWalker;
class TypeInfo;

class PtreeDeleteExpr : public NonLeaf {
public:
    PtreeDeleteExpr(Ptree* p, Ptree* q) : NonLeaf(p, q) {}
    int What() { return ntDeleteExpr; }
    Ptree* Translate(AbstractTranslatingWalker* w);
    void Typeof(AbstractTypingWalker*, TypeInfo&);
};

}




namespace Opencxx
{

class Ptree;
class AbstractTranslatingWalker;
class AbstractTypingWalker;
class TypeInfo;

class PtreeArrayExpr : public NonLeaf {
public:
    PtreeArrayExpr(Ptree* p, Ptree* q) : NonLeaf(p, q) {}
    int What() { return ntArrayExpr; }
    Ptree* Translate(AbstractTranslatingWalker* w);
    void Typeof(AbstractTypingWalker*, TypeInfo&);
};

}




namespace Opencxx
{

class Ptree;
class AbstractTranslatingWalker;
class AbstractTypingWalker;
class TypeInfo;

class PtreeFuncallExpr : public NonLeaf {
public:
    PtreeFuncallExpr(Ptree* p, Ptree* q) : NonLeaf(p, q) {}
    int What() { return ntFuncallExpr; }
    Ptree* Translate(AbstractTranslatingWalker* w);
    void Typeof(AbstractTypingWalker*, TypeInfo&);
};

}




namespace Opencxx
{

class Ptree;
class AbstractTranslatingWalker;
class AbstractTypingWalker;
class TypeInfo;

class PtreePostfixExpr : public NonLeaf {
public:
    PtreePostfixExpr(Ptree* p, Ptree* q) : NonLeaf(p, q) {}
    int What() { return ntPostfixExpr; }
    Ptree* Translate(AbstractTranslatingWalker* w);
    void Typeof(AbstractTypingWalker*, TypeInfo&);
};

}




namespace Opencxx
{

class Ptree;
class AbstractTranslatingWalker;
class AbstractTypingWalker;
class TypeInfo;

class PtreeUserStatementExpr : public NonLeaf {
public:
    PtreeUserStatementExpr(Ptree* p, Ptree* q) : NonLeaf(p, q) {}
    int What() { return ntUserStatementExpr; }
    Ptree* Translate(AbstractTranslatingWalker* w);
    void Typeof(AbstractTypingWalker*, TypeInfo&);
};

}




namespace Opencxx
{

class Ptree;
class AbstractTranslatingWalker;
class AbstractTypingWalker;
class TypeInfo;

class PtreeDotMemberExpr : public NonLeaf {
public:
    PtreeDotMemberExpr(Ptree* p, Ptree* q) : NonLeaf(p, q) {}
    int What() { return ntDotMemberExpr; }
    Ptree* Translate(AbstractTranslatingWalker* w);
    void Typeof(AbstractTypingWalker*, TypeInfo&);
};

}




namespace Opencxx
{

class Ptree;
class AbstractTranslatingWalker;
class AbstractTypingWalker;
class TypeInfo;

class PtreeArrowMemberExpr : public NonLeaf {
public:
    PtreeArrowMemberExpr(Ptree* p, Ptree* q) : NonLeaf(p, q) {}
    int What() { return ntArrowMemberExpr; }
    Ptree* Translate(AbstractTranslatingWalker* w);
    void Typeof(AbstractTypingWalker*, TypeInfo&);
};

}




namespace Opencxx
{

class Ptree;
class AbstractTranslatingWalker;
class AbstractTypingWalker;
class TypeInfo;

class PtreeParenExpr : public NonLeaf {
public:
    PtreeParenExpr(Ptree* p, Ptree* q) : NonLeaf(p, q) {}
    int What() { return ntParenExpr; }
    Ptree* Translate(AbstractTranslatingWalker* w);
    void Typeof(AbstractTypingWalker*, TypeInfo&);
};

}




namespace Opencxx
{

class Ptree;
class AbstractTranslatingWalker;
class AbstractTypingWalker;
class TypeInfo;

class PtreeStaticUserStatementExpr : public NonLeaf {
public:
    PtreeStaticUserStatementExpr(Ptree* p, Ptree* q) : NonLeaf(p, q) {}
    int What() { return ntStaticUserStatementExpr; }
    Ptree* Translate(AbstractTranslatingWalker* w);
    void Typeof(AbstractTypingWalker*, TypeInfo&);
};

}




namespace Opencxx
{

class Ptree;
class AbstractTranslatingWalker;

class PtreeIfStatement : public NonLeaf {
public:
    PtreeIfStatement(Ptree* p, Ptree* q) : NonLeaf(p, q) {}
    int What() { return ntIfStatement; }
    Ptree* Translate(AbstractTranslatingWalker* w);
};

}



namespace Opencxx
{

class Ptree;
class AbstractTranslatingWalker;

class PtreeSwitchStatement : public NonLeaf {
public:
    PtreeSwitchStatement(Ptree* p, Ptree* q) : NonLeaf(p, q) {}
    int What() { return ntSwitchStatement; }
    Ptree* Translate(AbstractTranslatingWalker* w);
};

}



namespace Opencxx
{

class Ptree;
class AbstractTranslatingWalker;

class PtreeWhileStatement : public NonLeaf {
public:
    PtreeWhileStatement(Ptree* p, Ptree* q) : NonLeaf(p, q) {}
    int What() { return ntWhileStatement; }
    Ptree* Translate(AbstractTranslatingWalker* w);
};

}



namespace Opencxx
{

class Ptree;
class AbstractTranslatingWalker;

class PtreeDoStatement : public NonLeaf {
public:
    PtreeDoStatement(Ptree* p, Ptree* q) : NonLeaf(p, q) {}
    int What() { return ntDoStatement; }
    Ptree* Translate(AbstractTranslatingWalker* w);
};

}



namespace Opencxx
{

class Ptree;
class AbstractTranslatingWalker;

class PtreeForStatement : public NonLeaf {
public:
    PtreeForStatement(Ptree* p, Ptree* q) : NonLeaf(p, q) {}
    int What() { return ntForStatement; }
    Ptree* Translate(AbstractTranslatingWalker* w);
};

}



namespace Opencxx
{

class Ptree;
class AbstractTranslatingWalker;

class PtreeTryStatement : public NonLeaf {
public:
    PtreeTryStatement(Ptree* p, Ptree* q) : NonLeaf(p, q) {}
    int What() { return ntTryStatement; }
    Ptree* Translate(AbstractTranslatingWalker* w);
};

}



namespace Opencxx
{

class Ptree;
class AbstractTranslatingWalker;

class PtreeBreakStatement : public NonLeaf {
public:
    PtreeBreakStatement(Ptree* p, Ptree* q) : NonLeaf(p, q) {}
    int What() { return ntBreakStatement; }
    Ptree* Translate(AbstractTranslatingWalker* w);
};

}



namespace Opencxx
{

class Ptree;
class AbstractTranslatingWalker;

class PtreeContinueStatement : public NonLeaf {
public:
    PtreeContinueStatement(Ptree* p, Ptree* q) : NonLeaf(p, q) {}
    int What() { return ntContinueStatement; }
    Ptree* Translate(AbstractTranslatingWalker* w);
};

}



namespace Opencxx
{

class Ptree;
class AbstractTranslatingWalker;

class PtreeReturnStatement : public NonLeaf {
public:
    PtreeReturnStatement(Ptree* p, Ptree* q) : NonLeaf(p, q) {}
    int What() { return ntReturnStatement; }
    Ptree* Translate(AbstractTranslatingWalker* w);
};

}



namespace Opencxx
{

class Ptree;
class AbstractTranslatingWalker;

class PtreeGotoStatement : public NonLeaf {
public:
    PtreeGotoStatement(Ptree* p, Ptree* q) : NonLeaf(p, q) {}
    int What() { return ntGotoStatement; }
    Ptree* Translate(AbstractTranslatingWalker* w);
};

}



namespace Opencxx
{

class Ptree;
class AbstractTranslatingWalker;

class PtreeCaseStatement : public NonLeaf {
public:
    PtreeCaseStatement(Ptree* p, Ptree* q) : NonLeaf(p, q) {}
    int What() { return ntCaseStatement; }
    Ptree* Translate(AbstractTranslatingWalker* w);
};

}



namespace Opencxx
{

class Ptree;
class AbstractTranslatingWalker;

class PtreeDefaultStatement : public NonLeaf {
public:
    PtreeDefaultStatement(Ptree* p, Ptree* q) : NonLeaf(p, q) {}
    int What() { return ntDefaultStatement; }
    Ptree* Translate(AbstractTranslatingWalker* w);
};

}



namespace Opencxx
{

class Ptree;
class AbstractTranslatingWalker;

class PtreeLabelStatement : public NonLeaf {
public:
    PtreeLabelStatement(Ptree* p, Ptree* q) : NonLeaf(p, q) {}
    int What() { return ntLabelStatement; }
    Ptree* Translate(AbstractTranslatingWalker* w);
};

}



namespace Opencxx
{

class Token;

class LeafAUTO : public LeafReserved {
public:
    LeafAUTO(Token& t) : LeafReserved(t) {}
    LeafAUTO(char* str, int len) : LeafReserved(str, len) {}
    int What() { return AUTO; }
};

}



namespace Opencxx
{

class Token;

class LeafBOOLEAN : public LeafReserved {
public:
    LeafBOOLEAN(Token& t) : LeafReserved(t) {}
    LeafBOOLEAN(char* str, int len) : LeafReserved(str, len) {}
    int What() { return BOOLEAN; }
};

}



namespace Opencxx
{

class Token;

class LeafCHAR : public LeafReserved {
public:
    LeafCHAR(Token& t) : LeafReserved(t) {}
    LeafCHAR(char* str, int len) : LeafReserved(str, len) {}
    int What() { return CHAR; }
};

}



namespace Opencxx
{

class Token;

class LeafWCHAR : public LeafReserved {
public:
    LeafWCHAR(Token& t) : LeafReserved(t) {}
    LeafWCHAR(char* str, int len) : LeafReserved(str, len) {}
    int What() { return WCHAR; }
};

}



namespace Opencxx
{

class Token;

class LeafCONST : public LeafReserved {
public:
    LeafCONST(Token& t) : LeafReserved(t) {}
    LeafCONST(char* str, int len) : LeafReserved(str, len) {}
    int What() { return CONST; }
};

}



namespace Opencxx
{

class Token;

class LeafDOUBLE : public LeafReserved {
public:
    LeafDOUBLE(Token& t) : LeafReserved(t) {}
    LeafDOUBLE(char* str, int len) : LeafReserved(str, len) {}
    int What() { return DOUBLE; }
};

}



namespace Opencxx
{

class Token;

class LeafEXTERN : public LeafReserved {
public:
    LeafEXTERN(Token& t) : LeafReserved(t) {}
    LeafEXTERN(char* str, int len) : LeafReserved(str, len) {}
    int What() { return EXTERN; }
};

}



namespace Opencxx
{

class Token;

class LeafFLOAT : public LeafReserved {
public:
    LeafFLOAT(Token& t) : LeafReserved(t) {}
    LeafFLOAT(char* str, int len) : LeafReserved(str, len) {}
    int What() { return FLOAT; }
};

}



namespace Opencxx
{

class Token;

class LeafFRIEND : public LeafReserved {
public:
    LeafFRIEND(Token& t) : LeafReserved(t) {}
    LeafFRIEND(char* str, int len) : LeafReserved(str, len) {}
    int What() { return FRIEND; }
};

}



namespace Opencxx
{

class Token;

class LeafINLINE : public LeafReserved {
public:
    LeafINLINE(Token& t) : LeafReserved(t) {}
    LeafINLINE(char* str, int len) : LeafReserved(str, len) {}
    int What() { return INLINE; }
};

}



namespace Opencxx
{

class Token;

class LeafINT : public LeafReserved {
public:
    LeafINT(Token& t) : LeafReserved(t) {}
    LeafINT(char* str, int len) : LeafReserved(str, len) {}
    int What() { return INT; }
};

}



namespace Opencxx
{

class Token;

class LeafLONG : public LeafReserved {
public:
    LeafLONG(Token& t) : LeafReserved(t) {}
    LeafLONG(char* str, int len) : LeafReserved(str, len) {}
    int What() { return LONG; }
};

}



namespace Opencxx
{

class Token;

class LeafMUTABLE : public LeafReserved {
public:
    LeafMUTABLE(Token& t) : LeafReserved(t) {}
    LeafMUTABLE(char* str, int len) : LeafReserved(str, len) {}
    int What() { return MUTABLE; }
};

}



namespace Opencxx
{

class Token;

class LeafNAMESPACE : public LeafReserved {
public:
    LeafNAMESPACE(Token& t) : LeafReserved(t) {}
    LeafNAMESPACE(char* str, int len) : LeafReserved(str, len) {}
    int What() { return NAMESPACE; }
};

}



namespace Opencxx
{

class Token;

class LeafPRIVATE : public LeafReserved {
public:
    LeafPRIVATE(Token& t) : LeafReserved(t) {}
    LeafPRIVATE(char* str, int len) : LeafReserved(str, len) {}
    int What() { return PRIVATE; }
};

}



namespace Opencxx
{

class Token;

class LeafPROTECTED : public LeafReserved {
public:
    LeafPROTECTED(Token& t) : LeafReserved(t) {}
    LeafPROTECTED(char* str, int len) : LeafReserved(str, len) {}
    int What() { return PROTECTED; }
};

}



namespace Opencxx
{

class Token;

class LeafPUBLIC : public LeafReserved {
public:
    LeafPUBLIC(Token& t) : LeafReserved(t) {}
    LeafPUBLIC(char* str, int len) : LeafReserved(str, len) {}
    int What() { return PUBLIC; }
};

}



namespace Opencxx
{

class Token;

class LeafREGISTER : public LeafReserved {
public:
    LeafREGISTER(Token& t) : LeafReserved(t) {}
    LeafREGISTER(char* str, int len) : LeafReserved(str, len) {}
    int What() { return REGISTER; }
};

}



namespace Opencxx
{

class Token;

class LeafSHORT : public LeafReserved {
public:
    LeafSHORT(Token& t) : LeafReserved(t) {}
    LeafSHORT(char* str, int len) : LeafReserved(str, len) {}
    int What() { return SHORT; }
};

}



namespace Opencxx
{

class Token;

class LeafSIGNED : public LeafReserved {
public:
    LeafSIGNED(Token& t) : LeafReserved(t) {}
    LeafSIGNED(char* str, int len) : LeafReserved(str, len) {}
    int What() { return SIGNED; }
};

}



namespace Opencxx
{

class Token;

class LeafSTATIC : public LeafReserved {
public:
    LeafSTATIC(Token& t) : LeafReserved(t) {}
    LeafSTATIC(char* str, int len) : LeafReserved(str, len) {}
    int What() { return STATIC; }
};

}



namespace Opencxx
{

class Token;

class LeafUNSIGNED : public LeafReserved {
public:
    LeafUNSIGNED(Token& t) : LeafReserved(t) {}
    LeafUNSIGNED(char* str, int len) : LeafReserved(str, len) {}
    int What() { return UNSIGNED; }
};

}



namespace Opencxx
{

class Token;

class LeafUSING : public LeafReserved {
public:
    LeafUSING(Token& t) : LeafReserved(t) {}
    LeafUSING(char* str, int len) : LeafReserved(str, len) {}
    int What() { return USING; }
};

}



namespace Opencxx
{

class Token;

class LeafVIRTUAL : public LeafReserved {
public:
    LeafVIRTUAL(Token& t) : LeafReserved(t) {}
    LeafVIRTUAL(char* str, int len) : LeafReserved(str, len) {}
    int What() { return VIRTUAL; }
};

}



namespace Opencxx
{

class Token;

class LeafVOID : public LeafReserved {
public:
    LeafVOID(Token& t) : LeafReserved(t) {}
    LeafVOID(char* str, int len) : LeafReserved(str, len) {}
    int What() { return VOID; }
};

}



namespace Opencxx
{

class Token;

class LeafVOLATILE : public LeafReserved {
public:
    LeafVOLATILE(Token& t) : LeafReserved(t) {}
    LeafVOLATILE(char* str, int len) : LeafReserved(str, len) {}
    int What() { return VOLATILE; }
};

}



namespace Opencxx
{

class Token;

class LeafUserKeyword2 : public LeafReserved {
public:
    LeafUserKeyword2(Token& t) : LeafReserved(t) {}
    LeafUserKeyword2(char* str, int len) : LeafReserved(str, len) {}
    int What() { return UserKeyword2; }
};

}

#endif /* ! defined(included_ptree_generated_h) */
