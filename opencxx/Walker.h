#ifndef guard_opencxx_Walker_h
#define guard_opencxx_Walker_h

//@beginlicenses@
//@license{chiba-tokyo}{}@
//@license{contributors}{}@
//
//  Copyright (C) 1997-2001 Shigeru Chiba, Tokyo Institute of Technology.
//
//  Permission to use, copy, distribute and modify this software and
//  its documentation for any purpose is hereby granted without fee,
//  provided that the above copyright notice appears in all copies and that
//  both that copyright notice and this permission notice appear in
//  supporting documentation.
//
//  Shigeru Chiba makes no representations about the suitability of this
//  software for any purpose.  It is provided "as is" without express or
//  implied warranty.
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
#include <opencxx/parser/deprecated.h>
#include <opencxx/parser/AbstractTranslatingWalker.h>
#include <opencxx/parser/AbstractTypingWalker.h>

namespace Opencxx
{

class Environment;
class TypeInfo;
class Class;
class Parser;
class Ptree;
class PtreeDeclarator;
class Parser;

class Walker 
  : public LightObject 
  , public AbstractTypingWalker
  , public AbstractTranslatingWalker
{
public:
    Walker(Parser*);
    Walker(Parser*, Environment*);
    Walker(Environment*);
    Walker(Walker*);

    Ptree* Translate(Ptree*);
    void Typeof(Ptree*, TypeInfo&);

    virtual bool IsClassWalker();

    // default translation
    virtual Ptree* TranslatePtree(Ptree*);
    virtual void TypeofPtree(Ptree*, TypeInfo&);

    virtual Ptree* TranslateTypedef(Ptree*);
    virtual Ptree* TranslateTemplateDecl(Ptree*);
    virtual Ptree* TranslateTemplateInstantiation(Ptree*);
    virtual Ptree* TranslateTemplateInstantiation(Ptree*, Ptree*,
						  Ptree*, Class*);
    virtual Class* MakeTemplateInstantiationMetaobject(
	Ptree* full_class_spec, Ptree* userkey, Ptree* class_spec);
    virtual Ptree* TranslateExternTemplate(Ptree*);
    virtual Ptree* TranslateTemplateClass(Ptree*, Ptree*);
    virtual Class* MakeTemplateClassMetaobject(Ptree*, Ptree*, Ptree*);
    virtual Ptree* TranslateTemplateFunction(Ptree*, Ptree*);
    virtual Ptree* TranslateMetaclassDecl(Ptree*);
    virtual Ptree* TranslateLinkageSpec(Ptree*);
    virtual Ptree* TranslateNamespaceSpec(Ptree*);
    virtual Ptree* TranslateNamespaceAlias(Ptree*);
    virtual Ptree* TranslateUsing(Ptree*);
    virtual Ptree* TranslateDeclaration(Ptree*);
    virtual Ptree* TranslateStorageSpecifiers(Ptree*);
    virtual Ptree* TranslateDeclarators(Ptree*);
    virtual Ptree* TranslateDeclarator(bool, PtreeDeclarator*);
    virtual Ptree* TranslateArgDeclList(bool, Ptree*, Ptree*);
    static Ptree* TranslateArgDeclList2(bool, Environment*, bool, bool, int,
					Ptree*);
    virtual Ptree* TranslateInitializeArgs(PtreeDeclarator*, Ptree*);
    virtual Ptree* TranslateAssignInitializer(PtreeDeclarator*, Ptree*);
    virtual Ptree* TranslateFunctionImplementation(Ptree*);
    virtual Ptree* RecordArgsAndTranslateFbody(Class*, Ptree* args,
					       Ptree* body);
    virtual Ptree* TranslateFunctionBody(Ptree*);
    virtual Ptree* TranslateBrace(Ptree*);
    virtual Ptree* TranslateBlock(Ptree*);
    virtual Ptree* TranslateClassBody(Ptree*, Ptree*, Class*);

    virtual Ptree* TranslateClassSpec(Ptree*);
    virtual Class* MakeClassMetaobject(Ptree*, Ptree*, Ptree*);
    virtual Ptree* TranslateClassSpec(Ptree*, Ptree*, Ptree*, Class*);
    virtual Ptree* TranslateEnumSpec(Ptree*);

    virtual Ptree* TranslateAccessSpec(Ptree*);
    virtual Ptree* TranslateAccessDecl(Ptree*);
    virtual Ptree* TranslateUserAccessSpec(Ptree*);

    virtual Ptree* TranslateIf(Ptree*);
    virtual Ptree* TranslateSwitch(Ptree*);
    virtual Ptree* TranslateWhile(Ptree*);
    virtual Ptree* TranslateDo(Ptree*);
    virtual Ptree* TranslateFor(Ptree*);
    virtual Ptree* TranslateTry(Ptree*);
    virtual Ptree* TranslateBreak(Ptree*);
    virtual Ptree* TranslateContinue(Ptree*);
    virtual Ptree* TranslateReturn(Ptree*);
    virtual Ptree* TranslateGoto(Ptree*);
    virtual Ptree* TranslateCase(Ptree*);
    virtual Ptree* TranslateDefault(Ptree*);
    virtual Ptree* TranslateLabel(Ptree*);
    virtual Ptree* TranslateExprStatement(Ptree*);

    virtual Ptree* TranslateTypespecifier(Ptree*);

    virtual Ptree* TranslateComma(Ptree*);
    virtual Ptree* TranslateAssign(Ptree*);
    virtual Ptree* TranslateCond(Ptree*);
    virtual Ptree* TranslateInfix(Ptree*);
    virtual Ptree* TranslatePm(Ptree*);
    virtual Ptree* TranslateCast(Ptree*);
    virtual Ptree* TranslateUnary(Ptree*);
    virtual Ptree* TranslateThrow(Ptree*);
    virtual Ptree* TranslateSizeof(Ptree*);
    virtual Ptree* TranslateTypeid(Ptree*);
    virtual Ptree* TranslateTypeof(Ptree*);
    virtual Ptree* TranslateNew(Ptree*);
    virtual Ptree* TranslateNew2(Ptree*, Ptree*, Ptree*, Ptree*,
				Ptree*, Ptree*, Ptree*);
    virtual Ptree* TranslateNew3(Ptree* type);
    virtual Ptree* TranslateDelete(Ptree*);
    virtual Ptree* TranslateThis(Ptree*);
    virtual Ptree* TranslateVariable(Ptree*);
    virtual Ptree* TranslateFstyleCast(Ptree*);
    virtual Ptree* TranslateArray(Ptree*);
    virtual Ptree* TranslateFuncall(Ptree*);	// and fstyle cast
    virtual Ptree* TranslatePostfix(Ptree*);
    virtual Ptree* TranslateUserStatement(Ptree*);
    virtual Ptree* TranslateDotMember(Ptree*);
    virtual Ptree* TranslateArrowMember(Ptree*);
    virtual Ptree* TranslateParen(Ptree*);
    virtual Ptree* TranslateStaticUserStatement(Ptree*);

    virtual void TypeofComma(Ptree*, TypeInfo&);
    virtual void TypeofAssign(Ptree*, TypeInfo&);
    virtual void TypeofCond(Ptree*, TypeInfo&);
    virtual void TypeofInfix(Ptree*, TypeInfo&);
    virtual void TypeofPm(Ptree*, TypeInfo&);
    virtual void TypeofCast(Ptree*, TypeInfo&);
    virtual void TypeofUnary(Ptree*, TypeInfo&);
    virtual void TypeofThrow(Ptree*, TypeInfo&);
    virtual void TypeofSizeof(Ptree*, TypeInfo&);
    virtual void TypeofTypeid(Ptree*, TypeInfo&);
    virtual void TypeofTypeof(Ptree*, TypeInfo&);
    virtual void TypeofNew(Ptree*, TypeInfo&);
    virtual void TypeofDelete(Ptree*, TypeInfo&);
    virtual void TypeofThis(Ptree*, TypeInfo&);
    virtual void TypeofVariable(Ptree*, TypeInfo&);
    virtual void TypeofFstyleCast(Ptree*, TypeInfo&);
    virtual void TypeofArray(Ptree*, TypeInfo&);
    virtual void TypeofFuncall(Ptree*, TypeInfo&);	// and fstyle cast
    virtual void TypeofPostfix(Ptree*, TypeInfo&);
    virtual void TypeofUserStatement(Ptree*, TypeInfo&);
    virtual void TypeofDotMember(Ptree*, TypeInfo&);
    virtual void TypeofArrowMember(Ptree*, TypeInfo&);
    virtual void TypeofParen(Ptree*, TypeInfo&);
    virtual void TypeofStaticUserStatement(Ptree*, TypeInfo&);

OPENCXX_DEPRECATED_PUBLIC:

    static bool GetArgDeclList(PtreeDeclarator*, Ptree*&);
    static Ptree* FillArgumentName(Ptree*, Ptree*, int arg_name);

public:
    struct NameScope {
	Environment* env;
	Walker* walker;
    };

    void NewScope();
    void NewScope(Environment*);
    void NewScope(Class*);
    Environment* ExitScope();
    void RecordBaseclassEnv(Ptree*);
    NameScope ChangeScope(Environment*);
    void RestoreScope(NameScope&);

protected:
    Ptree* TranslateDeclarators(Ptree*, bool);
    Class* LookupMetaclass(Ptree*, Ptree*, Ptree*, bool);

private:
    Class* LookupBaseMetaclass(Ptree*, Ptree*, bool);

public:
    Ptree* TranslateNewDeclarator(Ptree* decl);
    Ptree* TranslateNewDeclarator2(Ptree* decl);
    Ptree* TranslateArguments(Ptree*);

    static Ptree* FindDeclarator(Ptree*, char*, int, char*, int&,
				 Environment*);
    static bool MatchedDeclarator(Ptree*, char*, int, char*, Environment*);
    static bool WhichDeclarator(Ptree*, Ptree*, int&, Environment*);

    void ErrorMessage(char*, Ptree*, Ptree*);
    void WarningMessage(char*, Ptree*, Ptree*);

    static void InaccurateErrorMessage(char*, Ptree*, Ptree*);
    static void InaccurateWarningMessage(char*, Ptree*, Ptree*);

    static void ChangeDefaultMetaclass(char*);
    static void ChangeDefaultTemplateMetaclass(char*);

OPENCXX_DEPRECATED_PUBLIC:

    static Ptree* GetClassOrEnumSpec(Ptree*);
    static Ptree* GetClassTemplateSpec(Ptree*);
    static Ptree* StripCvFromIntegralType(Ptree*);

public:
    Parser* GetParser() { return parser; }

protected:
    Environment* env;
    Parser* parser;

private:
    static Parser* default_parser;
    static char* default_metaclass;
    static char* default_template_metaclass;
};

}

#endif /* ! guard_opencxx_Walker_h */
