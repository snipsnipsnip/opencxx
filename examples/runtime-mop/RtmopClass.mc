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

// Compile-time Meta Level


#include "RtmopClass.h"
#include <opencxx/PtreeIter.h>
#include <cstring>
#include <cassert>

using namespace Opencxx;

#define BUF_NAME  "mBuffer"
#define INDEX_VAR "mIndex"
#define METAOBJ   "MetaObj"
#define DISPATCH  "Reify"
#define TRAP      "Reflect"

void RtmopClass::InitializeInstance(Ptree* def, Ptree* meta)
{
    Ptree* name;

    Class::InitializeInstance(def, NULL);

    if(PtreeUtil::Match(meta, "[%?]", &name))
	rt_metaclass = name;
    else
	rt_metaclass = NULL;
}

void RtmopClass::TranslateClass(Environment* env)
{
    // all classes must inherit from METAOBJ.
    if(BaseClasses() == NULL)
	if(rt_metaclass == NULL)
	    AppendBaseClass(METAOBJ);
	else
	    AppendBaseClass(rt_metaclass);
    else{
	Class* baseclass = NthBaseClass(0);
	if(baseclass == 0
	   || strcmp(baseclass->MetaclassName(), this->MetaclassName()) != 0)
	    assert(! "The base class is not RtmopClass.");
    }

    // declare a dispatch function.
    AppendMember(Ptree::Make("private: void " DISPATCH "(int, char*);"));

    // make the dispatch function.
    Ptree* dispatcher = MakeDispatcher();
    AppendAfterToplevel(env, Ptree::qMake(
	"void `Name()`::" DISPATCH "(int member, char* buffer){\n"
	"    int i = 0;\n"
	"    switch(member){\n"
        "    `dispatcher` }}"));

}

Ptree* RtmopClass::MakeDispatcher()
{
    Member member;
    Ptree* dispatcher = NULL;
    int i = 0;
    while(NthMember(i++, member))
	if(member.IsFunction()
	   && !member.IsConstructor() && !member.IsDestructor()){
	    TypeInfo ftype;
	    TypeInfo argtype;

	    member.Signature(ftype);
	    PtreeArray loader;
	    int i;
	    for(i = 0; ftype.NthArgument(i, argtype); ++i)
		loader.Append(LoadArgument(i, argtype));

	    Ptree* return_val = StoreReterunValue(ftype, member.Name(), i);
	    dispatcher = Ptree::qMake(
		"`dispatcher` case `member.Nth()`:{\n"
		"`loader.All()`"
		"`return_val`}\n"
		"break;\n");
	}

    return dispatcher;
}

Ptree* RtmopClass::LoadArgument(int n, TypeInfo& argtype)
{
    Ptree* code;
    TypeInfoId whatis = argtype.WhatIs();
    if(whatis == BuiltInType)
	if(argtype.IsBuiltInType() & (FloatType | DoubleType))
	    return Ptree::Make("double p%d = *(double*)&buffer[i];"
			       "i+=sizeof(double);\n",
			       n);
	else
	    return Ptree::Make("int p%d = *(int*)&buffer[i];"
			       "i+=sizeof(int);\n",
			       n);
    else if(whatis == ClassType)
	return Ptree::Make("%p p%d(&buffer[i], i);\n",
			   argtype.ClassMetaobject()->Name(), n);
    else if(whatis == PointerType){
	Ptree* tname = argtype.MakePtree(NULL);
	return Ptree::Make("%p p%d = *(%p*)&buffer[i];"
			   "i+=sizeof(%p);\n",
			   tname, n, tname, tname);
    }
    else{
        assert(! "sorry, cannot handle this type");
    }
}

