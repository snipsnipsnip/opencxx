//(Automatic CVS control) $Revision: 1.3.2.1 $ $Date: 2004/07/29 20:20:17 $

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

Other Contributors are: Marcus Bartholomeu.
------------------------------------------------------------------------
*/

#include "opencxx/mop.h"

using namespace Opencxx;

class VerboseTemplateClass : public TemplateClass {
public:
    Ptree* TranslateMemberCall(Environment*, Ptree*, Ptree*,
                               Ptree*, Ptree*);
    Ptree* TranslateMemberRead(Environment*, Ptree*, Ptree*,
                               Ptree*);
};

Ptree* VerboseTemplateClass::TranslateMemberCall(Environment* env,
        Ptree* object, Ptree* op, Ptree* member, Ptree* arglist)
{
    return Ptree::Make("(cout << \"(%p%p%p%p)\", %p)",
		       object, op, member, arglist,
		       TemplateClass::TranslateMemberCall(env, object, op,
						  member, arglist));
}

Ptree* VerboseTemplateClass::TranslateMemberRead(Environment* env,
        Ptree* object, Ptree* op, Ptree* member)
{
    return Ptree::Make("(cout << \"(%p%p%p)\", %p)",
		       object, op, member,
		       TemplateClass::TranslateMemberRead(env, object, op,
						  member));
}
