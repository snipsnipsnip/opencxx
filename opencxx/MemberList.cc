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

#include <cstring>
#include <opencxx/MemberList.h>
#include <opencxx/parser/Ptree.h>
#include <opencxx/Class.h>
#include <opencxx/Environment.h>
#include <opencxx/Walker.h>
#include <opencxx/parser/Encoding.h>
#include <opencxx/EncodingUtil.h>
#include <opencxx/parser/token-names.h>
#include <opencxx/parser/PtreeUtil.h>
#include <opencxx/parser/GC.h>

namespace Opencxx
{

MemberList::MemberList()
{
    this_class = 0;
    num = 0;
    size = -1;
    array = 0;
}

MemberList::Mem* MemberList::Ref(int i)
{
    const int unit = 16;
    if(i >= size){
	int old_size = size;
	size = ((unsigned int)i + unit) & ~(unit - 1);
	Mem* a = new (GC) Mem[size];
	if(old_size > 0)
	    memmove(a, array, old_size * sizeof(Mem));

	array = a;
    }

    return &array[i];
}

void MemberList::Make(Class* metaobject)
{
    this_class = metaobject;
    num = 0;

    AppendThisClass(metaobject);

    Environment* env = metaobject->GetEnvironment();
    Ptree* bases = metaobject->BaseClasses();
    while(bases != 0){
	bases = bases->Cdr();		// skip : or ,
	if(bases != 0){
	    AppendBaseClass(env, bases->Car());
	    bases = bases->Cdr();
	}
    }
}

void MemberList::AppendThisClass(Class* metaobject)
{
    int access = PRIVATE;
    Ptree* user_access = 0;
    Ptree* members = metaobject->Members();
    while(members != 0){
	Ptree* def = members->Car();
	if(def->IsA(ntDeclaration)){
	    Ptree* decl;
	    int nth = 0;
	    do{
		int i = nth++;
		decl = PtreeUtil::NthDeclarator(def, i);
		if(decl != 0)
		    Append(def, decl, access, user_access);
	    } while(decl != 0);
	}
	else if(def->IsA(ntAccessSpec)){
	    access = def->Car()->What();
	    user_access = 0;
	}
	else if(def->IsA(ntUserAccessSpec))
	    user_access = def;
	else if(def->IsA(ntAccessDecl)) {
	    /* not implemented */
	}

	members = members->Cdr();
    }
}

void MemberList::Append(Ptree* declaration, Ptree* decl,
			int access, Ptree* user_access)
{
    int len;
    Mem mem;
    char* name = decl->GetEncodedName();
    char* signature = decl->GetEncodedType();
    Environment* e = this_class->GetEnvironment();
    name = EncodingUtil::GetBaseName(name, len, e);

    CheckHeader(declaration, &mem);

    Mem* m = Ref(num++);
    m->supplying = this_class;
    m->definition = declaration;
    m->declarator = decl;
    m->name = name;
    m->signature = signature;
    m->is_constructor = bool(PtreeUtil::Eq(this_class->Name(), name, len));
    m->is_destructor = bool(*name == '~');
    m->is_virtual = mem.is_virtual;
    m->is_static = mem.is_static;
    m->is_mutable = mem.is_mutable;
    m->is_inline = mem.is_inline;
    m->user_mod = mem.user_mod;
    m->access = access;
    m->user_access = user_access;
}

void MemberList::AppendBaseClass(Environment* env, Ptree* base_class)
{
    int access = PRIVATE;
    while(base_class->Cdr() != 0){
	Ptree* p = base_class->Car();
	if(p->IsA(PUBLIC) || p->IsA(PROTECTED) || p->IsA(PRIVATE))
	    access = p->What();

	base_class = base_class->Cdr();
    }	

    Class* metaobject = env->LookupClassMetaobject(base_class->Car());
    if(metaobject == 0)
	return;

    MemberList* mlist = metaobject->GetMemberList();
    for(int i = 0; i < mlist->num; ++i){
	Mem* m = &mlist->array[i];
	Mem* m2 = Lookup(m->name, m->signature);
	if(m2 != 0){				// overwrittten
	    if(!m2->is_virtual)
		m2->is_virtual = m->is_virtual;
	}
	else if(m->access != PRIVATE){		// inherited
	    m2 = Ref(num++);
	    *m2 = *m;
	    if(access == PRIVATE)
		m2->access = PRIVATE;
	    else if(access == PROTECTED)
		m2->access = PROTECTED;
	}
    }
}

MemberList::Mem* MemberList::Lookup(char* name, char* signature)
{
    for(int i = 0; i < num; ++i){
	Mem* m = Ref(i);
	if(strcmp(m->name, name) == 0 && strcmp(m->signature, signature) == 0)
	    return m;
    }

    return 0;
}

int MemberList::Lookup(char* name, int len, char* signature)
{
    for(int i = 0; i < num; ++i){
	Mem* m = Ref(i);
	if(strcmp(m->signature, signature) == 0
	   && strncmp(m->name, name, len) == 0 && m->name[len] == '\0')
	    return i;
    }

    return -1;
}

int MemberList::Lookup(Environment* env, Ptree* member, int index)
{
    char* name;
    int len;

    if(member == 0)
	return -1;
    else if(member->IsLeaf()){
	name = member->GetPosition();
	len = member->GetLength();
    }
    else
	name = EncodingUtil::GetBaseName(member->GetEncodedName(), len, env);

    for(int i = 0; i < num; ++i){
	Mem* m = Ref(i);
	if(strncmp(m->name, name, len) == 0 && m->name[len] == '\0')
	    if(index-- <= 0)
		return i;
    }

    return -1;
}

int MemberList::Lookup(Environment*, char* name, int index)
{
    if(name == 0)
	return -1;

    for(int i = 0; i < num; ++i){
	Mem* m = Ref(i);
	if(strcmp(m->name, name) == 0)
	    if(index-- <= 0)
		return i;
    }

    return -1;
}

void MemberList::CheckHeader(Ptree* declaration, Mem* m)
{
    m->is_virtual = false;
    m->is_static = false;
    m->is_mutable = false;
    m->is_inline = false;
    m->user_mod = 0;

    Ptree* header = declaration->Car();
    while(header != 0){
	Ptree* h = header->Car();
	if(h->IsA(VIRTUAL))
	    m->is_virtual = true;
	else if(h->IsA(STATIC))
	    m->is_static = true;
	else if(h->IsA(MUTABLE))
	    m->is_mutable = true;
	else if(h->IsA(INLINE))
	    m->is_inline = true;
	else if(h->IsA(ntUserdefKeyword))
	    m->user_mod = h;

	header = header->Cdr();
    }

    Ptree* d = PtreeUtil::Third(declaration);
    if(d != 0 && d->IsA(ntDeclarator))
	m->is_inline = true;
}

}