Ptree* RtmopClass::StoreReterunValue(TypeInfo& ftype, Ptree* member_name,
				     int nargs)
{
    TypeInfo rtype;
    Ptree* code;

    ftype.Dereference(rtype);
    TypeInfoId whatis = rtype.WhatIs();
    if(whatis == BuiltInType){
	unsigned int tag = rtype.IsBuiltInType();
	if(tag & (FloatType | DoubleType))
	    code = Ptree::Make("*(double*)buffer = %p(",
			       member_name);
	else if(tag & VoidType)
	    code = Ptree::Make("%p(", member_name);
	else
	    code = Ptree::Make("*(int*)buffer = %p(", member_name);
    }
    else if(whatis == ClassType)
	code = Ptree::Make("%p(", member_name);
    else if(whatis == PointerType)
	code = Ptree::Make("*(%p*)buffer = %p(",
			   rtype.MakePtree(NULL), member_name);
    else if(whatis == UndefType)
	code = Ptree::Make("*(int*)buffer = %p(", member_name);
    else{
        assert(! "sorry, cannot handle this type");
    }

    if(nargs > 0){
	Ptree* args = Ptree::Make("p%d", nargs-1);
	while(--nargs > 0)
	    args = Ptree::Make("p%d,%p", nargs-1, args);

	code = Ptree::List(code, args);
    }

    if(whatis == ClassType)
	return Ptree::Make("%p).Store(buffer, i);", code);
    else
	return Ptree::Make("%p);", code);
}

Ptree* RtmopClass::TranslateMemberCall(Environment* env, Ptree* object,
				       Ptree* op, Ptree* member,
				       Ptree* arglist)
{
    TypeInfo ftype;
    TypeInfo argtype;
    TypeInfo rtype;
    unsigned int tag;

    if(!LookupMemberType(member, ftype)){
        assert(! "no such member");
    }

    PtreeArray stub;
    PtreeIter next(Ptree::Second(arglist));
    for(int i = 0; ftype.NthArgument(i, argtype); ++i){
	stub.Append(StoreArgument(env, argtype, next()));
	next();		// skip ,
    }

    Ptree* code = Ptree::Make("(" INDEX_VAR "= 0, %p "
			      "%p%p" TRAP "(%d," BUF_NAME "," INDEX_VAR "),",
			      stub.All(), TranslateExpression(env, object),
			      op, IsMember(member));

    ftype.Dereference(rtype);
    switch(rtype.WhatIs()){
    case BuiltInType :
	tag = rtype.IsBuiltInType();
	if(tag & (FloatType | DoubleType))
	    code = Ptree::Make("%p *(double*)" BUF_NAME ")", code);
	else if(tag & VoidType)
	    code = Ptree::Make("%p 0)", code);
	else
	    code = Ptree::Make("%p *(int*)" BUF_NAME ")", code);
	break;
    case ClassType :
	code = Ptree::Make("%p %p(" BUF_NAME "))",
			   code,
			   rtype.ClassMetaobject()->Name());
	break;
    case PointerType :
	code = Ptree::Make("%p *(%p*)" BUF_NAME ")",
			   code, rtype.MakePtree(NULL));
	break;
    case UndefType :
	code = Ptree::Make("%p *(int*)" BUF_NAME ")", code);
	break;
    default :
	code = Ptree::Make("%p 0)", code);	// error
	break;
    }

    return code;
}

Ptree* RtmopClass::StoreArgument(Environment* env, TypeInfo& argtype,
				 Ptree* expr)
{
    Ptree* t;
    expr = TranslateExpression(env, expr);
    switch(argtype.WhatIs()){
    case BuiltInType :
	if(argtype.IsBuiltInType() & (FloatType | DoubleType))
	    return Ptree::Make("*(double*)&" BUF_NAME "[" INDEX_VAR "]= %p,"
			       INDEX_VAR "+=sizeof(double),",
			       expr);
	else
	    return Ptree::Make("*(int*)&" BUF_NAME "[" INDEX_VAR "]= %p,"
			       INDEX_VAR "+=sizeof(int),",
			       expr);
    case ClassType :
	return Ptree::Make("(%p).Store(" BUF_NAME "," INDEX_VAR "),",
			   expr);
    case PointerType :
	t = argtype.MakePtree(NULL);
	return Ptree::Make("*(%p*)&" BUF_NAME "[" INDEX_VAR "]= %p,"
			   INDEX_VAR "+=sizeof(%p),",
			   t, expr, t);
    default :
	return NULL;
    }
}
