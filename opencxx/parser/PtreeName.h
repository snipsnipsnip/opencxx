#ifndef guard_opencxx_ptree_PtreeName_h
#define guard_opencxx_ptree_PtreeName_h

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

#include <iosfwd>
#include <opencxx/parser/NonLeaf.h>

namespace Opencxx
{

class Encoding;
class Ptree;
class AbstractTranslatingWalker;
class TypeInfo;

class PtreeName : public NonLeaf {
public:
    PtreeName(Ptree*, char* encodedName);
    int What();
    char* GetEncodedName();
    void Print(std::ostream&, int, int);
    Ptree* Translate(AbstractTranslatingWalker*);
    void Typeof(AbstractTypingWalker*, TypeInfo&);

OPENCXX_DEPRECATED_PUBLIC:

    PtreeName(Ptree*, Encoding&);

private:
    char* name;
};

}

#endif /* ! guard_opencxx_ptree_PtreeName_h */
