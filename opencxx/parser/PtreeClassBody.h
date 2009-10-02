#ifndef guard_opencxx_ptree_PtreeClassBody_h
#define guard_opencxx_ptree_PtreeClassBody_h

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

#include <opencxx/parser/PtreeBrace.h>

namespace Opencxx
{

class AbstractTranslatingWalker;

class PtreeClassBody : public PtreeBrace {
public:
    PtreeClassBody(Ptree* p, Ptree* q) : PtreeBrace(p, q) {}
    PtreeClassBody(Ptree* ob, Ptree* bdy, Ptree* cb)
						: PtreeBrace(ob, bdy, cb) {}

    Ptree* Translate(AbstractTranslatingWalker*);
};

}

#endif /* ! guard_opencxx_ptree_PtreeClassBody_h */
