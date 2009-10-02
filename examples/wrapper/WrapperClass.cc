/* -*-Mode: C++;-*-

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

#include "WrapperClass.h"

using namespace Opencxx;

void WrapperClass::TranslateClass(Environment* env)
{
    Member member;
    int i = 0;
    while(NthMember(i++, member))
	if(member.IsPublic() && member.IsFunction()
	   && !member.IsConstructor() && !member.IsDestructor()){
	    Member wrapper = member;
	    Ptree* org_name = NewMemberName(member.Name());
	    member.SetName(org_name);
	    ChangeMember(member);
	    MakeWrapper(wrapper, org_name);
	    AppendMember(wrapper, Class::Public);
	}
}

void WrapperClass::TranslateMemberFunction(Environment* env, Member& member)
{
    if(member.IsPublic() && !member.IsConstructor()
       && !member.IsDestructor())
	member.SetName(NewMemberName(member.Name()));
}

Ptree* WrapperClass::NewMemberName(Ptree* name)
{
    return Ptree::qMake("_org_`name`");
}

void WrapperClass::MakeWrapper(Member& member, Ptree* org_name)
{
    Ptree* body = MakeWrapperBody(member, org_name);
    member.SetFunctionBody(Ptree::qMake("{ `body` }\n"));
}

Ptree* WrapperClass::MakeWrapperBody(Member& member, Ptree* org_name)
{
    TypeInfo t;

    Ptree* call_expr = ExprToCallOriginal(member, org_name);
    member.Signature(t);
    t.Dereference();		// get the return type

    if((t.IsBuiltInType() & VoidType) || t.IsNoReturnType())
	return Ptree::qMake("`call_expr`;\n");
    else{
	Ptree* rvar = Ptree::Make("_rvalue");
	Ptree* rvar_decl = t.MakePtree(rvar);
	return Ptree::qMake("`rvar_decl` = `call_expr`;\n"
			    "return `rvar`;\n");
    }
}

Ptree* WrapperClass::ExprToCallOriginal(Member& member, Ptree* org_name)
{
    return Ptree::qMake("`org_name`(`member.Arguments()`)");
}
