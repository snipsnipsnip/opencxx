//@beginlicenses@
//@license{chiba-tokyo}{}@
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
//@endlicenses@

#include <opencxx/parser/ptreeAll.h>
#include <opencxx/ClassBodyWalker.h>
#include <opencxx/Class.h>
#include <opencxx/memberAll.h>
#include <opencxx/Environment.h>
#include <opencxx/parser/Ptree.h>
#include <opencxx/parser/PtreeClassBody.h>
#include <opencxx/parser/PtreeTypedef.h>
#include <opencxx/parser/PtreeDeclarator.h>
#include <opencxx/parser/PtreeDeclaration.h>

using namespace std;

namespace Opencxx
{

Ptree* ClassBodyWalker::TranslateClassBody(Ptree* block, Ptree*,
					   Class* metaobject)
{
    Ptree* block2;

    Environment* fenv = metaobject->GetEnvironment();
    if(fenv == 0)
	fenv = env;	// should not reach here.

    NameScope old_env = ChangeScope(fenv);

    PtreeArray array;
    bool changed = false;
    Ptree* body = PtreeUtil::Second(block);
    Ptree* rest = body;
    while(rest != 0){
	Ptree* p = rest->Car();
	Ptree* q = Translate(p);
	array.Append(q);
	if(p != q)
	    changed = true;

	rest = rest->Cdr();
    }

    AppendNewMembers(metaobject, array, changed);

    Ptree* appended = metaobject->GetAppendedCode();
    if(appended != 0){
	changed = true;
	while(appended != 0){
	    array.Append(appended->Car());
	    appended = appended->Cdr();
	}
    }

    if(changed)
	block2 = new PtreeClassBody(PtreeUtil::First(block), array.All(),
				    PtreeUtil::Third(block));
    else
	block2 = block;

    RestoreScope(old_env);
    return block2;
}

void ClassBodyWalker::AppendNewMembers(Class* metaobject, PtreeArray& array,
				       bool& changed)
{
    ChangedMemberList::Cmem* m;
    ChangedMemberList* appended_list = metaobject->GetAppendedMembers();
    if(appended_list == 0)
	return;

    int i = 0;
    while((m = appended_list->Get(i++)) != 0)
	if(m->def != 0){
	    changed = true;
	    ClassWalker w(this);
	    array.Append(w.ConstructAccessSpecifier(m->access));
	    array.Append(w.ConstructMember(m));
	}
}

Ptree* ClassBodyWalker::TranslateTypespecifier(Ptree* tspec)
{
    if(tspec_list == 0)
	return tspec;

    int n = tspec_list->Number();
    for(int i =  0; i < n; i += 2)
	if(tspec_list->Ref(i) == tspec)
	    return tspec_list->Ref(i + 1);

    return tspec;
}

Ptree* ClassBodyWalker::TranslateTypedef(Ptree* def)
{
    Ptree *tspec, *tspec2;

    tspec = PtreeUtil::Second(def);
    tspec2 = TranslateTypespecifier(tspec);

    if(tspec == tspec2)
	return def;
    else
	return new PtreeTypedef(PtreeUtil::First(def),
				PtreeUtil::List(tspec2,
					    PtreeUtil::ListTail(def, 2)));
}

Ptree* ClassBodyWalker::TranslateMetaclassDecl(Ptree*)
{
    return 0;
}

Ptree* ClassBodyWalker::TranslateDeclarators(Ptree* decls)
{
    return ClassWalker::TranslateDeclarators(decls, false);
}

Ptree* ClassBodyWalker::TranslateAssignInitializer(PtreeDeclarator* decl,
						   Ptree* init)
{
    ClassWalker w(this);

    return w.TranslateAssignInitializer(decl, init);
}

Ptree* ClassBodyWalker::TranslateInitializeArgs(PtreeDeclarator* decl,
						Ptree* init)
{
    ClassWalker w(this);

    return w.TranslateInitializeArgs(decl, init);
}

Ptree* ClassBodyWalker::TranslateDeclarator(bool record,
					    PtreeDeclarator* decl)
{
    return TranslateDeclarator(record, decl, true);
}

Ptree* ClassBodyWalker::TranslateDeclarator(bool record,
					    PtreeDeclarator* decl,
					    bool append_body)
{
    ClassWalker w(this);

    Class* metaobject = env->LookupThis();
    if(metaobject != 0){
	ChangedMemberList::Cmem* m = metaobject->GetChangedMember(decl);
	if(m != 0){
	    Ptree* decl2 = w.MakeMemberDeclarator(record, m, decl);
	    if(m->removed || m->body == 0 || !append_body)
		return decl2;
	    else
		return PtreeUtil::List(decl2, m->body);
	}
    }

    return w.TranslateDeclarator(record, decl);
}

Ptree* ClassBodyWalker::TranslateFunctionImplementation(Ptree* impl)
{
    Ptree* sspec = PtreeUtil::First(impl);
    Ptree* sspec2 = TranslateStorageSpecifiers(sspec);
    Ptree* tspec = PtreeUtil::Second(impl);
    Ptree* decl = PtreeUtil::Third(impl);
    Ptree* body = PtreeUtil::Nth(impl, 3);
    Ptree* decl2;
    Ptree* body2;

    Ptree* tspec2 = TranslateTypespecifier(tspec);
    Environment* fenv = env->DontRecordDeclarator(decl);

    if(fenv == 0){
	// shouldn't reach here.
	NewScope();
	ClassWalker w(this);	// this should be after NewScope().
	decl2 = w.TranslateDeclarator(true, (PtreeDeclarator*)decl);
	body2 = w.TranslateFunctionBody(body);
	ExitScope();
    }
    else{
	bool is_nested_class = bool(env != fenv);
	NameScope old_env = ChangeScope(fenv);
	NewScope();
	ClassWalker w(this);
	if(is_nested_class){
	    // if it's a member function of a nested class
	    decl2 = w.TranslateDeclarator(true, (PtreeDeclarator*)decl);
	    body2 = w.TranslateFunctionBody(body);
	}
	else{
	    decl2 = TranslateDeclarator(true, (PtreeDeclarator*)decl, false);
	    Class* metaobject = fenv->IsClassEnvironment();
	    ChangedMemberList::Cmem* m = 0;
	    if(metaobject != 0)
		m = metaobject->GetChangedMember(decl);

	    if(m != 0 && m->body != 0)
		body2 = m->body;
	    else
		body2 = w.TranslateFunctionBody(body);
	}

	ExitScope();
	RestoreScope(old_env);
    }

    if(sspec == sspec2 && tspec == tspec2 && decl == decl2 && body == body2)
	return impl;
    else if(decl2 == 0)
	return new PtreeDeclaration(0, PtreeUtil::List(0,
						     Class::semicolon_t));
    else
	return new PtreeDeclaration(sspec2,
				    PtreeUtil::List(tspec2, decl2, body2));
}

}
