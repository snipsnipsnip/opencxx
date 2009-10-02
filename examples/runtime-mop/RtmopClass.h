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

/*
  RtmopClass implements a runtime MOP similar to OpenC++ version 1.
  The programmer can define runtime metaobjects to control objects.
  In this example, we deal with three levels: compile-time meta,
  runtime meta, and base.

  See also "metaobj.h" and "metaobj-test.cc".
*/

#ifndef __rtmop_h
#define __rtmop_h

#include <opencxx/mop.h>

// Compile-time Meta Level

class RtmopClass : public Opencxx::Class {
public:
    void InitializeInstance(Opencxx::Ptree* def, Opencxx::Ptree* margs);
    void TranslateClass(Opencxx::Environment*);

    Opencxx::Ptree* MakeDispatcher();
    Opencxx::Ptree* LoadArgument(int, Opencxx::TypeInfo&);
    Opencxx::Ptree* StoreReterunValue(Opencxx::TypeInfo&, Opencxx::Ptree*, int);

    Opencxx::Ptree* TranslateMemberCall(
        Opencxx::Environment*, Opencxx::Ptree*, Opencxx::Ptree*, 
        Opencxx::Ptree*, Opencxx::Ptree*);
    Opencxx::Ptree* StoreArgument(
        Opencxx::Environment*, Opencxx::TypeInfo&, Opencxx::Ptree*);

private:
    Opencxx::Ptree* rt_metaclass;
};

#endif  /* __rtmop_h */
