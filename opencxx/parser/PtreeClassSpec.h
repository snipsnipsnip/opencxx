#ifndef guard_opencxx_ptree_PtreeClassSpec_h
#define guard_opencxx_ptree_PtreeClassSpec_h

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

class PtreeClassSpec : public NonLeaf {
public:
    PtreeClassSpec(Ptree*, Ptree*, Ptree*);
    PtreeClassSpec(Ptree*, Ptree*, Ptree*, char*);
    int What();
    Ptree* Translate(AbstractTranslatingWalker*);
    char* GetEncodedName();
    Ptree* GetComments();

private:
    char* encoded_name;
    Ptree* comments;

friend class Parser;
};

}


#endif /* ! guard_opencxx_ptree_PtreeClassSpec_h */
