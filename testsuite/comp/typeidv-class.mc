//(Automatic CVS control) $Revision: 1.6.2.1 $ $Date: 2004/07/29 20:11:51 $

/*
------------------------------------------------------------------------
Copyright (C) 2001 Other Contributors.

Permission to use, copy, distribute and modify this software and its
documentation for any purpose is hereby granted without fee, provided that
the above copyright notice appear in all copies and that both that copyright
notice and this permission notice appear in supporting documentation.

Other Contributors make no representations about the suitability of this
software for any purpose. It is provided "as is" without express or implied
warranty.

Other Contributors are: Grzegorz Jakacki.
------------------------------------------------------------------------
*/
#include <iostream>
#include <opencxx/mop.h>

using namespace std;
using namespace Opencxx;

class TestClass 
    : public Class 
{
public:
    static bool Initialize();
    virtual Ptree* TranslateMemberCall(Environment* env, Ptree* object,
                                       Ptree* op, Ptree* member, Ptree* arglist);
    virtual Ptree* TranslatePostfix(Environment* env, Ptree* object,
                                    Ptree* post_op);
};

bool 
TestClass::Initialize() 
{
    ChangeDefaultMetaclass("TestClass");
    return true;
}

Ptree* 
TestClass::TranslateMemberCall(Environment* env, Ptree* object,
                               Ptree* op, Ptree* member, Ptree* arglist)
{
    cout << object << op << member << arglist << endl;
    return Class::TranslateMemberCall(env, member, arglist);
}

Ptree* 
TestClass::TranslatePostfix(Environment* env, Ptree* object,
                                Ptree* post_op)
{
    cout << object << post_op << endl;
    return Class::TranslatePostfix(env, object, post_op);
}
