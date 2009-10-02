// DO NOT EDIT!!! Generated with `./ptree-gen '
#include <opencxx/parser/ptree-generated.h>
#include <opencxx/parser/AbstractTypingWalker.h>
#include <opencxx/parser/AbstractTranslatingWalker.h>

namespace Opencxx
{

Ptree* PtreeCommaExpr::Translate(AbstractTranslatingWalker* w)
{
    return w->TranslateComma(this);
}

void PtreeCommaExpr::Typeof(AbstractTypingWalker* w, TypeInfo& t)
{
    w->TypeofComma(this, t);
}

}


namespace Opencxx
{

Ptree* PtreeAssignExpr::Translate(AbstractTranslatingWalker* w)
{
    return w->TranslateAssign(this);
}

void PtreeAssignExpr::Typeof(AbstractTypingWalker* w, TypeInfo& t)
{
    w->TypeofAssign(this, t);
}

}


namespace Opencxx
{

Ptree* PtreeCondExpr::Translate(AbstractTranslatingWalker* w)
{
    return w->TranslateCond(this);
}

void PtreeCondExpr::Typeof(AbstractTypingWalker* w, TypeInfo& t)
{
    w->TypeofCond(this, t);
}

}


namespace Opencxx
{

Ptree* PtreeInfixExpr::Translate(AbstractTranslatingWalker* w)
{
    return w->TranslateInfix(this);
}

void PtreeInfixExpr::Typeof(AbstractTypingWalker* w, TypeInfo& t)
{
    w->TypeofInfix(this, t);
}

}


namespace Opencxx
{

Ptree* PtreePmExpr::Translate(AbstractTranslatingWalker* w)
{
    return w->TranslatePm(this);
}

void PtreePmExpr::Typeof(AbstractTypingWalker* w, TypeInfo& t)
{
    w->TypeofPm(this, t);
}

}


namespace Opencxx
{

Ptree* PtreeCastExpr::Translate(AbstractTranslatingWalker* w)
{
    return w->TranslateCast(this);
}

void PtreeCastExpr::Typeof(AbstractTypingWalker* w, TypeInfo& t)
{
    w->TypeofCast(this, t);
}

}


namespace Opencxx
{

Ptree* PtreeUnaryExpr::Translate(AbstractTranslatingWalker* w)
{
    return w->TranslateUnary(this);
}

void PtreeUnaryExpr::Typeof(AbstractTypingWalker* w, TypeInfo& t)
{
    w->TypeofUnary(this, t);
}

}


namespace Opencxx
{

Ptree* PtreeThrowExpr::Translate(AbstractTranslatingWalker* w)
{
    return w->TranslateThrow(this);
}

void PtreeThrowExpr::Typeof(AbstractTypingWalker* w, TypeInfo& t)
{
    w->TypeofThrow(this, t);
}

}


namespace Opencxx
{

Ptree* PtreeSizeofExpr::Translate(AbstractTranslatingWalker* w)
{
    return w->TranslateSizeof(this);
}

void PtreeSizeofExpr::Typeof(AbstractTypingWalker* w, TypeInfo& t)
{
    w->TypeofSizeof(this, t);
}

}


namespace Opencxx
{

Ptree* PtreeTypeidExpr::Translate(AbstractTranslatingWalker* w)
{
    return w->TranslateTypeid(this);
}

void PtreeTypeidExpr::Typeof(AbstractTypingWalker* w, TypeInfo& t)
{
    w->TypeofTypeid(this, t);
}

}


namespace Opencxx
{

Ptree* PtreeTypeofExpr::Translate(AbstractTranslatingWalker* w)
{
    return w->TranslateTypeof(this);
}

void PtreeTypeofExpr::Typeof(AbstractTypingWalker* w, TypeInfo& t)
{
    w->TypeofTypeof(this, t);
}

}


namespace Opencxx
{

Ptree* PtreeNewExpr::Translate(AbstractTranslatingWalker* w)
{
    return w->TranslateNew(this);
}

void PtreeNewExpr::Typeof(AbstractTypingWalker* w, TypeInfo& t)
{
    w->TypeofNew(this, t);
}

}


namespace Opencxx
{

Ptree* PtreeDeleteExpr::Translate(AbstractTranslatingWalker* w)
{
    return w->TranslateDelete(this);
}

void PtreeDeleteExpr::Typeof(AbstractTypingWalker* w, TypeInfo& t)
{
    w->TypeofDelete(this, t);
}

}


namespace Opencxx
{

Ptree* PtreeArrayExpr::Translate(AbstractTranslatingWalker* w)
{
    return w->TranslateArray(this);
}

void PtreeArrayExpr::Typeof(AbstractTypingWalker* w, TypeInfo& t)
{
    w->TypeofArray(this, t);
}

}


