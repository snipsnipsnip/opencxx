//(Automatic CVS control) $Revision: 1.9.2.1 $ $Date: 2004/07/29 20:07:34 $

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

class TstartClass : public Class 
{
public:
    static bool Initialize();
    Ptree* TranslateMemberRead(Environment* env, Ptree* object, 
                               Ptree* op, Ptree* member);
};

bool
TstartClass::Initialize()
{
    ChangeDefaultMetaclass("TstartClass");
    return true;
}

Ptree*
TstartClass::TranslateMemberRead(Environment* env, Ptree* object,
                                    Ptree* op, Ptree* member)
{
    cout << "CALLED TranslateMemberRead()" << endl;
    return Class::TranslateMemberRead(env, object, op, member);
}
