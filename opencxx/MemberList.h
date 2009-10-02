#ifndef guard_opencxx_MemberList_h
#define guard_opencxx_MemberList_h

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
#include <opencxx/defs.h>

namespace Opencxx
{

class Class;
class Ptree;
class Environment;

class OCXXMOP MemberList : public LightObject {
public:
    struct Mem {
	Class* supplying;
	Ptree* definition;
	Ptree* declarator;
	char* name;
	char* signature;
	bool is_constructor, is_destructor;
	bool is_virtual, is_static, is_mutable, is_inline;
	int  access;
	Ptree* user_access;
	Ptree* user_mod;
    };

    MemberList();
    void Make(Class*);
    Mem* Ref(int);
    int Number() { return num; }
    Mem* Lookup(char*, char*);
    int Lookup(char*, int, char*);
    int Lookup(Environment*, Ptree*, int);
    int Lookup(Environment*, char*, int);

private:
    void AppendThisClass(Class*);
    void Append(Ptree*, Ptree*, int, Ptree*);
    void AppendBaseClass(Environment*, Ptree*);
    void CheckHeader(Ptree*, Mem*);

    Class* this_class;
    int num;

    int size;
    Mem* array;
};

}


#endif /* ! guard_opencxx_MemberList_h */
