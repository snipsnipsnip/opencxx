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

#include <opencxx/parser/PtreeFstyleCastExpr.h>
#include <opencxx/parser/AbstractTranslatingWalker.h>
#include <opencxx/parser/AbstractTypingWalker.h>
#include <opencxx/parser/token-names.h>
#include <opencxx/parser/Encoding.h>
#include <opencxx/parser/deprecated.h>

namespace Opencxx
{

PtreeFstyleCastExpr::PtreeFstyleCastExpr(Encoding& encType, Ptree* p, Ptree* q)
  : NonLeaf(p, q)
{
    Deprecated("PtreeFstyleCastExpr::PtreeFstyleCastExpr(Encoding& encType, Ptree* p, Ptree* q)",
               "PtreeFstyleCastExpr(encType.Get(), p, q)");
    type = encType.Get();
}

PtreeFstyleCastExpr::PtreeFstyleCastExpr(char* encType, Ptree* p, Ptree* q)
  : NonLeaf(p, q)
  , type(encType)
{
}

int PtreeFstyleCastExpr::What()
{
    return ntFstyleCast;
}

char* PtreeFstyleCastExpr::GetEncodedType()
{
    return type;
}

void PtreeFstyleCastExpr::Print(std::ostream& s, int i, int d)
{
    NonLeaf::Print(s, i, d);
}

Ptree* PtreeFstyleCastExpr::Translate(AbstractTranslatingWalker* w)
{
    return w->TranslateFstyleCast(this);
}

void PtreeFstyleCastExpr::Typeof(AbstractTypingWalker* w, TypeInfo& t)
{
    w->TypeofFstyleCast(this, t);
}

}
