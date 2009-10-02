#ifndef guard_opencxx_Environment_h
#define guard_opencxx_Environment_h

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
#include <opencxx/parser/Ptree.h>
#include <opencxx/parser/PtreeArray.h>

namespace Opencxx
{

class Walker;
class Environment;
class Ptree;
class Bind;
class HashTable;
class Class;
class TypeInfo;

class Environment : public LightObject {
public:
    Environment(Walker* w);
    Environment(Environment* e);
    Environment(Environment* e, Walker* w);

    bool IsEmpty();
    Environment* GetOuterEnvironment() { return next; }
    Environment* GetBottom();
    void AddBaseclassEnv(Environment* e) { baseclasses_or_using.Append(e); }
    Walker* GetWalker() { return walker; }
    void SetWalker(Walker* w) { walker = w; }

    Class* LookupClassMetaobject(Ptree* name);
    bool LookupType(const char* name, int len, Bind*& t);

    bool Lookup(Ptree* name, bool& is_type_name, TypeInfo& t);
    bool Lookup(Ptree* name, TypeInfo& t);
    bool Lookup(Ptree*, Bind*&);
    bool LookupTop(Ptree*, Bind*&);

    bool LookupTop(const char* name, int len, Bind*& t);
    bool LookupAll(const char* name, int len, Bind*& t);

    bool RecordVariable(char* name, Class* c);
    bool RecordPointerVariable(char* name, Class* c);

    int AddEntry(char*, int, Bind*);
    int AddDupEntry(char*, int, Bind*);

    void RecordNamespace(Environment*, Ptree*);
    Environment* LookupNamespace(const char* name, int len);
    Environment* LookupNamespace0(Ptree*);
    void RecordUsing(Ptree* name);
    void RecordUsingNamespace(Ptree* name);

    void RecordTypedefName(Ptree*);
    void RecordEnumName(Ptree*);
    void RecordClassName(char*, Class*);
    void RecordTemplateClass(Ptree*, Class*);
    Environment* RecordTemplateFunction(Ptree*, Ptree*);
    Environment* RecordDeclarator(Ptree*);
    Environment* DontRecordDeclarator(Ptree*);
    void RecordMetaclassName(Ptree*);
    Ptree* LookupMetaclass(Ptree*);
    static bool RecordClasskeyword(char*, char*);
    static Ptree* LookupClasskeyword(Ptree*);

    void SetMetaobject(Class* m) { metaobject = m; }
    Class* IsClassEnvironment() { return metaobject; }
    Ptree* IsNamespace() { return namespace_name; }
    Class* LookupThis();
    Environment* IsMember(Ptree*);

    void Dump();
    void Dump(int);

    Ptree* GetLineNumber(Ptree*, int&);

private:
    static bool SearchBaseOrUsing(Environment* this_env,
			bool (Environment::*func)(const char*, int, Bind*&),
			const char* name, int len, Bind*& t);
    Environment* LookupNamespace0(const char*, int);

public:
    class Array : public LightObject {
    public:
	Array(int = 2);
	unsigned Number() { return num; }
	Environment* Ref(unsigned index);
	void Append(Environment*);
    private:
	unsigned num, size;
	Environment** array;
    };

private:
    Environment*	next;
    HashTable*		htable;
    Class*		metaobject;
    Walker*		walker;
    PtreeArray		metaclasses;
    static PtreeArray*	classkeywords;
    Array		baseclasses_or_using;
			// baseclasses or using_namespaces
    Array		namespaces;
    Ptree*		namespace_name;  // 0 unless this is a namespace
};

}


#endif /* ! guard_opencxx_Environment_h */
