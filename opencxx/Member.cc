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

#include <opencxx/Member.h>
#include <opencxx/Class.h>
#include <opencxx/parser/Encoding.h>
#include <opencxx/EncodingUtil.h>
#include "opencxx/parser/CerrErrorLog.h"
#include <opencxx/parser/ErrorLog.h>
#include <opencxx/MemberList.h>
#include <opencxx/parser/MopMsg.h>
#include <opencxx/parser/PtreeArray.h>
#include <opencxx/PtreeTypeUtil.h>
#include <opencxx/parser/PtreeUtil.h>
#include <opencxx/parser/TheErrorLog.h>
#include <opencxx/TypeInfo.h>
#include <opencxx/parser/Ptree.h>
#include <opencxx/parser/PtreeDeclarator.h>
#include <opencxx/parser/token-names.h>

namespace Opencxx
{

Member::Member()
{
    metaobject = 0;
    declarator = 0;
    nth = -1;
    removed = false;
    new_name = 0;
    new_args = 0;
    new_init = 0;
    new_body = 0;
    arg_name_filled = false;

    implementation = 0;
    original_decl = 0;
}

Member::Member(const Member& m)
{
    metaobject = m.metaobject;
    declarator = m.declarator;
    nth = m.nth;
    removed = m.removed;
    new_name = m.new_name;
    new_args = m.new_args;
    new_init = m.new_init;
    new_body = m.new_body;
    arg_name_filled = m.arg_name_filled;

    implementation = m.implementation;
    original_decl = m.original_decl;
}

Member::Member(Class* c, Ptree* decl)
{
    metaobject = c;
    declarator = decl;
    nth = -1;
    removed = false;
    new_name = 0;
    new_args = 0;
    new_init = 0;
    new_body = 0;
    arg_name_filled = false;

    implementation = 0;
    original_decl = 0;
}

void Member::Set(Class* c, Ptree* decl, int n)
{
    metaobject = c;
    declarator = decl;
    nth = n;
    removed = false;
    new_name = 0;
    new_args = 0;
    new_init = 0;
    new_body = 0;
    arg_name_filled = false;

    implementation = 0;
    original_decl = 0;
}

void Member::Signature(TypeInfo& t) const
{
    if(declarator == 0){
	TheErrorLog().Report(MopMsg(Msg::Fatal, "Member::Signature()", "not initialized object."));
	return;
    }

    char* type = declarator->GetEncodedType();
    if(type == 0)
	t.Unknown();
    else
	t.Set(type, metaobject->GetEnvironment());
}

Ptree* Member::Name()
{
    int len;
    char* name = Name(len);
    return Encoding::NameToPtree(name, len);
}

char* Member::Name(int& len)
{
    if(declarator == 0){
	TheErrorLog().Report(MopMsg(Msg::Fatal, "Member::Name()", "not initialized object."));
	return 0;
    }

    char* name = declarator->GetEncodedName();
    if(name != 0){
	Environment* e = metaobject->GetEnvironment();
	name = EncodingUtil::GetBaseName(name, len, e);
    }

    return name;
}

Ptree* Member::Comments()
{
    if(declarator == 0){
	TheErrorLog().Report(MopMsg(Msg::Fatal, "Member::Comments()", "not initialized object."));
	return 0;
    }

    if (declarator->IsA(ntDeclarator))
	return ((PtreeDeclarator*)declarator)->GetComments();
    else
	return 0;
}

int Member::Nth()
{
    if(Find())
	return nth;
    else
	return -1;
}

Class* Member::Supplier()
{
    if(Find())
	return metaobject->GetMemberList()->Ref(nth)->supplying;
    else
	return 0;
}

bool Member::IsConstructor()
{
    if(declarator == 0){
	TheErrorLog().Report(MopMsg(Msg::Fatal, "Member::IsConstructor()", "not initialized object."));
	return false;
    }

    char* name = declarator->GetEncodedName();
    if(name != 0){
	int len;
	Environment* e = metaobject->GetEnvironment();
	name = EncodingUtil::GetBaseName(name, len, e);
	if(name != 0) {
	    Class* sup = Supplier();
	    if (sup != 0)
		return PtreeUtil::Eq(sup->Name(), name, len);
	}
    }

    return false;
}

bool Member::IsDestructor()
{
    if(declarator == 0){
	TheErrorLog().Report(MopMsg(Msg::Fatal, "Member::IsDestructor()", "not initialized object."));
	return false;
    }

    char* name = declarator->GetEncodedName();
    if(name != 0){
	int len;
	Environment* e = metaobject->GetEnvironment();
	name = EncodingUtil::GetBaseName(name, len, e);
	if(name != 0)
	    return bool(*name == '~');
    }

    return false;
}

bool Member::IsFunction()
{
    TypeInfo t;
    Signature(t);
    return t.IsFunction();
}

bool Member::IsPublic()
{
    if(Find())
	return bool(metaobject->GetMemberList()->Ref(nth)->access
		    == PUBLIC);
    else
	return false;
}

bool Member::IsProtected()
{
    if(Find())
	return bool(metaobject->GetMemberList()->Ref(nth)->access
		    == PROTECTED);
    else
	return false;
}

bool Member::IsPrivate()
{
    if(Find())
	return bool(metaobject->GetMemberList()->Ref(nth)->access
		    == PRIVATE);
    else
	return false;
}

bool Member::IsStatic()
{
    if(Find())
	return metaobject->GetMemberList()->Ref(nth)->is_static;
    else
	return false;
}

bool Member::IsMutable()
{
    if(Find())
	return metaobject->GetMemberList()->Ref(nth)->is_mutable;
    else
	return false;
}

bool Member::IsInline()
{
    if(Find() && metaobject->GetMemberList()->Ref(nth)->is_inline)
	return true;

    if(IsFunctionImplementation())
	return IsInlineFuncImpl();
    else
	return false;
}

bool Member::IsInlineFuncImpl()
{
    Ptree* header = implementation->Car();
    while(header != 0){
	Ptree* h = header->Car();
	if(h->IsA(INLINE))
	    return true;

	header = header->Cdr();
    }

    return false;
}

bool Member::IsVirtual()
{
    if(Find())
	return metaobject->GetMemberList()->Ref(nth)->is_virtual;
    else
	return false;
}

bool Member::IsPureVirtual()
{
    using namespace PtreeUtil;
    if(IsFunction())
	return Eq(Last(declarator)->Car(),'0');
    else
	return false;
}

Ptree* Member::GetUserAccessSpecifier()
{
    if(Find())
	return metaobject->GetMemberList()->Ref(nth)->user_access;
    else
	return 0;
}

bool Member::GetUserArgumentModifiers(PtreeArray& mods)
{
    Ptree* args;

    mods.Clear();
    if(!Find())
	return false;

    if(!PtreeUtil::GetArgDeclList((PtreeDeclarator*)declarator, args))
	return false;

    while(args != 0){
	Ptree* a = args->Car();
	if(!a->IsLeaf() && a->Car()->IsA(ntUserdefKeyword))
	    mods.Append(a->Car());
	else
	    mods.Append(0);
	
	args = PtreeUtil::ListTail(args, 2);	// skip ,
    }

    return true;
}

Ptree* Member::GetUserMemberModifier()
{
    if(Find())
	return metaobject->GetMemberList()->Ref(nth)->user_mod;
    else
	return 0;
}

bool Member::Find()
{
    if(nth >= 0)
	return true;
    else if(metaobject == 0 || declarator == 0)
	return false;

    MemberList* mlist = metaobject->GetMemberList();

    int len;
    char* name = Name(len);
    char* sig = declarator->GetEncodedType();
    if(mlist == 0 || name == 0 || sig == 0)
	return false;

    nth = mlist->Lookup(name, len, sig);
    if(nth >= 0){
	MemberList::Mem* m = mlist->Ref(nth);
	metaobject = m->supplying;
	declarator = m->declarator;
	return true;
    }
    else
	return false;
}

void Member::SetQualifiedName(Ptree* name)
{
    new_name = name;
}

void Member::SetName(Ptree* name)
{
    if(IsFunctionImplementation())
	SetName(name, original_decl);
    else
	SetName(name, declarator);
}

void Member::SetName(Ptree* name, Ptree* decl)
{
    using PtreeUtil::First;
    using PtreeUtil::Last;
    
    if(decl == 0){
	TheErrorLog().Report(MopMsg(Msg::Fatal, "Member::SetName()", "not initialized object."));
	return;
    }

    char* encoded = decl->GetEncodedName();
    if(encoded != 0 && *encoded == 'Q'){
	Ptree* qname = ((PtreeDeclarator*)decl)->Name();
	Ptree* old_name = First(Last(qname));
	new_name = PtreeUtil::ShallowSubst(name, old_name, qname);
    }
    else
	new_name = name;
}

Ptree* Member::ArgumentList()
{
    if(IsFunctionImplementation())
	return ArgumentList(original_decl);
    else
	return ArgumentList(declarator);
}

Ptree* Member::ArgumentList(Ptree* decl)
{
    Ptree* args;
    if(PtreeUtil::GetArgDeclList((PtreeDeclarator*)decl, args))
	return args;
    else
	return 0;
}

void Member::SetArgumentList(Ptree* args)
{
    new_args = args;
}

Ptree* Member::MemberInitializers()
{
    if(IsFunctionImplementation())
	return MemberInitializers(original_decl);
    else
	return MemberInitializers(declarator);
}

Ptree* Member::MemberInitializers(Ptree* decl)
{
    if(IsConstructor()){
	Ptree* init = PtreeUtil::Last(decl)->Car();
	if(!init->IsLeaf() && PtreeUtil::Eq(init->Car(), ':'))
	    return init;
    }

    return 0;
}

void Member::SetMemberInitializers(Ptree* init)
{
    new_init = init;
}

Ptree* Member::FunctionBody()
{
    if(IsFunctionImplementation())
	return PtreeUtil::Nth(implementation, 3);
    else if(Find()){
	Ptree* def = metaobject->GetMemberList()->Ref(nth)->definition;
	Ptree* decls = PtreeUtil::Third(def);
	if(decls->IsA(ntDeclarator))
	    return PtreeUtil::Nth(def, 3);
    }

    return 0;
}

void Member::SetFunctionBody(Ptree* body)
{
    new_body = body;
}

Ptree* Member::Arguments()
{
    return Arguments(ArgumentList(), 0);
}

Ptree* Member::Arguments(Ptree* args, int i)
{
    Ptree* rest;

    if(args == 0)
	return args;

    if(args->Cdr() == 0)
	rest = 0;
    else{
	rest = Arguments(args->Cdr()->Cdr(), i + 1);	// skip ","
	rest = PtreeUtil::Cons(args->Cdr()->Car(), rest);
    }

    Ptree* a = args->Car();
    Ptree* p;
    if(a->IsLeaf())
	p = a;
    else{
	if(a->Car()->IsA(ntUserdefKeyword) || a->Car()->IsA(REGISTER))
	    p = PtreeUtil::Third(a);
	else
	    p = PtreeUtil::Second(a);

	p = ((PtreeDeclarator*)p)->Name();
    }

    if(p == 0){
	arg_name_filled = true;
	p = Ptree::Make(PtreeTypeUtil::TheArgumentName(), i);
    }

    return PtreeUtil::Cons(p, rest);
}

void Member::Copy(Member* mem, void* ptr)
{
    ChangedMemberList::Cmem* cmem = (ChangedMemberList::Cmem*)ptr;
    ChangedMemberList::Copy(mem, cmem, Class::Undefined);
    if(mem->IsFunctionImplementation()){
	cmem->declarator = mem->original_decl;
	cmem->def = mem->implementation;
    }
}

}

