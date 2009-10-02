#ifndef guard_opencxx_AbstractTranslatingWalker_h
#define guard_opencxx_AbstractTranslatingWalker_h
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

namespace Opencxx
{

class Ptree;
class PtreeDeclarator;
class Class;

class AbstractTranslatingWalker {
public:
    virtual Ptree* TranslatePtree(Ptree*) = 0; ///< catch-all

    virtual Ptree* TranslateTypedef(Ptree*) = 0;
    virtual Ptree* TranslateTemplateDecl(Ptree*) = 0;
    virtual Ptree* TranslateTemplateInstantiation(Ptree*) = 0;
    virtual Ptree* TranslateTemplateInstantiation(Ptree*, Ptree*,
						  Ptree*, Class*) = 0;
    virtual Ptree* TranslateExternTemplate(Ptree*) = 0;
    virtual Ptree* TranslateTemplateClass(Ptree*, Ptree*) = 0;
    virtual Ptree* TranslateTemplateFunction(Ptree*, Ptree*) = 0;
    virtual Ptree* TranslateMetaclassDecl(Ptree*) = 0;
    virtual Ptree* TranslateLinkageSpec(Ptree*) = 0;
    virtual Ptree* TranslateNamespaceSpec(Ptree*) = 0;
    virtual Ptree* TranslateNamespaceAlias(Ptree*) = 0;
    virtual Ptree* TranslateUsing(Ptree*) = 0;
    virtual Ptree* TranslateDeclaration(Ptree*) = 0;
    virtual Ptree* TranslateStorageSpecifiers(Ptree*) = 0;
    virtual Ptree* TranslateDeclarators(Ptree*) = 0;
    virtual Ptree* TranslateDeclarator(bool, PtreeDeclarator*) = 0;
    virtual Ptree* TranslateArgDeclList(bool, Ptree*, Ptree*) = 0;
    virtual Ptree* TranslateInitializeArgs(PtreeDeclarator*, Ptree*) = 0;
    virtual Ptree* TranslateAssignInitializer(PtreeDeclarator*, Ptree*) = 0;
    virtual Ptree* TranslateFunctionImplementation(Ptree*) = 0;
    virtual Ptree* TranslateFunctionBody(Ptree*) = 0;
    virtual Ptree* TranslateBrace(Ptree*) = 0;
    virtual Ptree* TranslateBlock(Ptree*) = 0;
    virtual Ptree* TranslateClassBody(Ptree*, Ptree*, Class*) = 0;

    virtual Ptree* TranslateClassSpec(Ptree*) = 0;
    virtual Ptree* TranslateClassSpec(Ptree*, Ptree*, Ptree*, Class*) = 0;
    virtual Ptree* TranslateEnumSpec(Ptree*) = 0;

    virtual Ptree* TranslateAccessSpec(Ptree*) = 0;
    virtual Ptree* TranslateAccessDecl(Ptree*) = 0;
    virtual Ptree* TranslateUserAccessSpec(Ptree*) = 0;

    virtual Ptree* TranslateIf(Ptree*) = 0;
    virtual Ptree* TranslateSwitch(Ptree*) = 0;
    virtual Ptree* TranslateWhile(Ptree*) = 0;
    virtual Ptree* TranslateDo(Ptree*) = 0;
    virtual Ptree* TranslateFor(Ptree*) = 0;
    virtual Ptree* TranslateTry(Ptree*) = 0;
    virtual Ptree* TranslateBreak(Ptree*) = 0;
    virtual Ptree* TranslateContinue(Ptree*) = 0;
    virtual Ptree* TranslateReturn(Ptree*) = 0;
    virtual Ptree* TranslateGoto(Ptree*) = 0;
    virtual Ptree* TranslateCase(Ptree*) = 0;
    virtual Ptree* TranslateDefault(Ptree*) = 0;
    virtual Ptree* TranslateLabel(Ptree*) = 0;
    virtual Ptree* TranslateExprStatement(Ptree*) = 0;

    virtual Ptree* TranslateTypespecifier(Ptree*) = 0;

    virtual Ptree* TranslateComma(Ptree*) = 0;
    virtual Ptree* TranslateAssign(Ptree*) = 0;
    virtual Ptree* TranslateCond(Ptree*) = 0;
    virtual Ptree* TranslateInfix(Ptree*) = 0;
    virtual Ptree* TranslatePm(Ptree*) = 0;
    virtual Ptree* TranslateCast(Ptree*) = 0;
    virtual Ptree* TranslateUnary(Ptree*) = 0;
    virtual Ptree* TranslateThrow(Ptree*) = 0;
    virtual Ptree* TranslateSizeof(Ptree*) = 0;
    virtual Ptree* TranslateTypeid(Ptree*) = 0;
    virtual Ptree* TranslateTypeof(Ptree*) = 0;
    virtual Ptree* TranslateNew(Ptree*) = 0;
    virtual Ptree* TranslateNew2(Ptree*, Ptree*, Ptree*, Ptree*,
				Ptree*, Ptree*, Ptree*) = 0;
    virtual Ptree* TranslateNew3(Ptree* type) = 0;
    virtual Ptree* TranslateDelete(Ptree*) = 0;
    virtual Ptree* TranslateThis(Ptree*) = 0;
    virtual Ptree* TranslateVariable(Ptree*) = 0;
    virtual Ptree* TranslateFstyleCast(Ptree*) = 0;
    virtual Ptree* TranslateArray(Ptree*) = 0;
    virtual Ptree* TranslateFuncall(Ptree*) = 0;	// and fstyle cast
    virtual Ptree* TranslatePostfix(Ptree*) = 0;
    virtual Ptree* TranslateUserStatement(Ptree*) = 0;
    virtual Ptree* TranslateDotMember(Ptree*) = 0;
    virtual Ptree* TranslateArrowMember(Ptree*) = 0;
    virtual Ptree* TranslateParen(Ptree*) = 0;
    virtual Ptree* TranslateStaticUserStatement(Ptree*) = 0;

protected:
   virtual ~AbstractTranslatingWalker() {}

};

}


#endif /* ! guard_opencxx_AbstractTranslatingWalker_h */
