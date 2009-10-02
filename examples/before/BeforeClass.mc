/*
  Copyright (C) 1997,1998 Shigeru Chiba, University of Tsukuba.

  Permission to use, copy, distribute and modify this software and   
  its documentation for any purpose is hereby granted without fee,        
  provided that the above copyright notice appear in all copies and that 
  both that copyright notice and this permission notice appear in 
  supporting documentation.

  Shigeru Chiba makes no representations about the suitability of this 
  software for any purpose.  It is provided "as is" without express or
  implied warranty.

  July 1997: rewritten by Toru Takimoto for version 2.5.
*/
/*
  Copyright (c) 1995, 1996 Xerox Corporation.
  All Rights Reserved.

  Use and copying of this software and preparation of derivative works
  based upon this software are permitted. Any copy of this software or
  of any derivative work must include the above copyright notice of
  Xerox Corporation, this paragraph and the one after it.  Any
  distribution of this software or derivative works must comply with all
  applicable United States export control laws.

  This software is made available AS IS, and XEROX CORPORATION DISCLAIMS
  ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
  PURPOSE, AND NOTWITHSTANDING ANY OTHER PROVISION CONTAINED HEREIN, ANY
  LIABILITY FOR DAMAGES RESULTING FROM THE SOFTWARE OR ITS USE IS
  EXPRESSLY DISCLAIMED, WHETHER ARISING IN CONTRACT, TORT (INCLUDING
  NEGLIGENCE) OR STRICT LIABILITY, EVEN IF XEROX CORPORATION IS ADVISED
  OF THE POSSIBILITY OF SUCH DAMAGES.
*/

#include <opencxx/mop.h>

using namespace Opencxx;

/*
  If the metaclass is BeforeClass, if a member function f() is called,
  it is preceded by before_f().  If there is not before_f(), only f()
  is called.
*/

class BeforeClass : public Class {
public:
    Ptree* TranslateMemberCall(Environment*, Ptree*, Ptree*, Ptree*,
			       Ptree*);
    Ptree* TranslateMemberCall(Environment*, Ptree*, Ptree*);

private:
    Ptree* BeforeFunction(Ptree* name);
};

// BeforeFunction() receives a member NAME and looks for a before_NAME()
// member function.  If it finds, it returns before_NAME.

Ptree* BeforeClass::BeforeFunction(Ptree* name)
{
    Ptree* before = Ptree::Make("before_%p", name);
    Member member;
    if(LookupMember(before))
	return before;
    else
	return nil;
}

/*
  TranslateMemberCall() translates "object.member(arg, ...)" into
  "((tmp = &object)->before_member(), tmp->member(arg, ...)" if there
  is before_member() in the class.

  If tmp has not been declared yet, the function inserts the declaration
  by calling InsertDeclaration().
*/
Ptree* BeforeClass::TranslateMemberCall(Environment* env, Ptree* object,
					Ptree* op, Ptree* member,
					Ptree* arglist)
{
    Ptree* before_func = BeforeFunction(member);

    if(before_func == nil)
	return Class::TranslateMemberCall(env, object, op, member, arglist);
    else{
	Ptree* class_name = Name();
	Ptree *tmpvar = (Ptree*)LookupClientData(env, class_name);
	if(tmpvar == nil) {
	    tmpvar = PtreeUtil::GenSym();
	    Ptree* decl = Ptree::qMake("`class_name`* `tmpvar`;");
	    InsertDeclaration(env, decl, class_name, tmpvar);
	}

	return Ptree::Make("((%p=%c%p)->%p(), %p->%p%p)",
			   tmpvar, (op->Eq('.') ? '&' : ' '), object,
			   before_func, tmpvar, member, arglist);
    }
}

Ptree* BeforeClass::TranslateMemberCall(Environment* env, Ptree* member,
					Ptree* arglist)
{
    Ptree* before = BeforeFunction(member);
    Ptree* expr = Class::TranslateMemberCall(env, member, arglist);
    if(before == nil)
	return expr;
    else
	return Ptree::qMake("(`before`(), `expr`)");
}
