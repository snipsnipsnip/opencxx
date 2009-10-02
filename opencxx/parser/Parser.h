#ifndef guard_opencxx_Parser_h
#define guard_opencxx_Parser_h

//@beginlicenses@
//@license{chiba_tokyo}{}@
//@license{contributors}{}@
//
//  Permission to use, copy, distribute and modify this software and its  
//  documentation for any purpose is hereby granted without fee, provided that
//  the above copyright notice appears in all copies and that both that copyright
//  notice and this permission notice appear in supporting documentation.
// 
//  1997-2001 Shigeru Chiba, Tokyo Institute of Technology. make(s) no representations about the suitability of this
//  software for any purpose. It is provided "as is" without express or implied
//  warranty.
//  
//  Copyright (C)  1997-2001 Shigeru Chiba, Tokyo Institute of Technology.
//
//  -----------------------------------------------------------------
//
//  Permission to use, copy, distribute and modify this software and its  
//  documentation for any purpose is hereby granted without fee, provided that
//  the above copyright notice appears in all copies and that both that copyright
//  notice and this permission notice appear in supporting documentation.
// 
//  Other Contributors (see file AUTHORS) make(s) no representations about the suitability of this
//  software for any purpose. It is provided "as is" without express or implied
//  warranty.
//  
//  Copyright (C)  Other Contributors (see file AUTHORS)
//
//@endlicenses@

#include <opencxx/parser/GC.h>
#include <opencxx/parser/SourceLocation.h>
#include <opencxx/parser/MetaclassLoader.h>

namespace Opencxx
{

class Lex;
class Token;
class Environment;
class Ptree;
class Encoding;
class ErrorLog;

/*
  Naming conventions for member functions

  r<name>:   grammar rule (terminal or non-terminal)
  opt<name>: optional terminal/non-termianl symbol
  is<name>:  is the following symbol <name>?
*/

class Parser : public Object {
public:
    Parser(Lex* lex, ErrorLog& errorLog);
    unsigned LineNumber(char* pos, char*& fname, int& fname_len);

    bool rProgram(Ptree*&);

    enum DeclKind { kDeclarator, kArgDeclarator, kCastDeclarator };
    enum TemplateDeclKind { tdk_unknown, tdk_decl, tdk_instantiation, 
			    tdk_specialization, num_tdks };

    bool rDefinition(Ptree*&);
    bool rNullDeclaration(Ptree*&);
    bool rTypedef(Ptree*&);
    bool rTypeSpecifier(Ptree*&, bool, Encoding&);
    bool isTypeSpecifier();
    bool rMetaclassDecl(Ptree*&);
    bool rMetaArguments(Ptree*&);
    bool rLinkageSpec(Ptree*&);
    bool rNamespaceSpec(Ptree*&);
    bool rNamespaceAlias(Ptree*&);
    bool rUsing(Ptree*&);
    bool rLinkageBody(Ptree*&);
    bool rTemplateDecl(Ptree*&);
    bool rTemplateDecl2(Ptree*&, TemplateDeclKind &kind);
    bool rTempArgList(Ptree*&);
    bool rTempArgDeclaration(Ptree*&);
    bool rExternTemplateDecl(Ptree*&);

    bool rDeclaration(Ptree*&);
    bool rIntegralDeclaration(Ptree*&, Encoding&, Ptree*, Ptree*, Ptree*);
    bool rConstDeclaration(Ptree*&, Encoding&, Ptree*, Ptree*);
    bool rOtherDeclaration(Ptree*&, Encoding&, Ptree*, Ptree*, Ptree*);
    bool rCondition(Ptree*&);
    bool rSimpleDeclaration(Ptree*&);

    bool isConstructorDecl();
    bool isPtrToMember(int);
    bool optMemberSpec(Ptree*&);
    bool optStorageSpec(Ptree*&);
    bool optCvQualify(Ptree*&);
    bool optIntegralTypeOrClassSpec(Ptree*&, Encoding&);
    bool rConstructorDecl(Ptree*&, Encoding&);
    bool optThrowDecl(Ptree*&);

    bool rDeclarators(Ptree*&, Encoding&, bool, bool = false);
    bool rDeclaratorWithInit(Ptree*&, Encoding&, bool, bool);
    bool rDeclarator(Ptree*&, DeclKind, bool, Encoding&, Encoding&, bool,
		     bool = false);
    bool rDeclarator2(Ptree*&, DeclKind, bool, Encoding&, Encoding&, bool,
		      bool, Ptree**);
    bool optPtrOperator(Ptree*&, Encoding&);
    bool rMemberInitializers(Ptree*&);
    bool rMemberInit(Ptree*&);

