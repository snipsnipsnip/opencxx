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

#include <opencxx/Environment.h>
#include <cstring>
#include <iostream>
#include <opencxx/parser/BigHashTable.h>
#include <opencxx/BindClassName.h>
#include <opencxx/BindEnumName.h>
#include <opencxx/BindTemplateClass.h>
#include <opencxx/BindTemplateFunction.h>
#include <opencxx/BindTypedefName.h>
#include <opencxx/BindVarName.h>
#include <opencxx/Class.h>
#include <opencxx/parser/Encoding.h>
#include <opencxx/EncodingUtil.h>
#include "opencxx/parser/CerrErrorLog.h"
#include <opencxx/parser/ErrorLog.h>
#include <opencxx/parser/HashTable.h>
#include <opencxx/parser/MopMsg.h>
#include <opencxx/parser/Parser.h>
#include <opencxx/parser/TheErrorLog.h>
#include <opencxx/TypeInfo.h>
#include <opencxx/Walker.h>
#include <opencxx/parser/GC.h>
#include <opencxx/parser/Leaf.h>
#include <opencxx/parser/Ptree.h>
#include <opencxx/parser/token-names.h>

namespace Opencxx
{

PtreeArray* Environment::classkeywords = 0;

Environment::Environment(Walker* w)
: baseclasses_or_using(0), namespaces(0)
{
    htable = new BigHashTable;          // base environment
    next = 0;
    metaobject = 0;
    walker = w;
    namespace_name = 0;
}

Environment::Environment(Environment* e)
: baseclasses_or_using(0), namespaces(0)
{
    htable = new HashTable;
    next = e;
    metaobject = 0;
    walker = e->walker;
    namespace_name = 0;
}

Environment::Environment(Environment* e, Walker* w)
: baseclasses_or_using(0), namespaces(0)
{
    htable = new HashTable;
    next = e;
    metaobject = 0;
    walker = w;
    namespace_name = 0;
}

bool Environment::IsEmpty()
{
    return bool(htable->IsEmpty());
}

Environment* Environment::GetBottom()
{
    Environment* p;
    for(p = this; p->next != 0; p = p->next)
        ;

    return p;
}

Class* Environment::LookupClassMetaobject(Ptree* name)
{
    TypeInfo tinfo;
    Bind* bind = 0;

    if (this == 0) {
	TheErrorLog().Report(
	    MopMsg(Msg::Fatal, 
		   "Environment::LookupClassMetaobject()", 
		   "0 environment"));
        return 0;
    }

    if(name == 0)
        return 0;
    else if(name->IsLeaf()){
        if(LookupType(name->GetPosition(), name->GetLength(), bind))
            if(bind != 0){
                bind->GetType(tinfo, this);
                return tinfo.ClassMetaobject();
            }

        return 0;
    }
    else{
        int len;
        Environment* e = this;
        char* base = EncodingUtil::GetBaseName(name->GetEncodedName(), len, e);
        if(base != 0 && e != 0)
            if(LookupType(base, len, bind))
                if(bind != 0){
                    bind->GetType(tinfo, this);
                    return tinfo.ClassMetaobject();
                }

        return 0;
    }
}

bool Environment::LookupType(const char* name, int len, Bind*& t)
{
    Environment* p;

    for(p = this; p != 0; p = p->next){
        int count = 0;
        while(p->htable->LookupEntries((char*)name, len, (HashTable::Value*)&t,
                                       count))
            if(t != 0)
                switch(t->What()){
                case Bind::isVarName :
                case Bind::isTemplateFunction :
                    break;
                default :
                    return true;
                }

        if (SearchBaseOrUsing(p, &Environment::LookupType, name, len, t))
            return true;
    }

    return false;
}

bool Environment::SearchBaseOrUsing(Environment* this_env,
                        bool (Environment::*func)(const char*, int, Bind*&),
                        const char* name, int len, Bind*& t)
{
    unsigned n = this_env->baseclasses_or_using.Number();
    for(unsigned i = 0; i < n; ++i) {
        Environment* e = this_env->baseclasses_or_using.Ref(i);
        Environment* e0 = e->next;
        e->next = 0;
        bool result = (e->*func)(name, len, t);
        e->next = e0;
        if (result)
            return true;
    }

    return false;
}

bool Environment::Lookup(Ptree* name, TypeInfo& t)
{
    Bind* bind;

    if(Lookup(name, bind) && bind != 0){
        bind->GetType(t, this);
        return true;
    }
    else{
        t.Unknown();
        return false;
    }
}

bool Environment::Lookup(Ptree* name, bool& is_type_name, TypeInfo& t)
{
    Bind* bind;

    if(Lookup(name, bind) && bind != 0){
        is_type_name = bind->IsType();
        bind->GetType(t, this);
        return true;
    }
    else{
        t.Unknown();
        return false;
    }
}

bool Environment::Lookup(Ptree* name, Bind*& bind)
{
    bind = 0;
    if(this == 0){
        TheErrorLog().Report(MopMsg(Msg::Fatal, "Environment::Lookup()", "0 environment"));
        return false;
    }

    if(name == 0)
        return false;
    else if(name->IsLeaf())
        return LookupAll(name->GetPosition(), name->GetLength(), bind);
    else{
        char* encode = name->GetEncodedName();
        if(encode == 0)
            return false;
        else{
            int len;
            Environment* e = this;
            char* base = EncodingUtil::GetBaseName(encode, len, e);
            if(base != 0 && e != 0)
                return e->LookupAll(base, len, bind);
            else
                return false;
        }
    }
}

bool Environment::LookupTop(Ptree* name, Bind*& bind)
{
    bind = 0;
    if(this == 0){
        TheErrorLog().Report(MopMsg(Msg::Fatal, "Environment::LookupTop()", "0 environment"));
        return false;
    }

    if(name == 0)
        return false;
    else if(name->IsLeaf())
        return LookupTop(name->GetPosition(), name->GetLength(), bind);
    else{
        char* encode = name->GetEncodedName();
        if(encode == 0)
            return false;
        else{
            int len;
            Environment* e = this;
            char* base = EncodingUtil::GetBaseName(encode, len, e);
            if(base != 0 && e != 0)
                return e->LookupTop(base, len, bind);
            else
                return false;
        }
    }
}

/* Search only this environment.
 */
bool Environment::LookupTop(const char* name, int len, Bind*& t)
{
    if(htable->Lookup((char*)name, len, (HashTable::Value*)&t))
        return true;
    else
        return SearchBaseOrUsing(this, &Environment::LookupTop, name, len, t);
}

/* Search all environments.
 */
bool Environment::LookupAll(const char* name, int len, Bind*& t)
{
    Environment* p;

    for(p = this; p != 0; p = p->next)
        if(p->htable->Lookup((char*)name, len, (HashTable::Value*)&t))
            return true;
        else
            if (SearchBaseOrUsing(p, &Environment::LookupAll, name, len, t))
                return true;

    return false;
}

bool Environment::RecordVariable(char* name, Class* c)
{
    Encoding encode;
    encode.SimpleName(c->Name());
    return htable->AddEntry(name, new BindVarName(encode.Get())) >= 0;
}

bool Environment::RecordPointerVariable(char* name, Class* c)
{
    Encoding encode;
    encode.SimpleName(c->Name());
    encode.PtrOperator('*');
    return htable->AddEntry(name, new BindVarName(encode.Get())) >= 0;
}

int Environment::AddEntry(char* key, int len, Bind* b) {
    return htable->AddEntry(key, len, b);
}

int Environment::AddDupEntry(char* key, int len, Bind* b) {
    return htable->AddDupEntry(key, len, b);
}

void Environment::RecordNamespace(Environment* name_space, Ptree* name)
{
    if (name_space != 0) {
        name_space->namespace_name = name;
        namespaces.Append(name_space);
    }
}

Environment* Environment::LookupNamespace(const char* name, int len)
{
    Environment* e = this;
    while (e != 0) {
        Environment* ns = e->LookupNamespace0(name, len);
        if (ns != 0)
            return ns;

        unsigned n = e->baseclasses_or_using.Number();
        for(unsigned i = 0; i < n; ++i) {
            Environment* p = e->baseclasses_or_using.Ref(i);
            Environment* p0 = p->next;
            p->next = 0;
            ns = p->LookupNamespace(name, len);
            p->next = p0;
            if (ns != 0)
                return ns;
        }

        e = e->next;
    }

    return 0;
}

Environment* Environment::LookupNamespace0(Ptree* name)
{
    if (name == 0)
        return LookupNamespace0(0, 0);
    else
        return LookupNamespace0(name->GetPosition(), name->GetLength());
}

Environment* Environment::LookupNamespace0(const char* name, int len)
{
    int n = namespaces.Number();
    for (int i = 0; i < n; ++i) {
        Environment* ns = namespaces.Ref(i);
        Ptree* n = ns->namespace_name;
        if (name == 0) {
            if (n == 0)
                return ns;
        }
        else if (n != 0 && PtreeUtil::Eq(n, name, len))
            return ns;
    }

    return 0;
}

void Environment::RecordUsing(Ptree* name)
{
    char* encode = name->GetEncodedName();
    if(encode == 0)
        return;

    int len;
    Environment* e = this;
    char* base = EncodingUtil::GetBaseName(encode, len, e);
    if(base == 0 || e == 0)
        return;

    Bind* bind;
    if (!e->LookupAll(base, len, bind))
        return;

    htable->AddEntry(base, len, bind);
}

void Environment::RecordUsingNamespace(Ptree* name)
{
    char* encode = name->GetEncodedName();
    if(encode == 0)
        return;

    int len;
    Environment* e = this;
    char* base = EncodingUtil::GetBaseName(encode, len, e);
    if(base == 0 || e == 0)
        return;

    Environment* ns = e->LookupNamespace(base, len);
    if (ns != 0 && !IsClassEnvironment())
        AddBaseclassEnv(ns);
}

void Environment::RecordTypedefName(Ptree* decls)
{
    while(decls != 0){
        Ptree* d = decls->Car();
        if(d->What() == ntDeclarator){
            char* name = d->GetEncodedName();
            char* type = d->GetEncodedType();
            if(name != 0 && type != 0){
                int len;
                Environment* e = this;
                name = EncodingUtil::GetBaseName(name, len, e);
                if(name != 0)
                    AddEntry(name, len, new BindTypedefName(type));
            }
        }

        decls = PtreeUtil::ListTail(decls, 2);
    }
}

void Environment::RecordEnumName(Ptree* spec)
{
    Ptree* tag = PtreeUtil::Second(spec);
    char* encoded_name = spec->GetEncodedName();
    if(tag != 0 && tag->IsLeaf())
        AddEntry(tag->GetPosition(), tag->GetLength(),
                 new BindEnumName(encoded_name, spec));
    else{
        int n;
        Environment* e = this;
        char* name = EncodingUtil::GetBaseName(encoded_name, n, e);
        if(name != 0 && e != 0)
            e->AddEntry(name, n, new BindEnumName(encoded_name, spec));
    }
}

void Environment::RecordClassName(char* encoded_name, Class* metaobject)
{
    int n;
    Environment* e;
    Bind* bind;

    e = this;
    char* name = EncodingUtil::GetBaseName(encoded_name, n, e);
    if(name == 0 || e == 0)
        return;         // error?

    if(e->LookupAll(name, n, bind)){
        if(bind != 0 && bind->What() == Bind::isClassName){
            if(metaobject != 0) {
                // if we already have a class of this name in the very same 
                // environment, overwrite the old one (e.g. forwarded class)
                if(!bind->ClassMetaobject() || 
                   bind->ClassMetaobject()->GetEnvironment()->GetOuterEnvironment() == e)
                    bind->SetClassMetaobject(metaobject);
                else
                    e->AddEntry(name, n, new BindClassName(metaobject));
            }

            return;
        }
    }

    e->AddEntry(name, n, new BindClassName(metaobject));
}

void Environment::RecordTemplateClass(Ptree* spec, Class* metaobject)
{
    int n;
    Environment* e;
    Bind* bind;

    e = this;
    char* name = EncodingUtil::GetBaseName(spec->GetEncodedName(), n, e);
    if(name == 0 || e == 0)
        return;         // error?

    if(e->LookupAll(name, n, bind))
        if(bind != 0 && bind->What() == Bind::isTemplateClass){
            if(metaobject != 0)
                bind->SetClassMetaobject(metaobject);

            return;
        }

    e->AddEntry(name, n, new BindTemplateClass(metaobject));
}

Environment* Environment::RecordTemplateFunction(Ptree* def, Ptree* body)
{
    int n;
    Ptree* decl = PtreeUtil::Third(body);
    if(decl->IsA(ntDeclarator)){
        char* name = decl->GetEncodedName();
        if(name != 0){
            Environment* e = this;
            name = EncodingUtil::GetBaseName(name, n, e);
            if(name != 0 && e != 0)
                e->AddEntry(name, n, new BindTemplateFunction(def));

            return e;
        }
    }

    return this;
}

Environment* Environment::RecordDeclarator(Ptree* decl)
{
    if(decl->What() == ntDeclarator){
        char* name = decl->GetEncodedName();
        char* type = decl->GetEncodedType();
        if(name != 0 && type != 0){
            int len;
            Environment* e = this;
            name = EncodingUtil::GetBaseName(name, len, e);

            // allow a duplicated entry because of overloaded functions
            if(name != 0 && e != 0)
                e->AddDupEntry(name, len, new BindVarName(type));

            return e;
        }
    }

    return this;
}

Environment* Environment::DontRecordDeclarator(Ptree* decl)
{
    if(decl->What() == ntDeclarator){
        char* name = decl->GetEncodedName();
        if(name != 0){
            int len;
            Environment* e = this;
            EncodingUtil::GetBaseName(name, len, e);
            return e;
        }
    }

    return this;
}

void Environment::RecordMetaclassName(Ptree* decl)
{
    if(PtreeUtil::Third(decl) != 0)
        metaclasses.Append(decl);
}

Ptree* Environment::LookupMetaclass(Ptree* name)
{
    using namespace PtreeUtil;
    unsigned n = metaclasses.Number();
    for(unsigned i = 0; i < n; ++i){
        Ptree* d = metaclasses[i];
        if(Eq(Third(d), name)) {
            return d;
        }
    }

    return 0;
}

bool Environment::RecordClasskeyword(char* keyword, char* metaclass_name)
{
    Ptree* keywordp = new Leaf(keyword, strlen(keyword));
    Ptree* metaclassp = new Leaf(metaclass_name, strlen(metaclass_name));

    if(LookupClasskeyword(keywordp) == 0){
        classkeywords->Append(keywordp);
        classkeywords->Append(metaclassp);
        return true;
    }
    else
        return false;
}

Ptree* Environment::LookupClasskeyword(Ptree* keyword)
{
    if(classkeywords == 0)
        classkeywords = new PtreeArray;

    unsigned n = classkeywords->Number();
    for(unsigned i = 0; i < n; i += 2){
        Ptree* d = classkeywords->Ref(i);
        if(PtreeUtil::Eq(d, keyword))
            return classkeywords->Ref(i + 1);
    }

    return 0;
}

Class* Environment::LookupThis()
{
    Environment* p;
    for(p = this; p != 0; p = p->next)
        if(p->metaobject != 0)
            return p->metaobject;

    return 0;
}

// IsMember() returns the class environment that the member belongs to.
// If the member is not valid, IsMember() returns 0.

Environment* Environment::IsMember(Ptree* member)
{
    Bind* bind;
    Environment* e;

    if(!member->IsLeaf()){
        char* encode = member->GetEncodedName();
        if(encode != 0){
            int len;
            e = this;
            char* base = EncodingUtil::GetBaseName(encode, len, e);
            if(base != 0 && e != 0 && e->metaobject != 0)
                return e;
        }
    }

    for(e = this; e != 0; e = e->next)
        if(e->metaobject != 0)
            break;
        else if(e->LookupTop(member, bind))
            if(bind != 0 && !bind->IsType())
                return 0;     // the member is overridden.

    if(e != 0 && e->LookupTop(member, bind))
       if(bind != 0 && !bind->IsType())
           return e;

    return 0;
}

void Environment::Dump()
{
    htable->Dump(std::cerr);
    std::cerr << '\n';
}

void Environment::Dump(int level)
{
    Environment* e = this;
    while(level-- > 0)
        if(e->next != 0)
            e = e->next;
        else{
            std::cerr << "Environment::Dump(): the bottom is reached.\n";
            return;
        }

    e->Dump();
}

Ptree* Environment::GetLineNumber(Ptree* p, int& number)
{
    if (walker == 0) {
        number = 0;
        return 0;
    }

    char* fname;
    int fname_len;
    number = (int)walker->GetParser()->LineNumber(p->GetPosition(),
                                                  fname, fname_len);
    return new Leaf(fname, fname_len);
}

Environment::Array::Array(int s)
{
    num = 0;
    size = s;
    if(s > 0) {
        array = new (GC) Environment*[s];
    }
    else {
        array = 0;
    }
}

void Environment::Array::Append(Environment* p)
{
    if(num >= size) {
        size += 8;
        Environment** a = new (GC) Environment*[size];
        memmove(a, array, size_t(num * sizeof(Environment*)));
        delete [] array;
        array = a;
    }

    array[num++] = p;
}

Environment* Environment::Array::Ref(unsigned i)
{
    if(i < num)
        return array[i];
    else
        return 0;
}

}
