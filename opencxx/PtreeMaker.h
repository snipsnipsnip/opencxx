#ifndef guard_opencxx_PtreeMaker_h
#define guard_opencxx_PtreeMaker_h

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

/** Helps to implement Ptree::qMake()
  */
class PtreeMaker {
public:
    /** Marks end of construction */
    class End {};
    
    PtreeMaker() : ptree_(0) {}
    /** @name Appenders
        Append respective objects to tree being constructed
      */
    /*@{*/
    PtreeMaker& operator << (Ptree*);
    PtreeMaker& operator << (const char*);
    PtreeMaker& operator << (char*);
    PtreeMaker& operator << (char);
    PtreeMaker& operator << (int);
    /*@}*/
    
    /** Concludes the constructions, returns the constructed object */
    Ptree* operator << (End)
    { 
        Ptree* tmp = ptree_;
        ptree_ = 0;
        return tmp; 
    }

private:
    static Ptree* Append(Ptree*, Ptree*);
    static Ptree* Append(Ptree*, char*, int);

private:
    Ptree* ptree_;
};

}

#endif /* ! guard_opencxx_PtreeMaker_h */
