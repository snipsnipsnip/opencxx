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
#include <opencxx/ChangedMemberList.h>
#include <opencxx/Member.h>
#include <opencxx/MemberList.h>
#include <opencxx/Class.h>
#include <opencxx/parser/GC.h>

namespace Opencxx
{

ChangedMemberList::ChangedMemberList()
{
    num = 0;
    size = -1;
    array = 0;
}

void ChangedMemberList::Append(Member* m, int access)
{
    Cmem* mem = Ref(num++);
    Copy(m, mem, access);
}

void ChangedMemberList::Copy(Member* src, Cmem* dest, int access)
{
    dest->declarator = src->declarator;
    dest->removed = src->removed;
    dest->name = src->new_name;
    dest->args = src->new_args;
    dest->init = src->new_init;
    dest->body = src->new_body;
    dest->arg_name_filled = src->arg_name_filled;

    if(src->Find()){
	MemberList::Mem* m = src->metaobject->GetMemberList()->Ref(src->nth);
	dest->def = m->definition;
	if(access == Class::Undefined)
	    dest->access = m->access;
	else
	    dest->access = access;
    }
    else{
	dest->def = 0;
	if(access == Class::Undefined)
	    dest->access = Class::Public;
	else
	    dest->access = access;
    }
}

ChangedMemberList::Cmem* ChangedMemberList::Lookup(Ptree* decl)
{
    for(int i = 0; i < num; ++i){
	Cmem* m = Ref(i);
	if(m->declarator == decl)
	    return m;
    }

    return 0;
}

ChangedMemberList::Cmem* ChangedMemberList::Get(int i)
{
    if(i >= num)
	return 0;
    else
	return Ref(i);
}

ChangedMemberList::Cmem* ChangedMemberList::Ref(int i)
{
    const int unit = 16;
    if(i >= size){
	int old_size = size;
	size = ((unsigned int)i + unit) & ~(unit - 1);
	Cmem* a = new (GC) Cmem[size];
	if(old_size > 0)
	    memmove(a, array, old_size * sizeof(Cmem));

	array = a;
    }

    return &array[i];
}

}