    bool rName(Ptree*&, Encoding&);
    bool rOperatorName(Ptree*&, Encoding&);
    bool rCastOperatorName(Ptree*&, Encoding&);
    bool rPtrToMember(Ptree*&, Encoding&);
    bool rTemplateArgs(Ptree*&, Encoding&);

    bool rArgDeclListOrInit(Ptree*&, bool&, Encoding&, bool);
    bool rArgDeclList(Ptree*&, Encoding&);
    bool rArgDeclaration(Ptree*&, Encoding&);

    bool rFunctionArguments(Ptree*&);
    bool rInitializeExpr(Ptree*&);

    bool rEnumSpec(Ptree*&, Encoding&);
    bool rEnumBody(Ptree*&);
    bool rClassSpec(Ptree*&, Encoding&);
    bool rBaseSpecifiers(Ptree*&);
    bool rClassBody(Ptree*&);
    bool rClassMember(Ptree*&);
    bool rAccessDecl(Ptree*&);
    bool rUserAccessSpec(Ptree*&);

    bool rCommaExpression(Ptree*&);

    bool rExpression(Ptree*&);
    bool rConditionalExpr(Ptree*&);
    bool rLogicalOrExpr(Ptree*&, bool);
    bool rLogicalAndExpr(Ptree*&, bool);
    bool rInclusiveOrExpr(Ptree*&, bool);
    bool rExclusiveOrExpr(Ptree*&, bool);
    bool rAndExpr(Ptree*&, bool);
    bool rEqualityExpr(Ptree*&, bool);
    bool rRelationalExpr(Ptree*&, bool);
    bool rShiftExpr(Ptree*&);
    bool rAdditiveExpr(Ptree*&);
    bool rMultiplyExpr(Ptree*&);
    bool rPmExpr(Ptree*&);
    bool rCastExpr(Ptree*&);
    bool rTypeName(Ptree*&);
    bool rTypeName(Ptree*&, Encoding&);
    bool rUnaryExpr(Ptree*&);
    bool rThrowExpr(Ptree*&);
    bool rSizeofExpr(Ptree*&);
    bool rTypeidExpr(Ptree*&);
    bool isAllocateExpr(int);
    bool rAllocateExpr(Ptree*&);
    bool rUserdefKeyword(Ptree*&);
    bool rAllocateType(Ptree*&);
    bool rNewDeclarator(Ptree*&, Encoding&);
    bool rAllocateInitializer(Ptree*&);
    bool rPostfixExpr(Ptree*&);
    bool rPrimaryExpr(Ptree*&);
    bool rUserdefStatement(Ptree*&);
    bool rVarName(Ptree*&);
    bool rVarNameCore(Ptree*&, Encoding&);
    bool isTemplateArgs();

    bool rFunctionBody(Ptree*&);
    bool rCompoundStatement(Ptree*&);
    bool rStatement(Ptree*&);
    bool rIfStatement(Ptree*&);
    bool rSwitchStatement(Ptree*&);
    bool rWhileStatement(Ptree*&);
    bool rDoStatement(Ptree*&);
    bool rForStatement(Ptree*&);
    bool rTryStatement(Ptree*&);

    bool rExprStatement(Ptree*&);
    bool rDeclarationStatement(Ptree*&);
    bool rIntegralDeclStatement(Ptree*&, Encoding&, Ptree*, Ptree*, Ptree*);
    bool rOtherDeclStatement(Ptree*&, Encoding&, Ptree*, Ptree*);

    bool MaybeTypeNameOrClassTemplate(Token&);
    void SkipTo(int token);
    SourceLocation GetLocation(Ptree* where);
    
    void InstallMetaclassLoader(MetaclassLoader* loader)
    {
        metaclassLoader_ = loader;
    }
    
    bool SyntaxErrorsFound() const { return syntaxErrors_; }

private:
    bool moreVarName();
    bool SyntaxError();
    void ShowMessageHead(char*);

private:
    Lex* lex;
    MetaclassLoader* metaclassLoader_; // :TODO: make it an auto_ptr
    ErrorLog& errorLog_;
    bool syntaxErrors_;
};

}


#endif /* ! guard_opencxx_Parser_h */
