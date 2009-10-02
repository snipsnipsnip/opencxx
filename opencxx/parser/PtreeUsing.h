#ifndef guard_opencxx_ptree_PtreeUsing_h
#define guard_opencxx_ptree_PtreeUsing_h

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

class Ptree;
class AbstractTranslatingWalker;
class Encoding;

class PtreeUsing : public NonLeaf {
public:
    PtreeUsing(Ptree* p, Ptree* q, Ptree* r, char* encName, Ptree* s);
    int What();
    Ptree* Translate(AbstractTranslatingWalker*);

    /* the encoded name of the symbol or namespace specified
     * by this using declaration.
     */
    char* GetEncodedName();

    /* true if this using declaration specifies a namespace.
     */
    bool isNamespace() { return bool(Cdr()->Car() != 0); }

OPENCXX_DEPRECATED_PUBLIC:

    PtreeUsing(Ptree* p, Ptree* q, Ptree* r, Encoding& encName, Ptree* s);

private:
    char* name;
};

}

#endif /* ! guard_opencxx_ptree_PtreeUsing_h */
