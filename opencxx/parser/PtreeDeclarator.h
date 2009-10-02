#ifndef guard_opencxx_ptree_PtreeDeclarator_h
#define guard_opencxx_ptree_PtreeDeclarator_h

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

class Ptree;
class Encoding;

class PtreeDeclarator : public NonLeaf {
public:
    PtreeDeclarator(Ptree* list, char* typeEnc,
                    char* nameEnc, Ptree* declaredName);
                    
    PtreeDeclarator(char*, char*, Ptree*);
    
    PtreeDeclarator(Ptree*, Ptree*, char*, char*, Ptree*);

    PtreeDeclarator(Ptree*, char*);
    
    PtreeDeclarator(char* encType);
    
    PtreeDeclarator(PtreeDeclarator*, Ptree*, Ptree*);

    int What();
    char* GetEncodedType();
    char* GetEncodedName();
    Ptree* Name() { return declared_name; }
    void Print(std::ostream&, int, int);

    Ptree* GetComments() { return comments; }
    void SetComments(Ptree* c) { comments = c; }

OPENCXX_DEPRECATED_PUBLIC:

    PtreeDeclarator(Ptree* list, Encoding& typeEnc, 
                    Encoding& nameEnc, Ptree* declaredName);
    PtreeDeclarator(Encoding&, Encoding&, Ptree*);
    PtreeDeclarator(Ptree*, Ptree*, Encoding&, Encoding&, Ptree*);
    PtreeDeclarator(Ptree*, Encoding&);
    PtreeDeclarator(Encoding& encType);

private:
    char* type;
    char* name;
    Ptree* declared_name;
    Ptree* comments;
};

}

#endif /* ! guard_opencxx_ptree_PtreeDeclarator_h */
