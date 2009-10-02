#ifndef guard_opencxx_Member_h
#define guard_opencxx_Member_h

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

class Ptree;
class Class;
class TypeInfo;
class PtreeArray;

class OCXXMOP Member : public LightObject {
public:
    Member();
    Member(const Member&);
    Member(Class*, Ptree*);
    void Set(Class*, Ptree*, int);

    void Signature(TypeInfo& t) const;
    Ptree* Name();
    Ptree* Comments();
    int Nth();
    Class* Supplier();
    bool IsConstructor();
    bool IsDestructor();
    bool IsFunction();
    bool IsPublic();
    bool IsProtected();
    bool IsPrivate();
    bool IsStatic();
    bool IsMutable();
    bool IsInline();
    bool IsVirtual();
    bool IsPureVirtual();

    Ptree* GetUserMemberModifier();
    Ptree* GetUserAccessSpecifier();
    bool GetUserArgumentModifiers(PtreeArray& result);

    void Remove() { removed = true; }
    void SetName(Ptree*);
    void SetQualifiedName(Ptree*);
    Ptree* NewName() { return new_name; }
    Ptree* ArgumentList();
    void SetArgumentList(Ptree*);
    Ptree* NewArgumentList() { return new_args; }
    Ptree* MemberInitializers();
    void SetMemberInitializers(Ptree*);
    Ptree* NewMemberInitializers() { return new_init; }
    Ptree* FunctionBody();
    void SetFunctionBody(Ptree*);
    Ptree* NewFunctionBody() { return new_body; }

    Ptree* Arguments();

    static void Copy(Member*, void* /* ChangedMemberList::Cmem* */);

protected:
    bool IsInlineFuncImpl();
    void SetName(Ptree*, Ptree*);
    Ptree* ArgumentList(Ptree* decl);
    Ptree* Arguments(Ptree*, int);
    Ptree* MemberInitializers(Ptree* decl);

private:
    char* Name(int&);
    bool Find();

protected:

    // The next function is true if the member is a function
    // with the implementation but it is not inlined.  And if so,
    // the following variables are effective.

    bool IsFunctionImplementation() { return bool(implementation != 0); }
    Ptree *implementation;
    Ptree* original_decl;

private:
    bool removed;
    Ptree* new_name;
    Ptree* new_args;
    Ptree* new_init;
    Ptree* new_body;
    bool arg_name_filled;

    Class* metaobject;
    Ptree* declarator;
    int nth;

friend class ChangedMemberList;
};

}

#endif /* ! guard_opencxx_Member_h */