namespace Opencxx
{

Ptree* PtreeFuncallExpr::Translate(AbstractTranslatingWalker* w)
{
    return w->TranslateFuncall(this);
}

void PtreeFuncallExpr::Typeof(AbstractTypingWalker* w, TypeInfo& t)
{
    w->TypeofFuncall(this, t);
}

}


namespace Opencxx
{

Ptree* PtreePostfixExpr::Translate(AbstractTranslatingWalker* w)
{
    return w->TranslatePostfix(this);
}

void PtreePostfixExpr::Typeof(AbstractTypingWalker* w, TypeInfo& t)
{
    w->TypeofPostfix(this, t);
}

}


namespace Opencxx
{

Ptree* PtreeUserStatementExpr::Translate(AbstractTranslatingWalker* w)
{
    return w->TranslateUserStatement(this);
}

void PtreeUserStatementExpr::Typeof(AbstractTypingWalker* w, TypeInfo& t)
{
    w->TypeofUserStatement(this, t);
}

}


namespace Opencxx
{

Ptree* PtreeDotMemberExpr::Translate(AbstractTranslatingWalker* w)
{
    return w->TranslateDotMember(this);
}

void PtreeDotMemberExpr::Typeof(AbstractTypingWalker* w, TypeInfo& t)
{
    w->TypeofDotMember(this, t);
}

}


namespace Opencxx
{

Ptree* PtreeArrowMemberExpr::Translate(AbstractTranslatingWalker* w)
{
    return w->TranslateArrowMember(this);
}

void PtreeArrowMemberExpr::Typeof(AbstractTypingWalker* w, TypeInfo& t)
{
    w->TypeofArrowMember(this, t);
}

}


namespace Opencxx
{

Ptree* PtreeParenExpr::Translate(AbstractTranslatingWalker* w)
{
    return w->TranslateParen(this);
}

void PtreeParenExpr::Typeof(AbstractTypingWalker* w, TypeInfo& t)
{
    w->TypeofParen(this, t);
}

}


namespace Opencxx
{

Ptree* PtreeStaticUserStatementExpr::Translate(AbstractTranslatingWalker* w)
{
    return w->TranslateStaticUserStatement(this);
}

void PtreeStaticUserStatementExpr::Typeof(AbstractTypingWalker* w, TypeInfo& t)
{
    w->TypeofStaticUserStatement(this, t);
}

}


namespace Opencxx
{

Ptree* PtreeIfStatement::Translate(AbstractTranslatingWalker* w)
{
    return w->TranslateIf(this);
}

}


namespace Opencxx
{

Ptree* PtreeSwitchStatement::Translate(AbstractTranslatingWalker* w)
{
    return w->TranslateSwitch(this);
}

}


namespace Opencxx
{

Ptree* PtreeWhileStatement::Translate(AbstractTranslatingWalker* w)
{
    return w->TranslateWhile(this);
}

}


namespace Opencxx
{

Ptree* PtreeDoStatement::Translate(AbstractTranslatingWalker* w)
{
    return w->TranslateDo(this);
}

}


namespace Opencxx
{

Ptree* PtreeForStatement::Translate(AbstractTranslatingWalker* w)
{
    return w->TranslateFor(this);
}

}


namespace Opencxx
{

Ptree* PtreeTryStatement::Translate(AbstractTranslatingWalker* w)
{
    return w->TranslateTry(this);
}

}


namespace Opencxx
{

Ptree* PtreeBreakStatement::Translate(AbstractTranslatingWalker* w)
{
    return w->TranslateBreak(this);
}

}


namespace Opencxx
{

Ptree* PtreeContinueStatement::Translate(AbstractTranslatingWalker* w)
{
    return w->TranslateContinue(this);
}

}


namespace Opencxx
{

Ptree* PtreeReturnStatement::Translate(AbstractTranslatingWalker* w)
{
    return w->TranslateReturn(this);
}

}


namespace Opencxx
{

Ptree* PtreeGotoStatement::Translate(AbstractTranslatingWalker* w)
{
    return w->TranslateGoto(this);
}

}


namespace Opencxx
{

Ptree* PtreeCaseStatement::Translate(AbstractTranslatingWalker* w)
{
    return w->TranslateCase(this);
}

}


namespace Opencxx
{

Ptree* PtreeDefaultStatement::Translate(AbstractTranslatingWalker* w)
{
    return w->TranslateDefault(this);
}

}


namespace Opencxx
{

Ptree* PtreeLabelStatement::Translate(AbstractTranslatingWalker* w)
{
    return w->TranslateLabel(this);
}

}




























