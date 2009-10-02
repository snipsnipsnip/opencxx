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
//@beginlicenses@
//@license{xerox}{}@
//@endlicenses@

#include <opencxx/mop.h>

class WrapperClass : public Opencxx::Class {
public:
    void TranslateClass(Opencxx::Environment* env);
    void TranslateMemberFunction(Opencxx::Environment* env, Opencxx::Member& member);

    Opencxx::Ptree* NewMemberName(Opencxx::Ptree* name);
    void MakeWrapper(Opencxx::Member& member, Opencxx::Ptree* org_name);
    virtual Opencxx::Ptree* MakeWrapperBody(
        Opencxx::Member& member, Opencxx::Ptree* org_name);
    Opencxx::Ptree* ExprToCallOriginal(
        Opencxx::Member& member, Opencxx::Ptree* org_name);
};

