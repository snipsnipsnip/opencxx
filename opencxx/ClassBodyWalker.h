#ifndef guard_opencxx_ClassBodyWalker_h
#define guard_opencxx_ClassBodyWalker_h

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

#include <opencxx/ClassWalker.h>

class Ptree;
class Class;
class PtreeArray;
class PtreeDeclarator;

namespace Opencxx
{

// ClassBodyWalker is only used by ClassWalker::TranslateClassSpec.

class ClassBodyWalker : public ClassWalker {
public:
    ClassBodyWalker(Walker* w, PtreeArray* tlist) : ClassWalker(w) {
	tspec_list = tlist;
    }
    Ptree* TranslateClassBody(Ptree* block, Ptree* bases, Class*);
    void AppendNewMembers(Class*, PtreeArray&, bool&);
    Ptree* TranslateTypespecifier(Ptree* tspec);
    Ptree* TranslateTypedef(Ptree* def);
    Ptree* TranslateMetaclassDecl(Ptree* decl);
    Ptree* TranslateDeclarators(Ptree* decls);
    Ptree* TranslateAssignInitializer(PtreeDeclarator* decl, Ptree* init);
    Ptree* TranslateInitializeArgs(PtreeDeclarator* decl, Ptree* init);
    Ptree* TranslateDeclarator(bool record, PtreeDeclarator* decl);
    Ptree* TranslateDeclarator(bool record, PtreeDeclarator* decl,
			       bool append_body);
    Ptree* TranslateFunctionImplementation(Ptree* impl);

private:
    PtreeArray* tspec_list;
};

}

#endif /* ! guard_opencxx_ClassBodyWalker_h */
