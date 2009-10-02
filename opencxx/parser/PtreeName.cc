//@beginlicenses@
//@license{chiba_tokyo}{}@
//@license{chiba_tsukuba}{}@
//@license{chiba_tokyo}{}@
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
//  1997-2001 Shigeru Chiba, University of Tsukuba. make(s) no representations about the suitability of this
//  software for any purpose. It is provided "as is" without express or implied
//  warranty.
//  
//  Copyright (C)  1997-2001 Shigeru Chiba, University of Tsukuba.
//
//  -----------------------------------------------------------------
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
//@endlicenses@

#include <opencxx/parser/PtreeName.h>
#include <opencxx/parser/AbstractTranslatingWalker.h>
#include <opencxx/parser/AbstractTypingWalker.h>
#include <opencxx/parser/Encoding.h>
#include <opencxx/parser/token-names.h>

namespace Opencxx
{

PtreeName::PtreeName(Ptree* p, Encoding& e)
  : NonLeaf(p->Car(), p->Cdr())
{
    Deprecated("PtreeName::PtreeName(Ptree*,Encoding&)", 
               "PtreeName(ptree,encoding.Get())");
    name = e.Get();
}

PtreeName::PtreeName(Ptree* p, char* encodedName)
  : NonLeaf(p->Car(), p->Cdr())
  , name(encodedName)
{
}

int PtreeName::What()
{
    return ntName;
}

char* PtreeName::GetEncodedName()
{
    return name;
}

void PtreeName::Print(std::ostream& s, int i, int d)
{
    NonLeaf::Print(s, i, d);
}

Ptree* PtreeName::Translate(AbstractTranslatingWalker* w)
{
    return w->TranslateVariable(this);
}

void PtreeName::Typeof(AbstractTypingWalker* w, TypeInfo& t)
{
    w->TypeofVariable(this, t);
}

}
