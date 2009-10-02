#ifndef guard_opencxx_ptree_PtreeBrace_h
#define guard_opencxx_ptree_PtreeBrace_h

//@beginlicenses@
//@license{chiba_tokyo}{}@
//@license{contributors}{}@
//
//  Permission to use, copy, distribute and modify this software and its  
//  documentation for any purpose is hereby granted without fee, provided that
//  the above copyright notice appears in all copies and that both that copyright
//  notice and this permission notice appear in supporting documentation.
// 
//  1997-2001 Shigeru Chiba, Tokyo Institute of Technology. make(s) no representations about the suitability of this
//  software for any purpose. It is provided "as is" without express or implied
//  warranty.
//  
//  Copyright (C)  1997-2001 Shigeru Chiba, Tokyo Institute of Technology.
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

#include <opencxx/parser/NonLeaf.h>

namespace Opencxx
{

class AbstractTranslatingWalker;

class PtreeBrace : public NonLeaf {
public:
    PtreeBrace(Ptree* p, Ptree* q) : NonLeaf(p, q) {}
    PtreeBrace(Ptree* ob, Ptree* body, Ptree* cb)
	: NonLeaf(ob, new NonLeaf(body, new NonLeaf(cb, 0))) {}

    void Print(std::ostream&, int, int);
    int Write(std::ostream&, int);

    Ptree* Translate(AbstractTranslatingWalker*);
};

}

#endif /* ! guard_opencxx_ptree_PtreeBrace_h */
