#ifndef guard_opencxx_ChangedMemberList_h
#define guard_opencxx_ChangedMemberList_h

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

namespace Opencxx
{

class Ptree;
class Member;


class ChangedMemberList : public LightObject {
public:
    struct Cmem {
	Ptree* declarator;
	bool removed;
	Ptree* name;
	Ptree* args;
	Ptree* init;
	Ptree* body;
	Ptree* def;
	int    access;		// used only by Classs::appended_member_list
	bool arg_name_filled;
    };

    ChangedMemberList();
    void Append(Member*, int access);
    static void Copy(Member* src, Cmem* dest, int access);
    Cmem* Lookup(Ptree* decl);
    Cmem* Get(int);

private:
    Cmem* Ref(int);

private:
    int num;
    int size;
    Cmem* array;
};

}

#endif /* ! guard_opencxx_ChangedMemberList_h */
