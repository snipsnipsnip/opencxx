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

#include <opencxx/parser/PtreeUsing.h>
#include <opencxx/parser/AbstractTranslatingWalker.h>
#include <opencxx/parser/token-names.h>
#include <opencxx/parser/Encoding.h>
#include <opencxx/parser/deprecated.h>

namespace Opencxx
{

PtreeUsing::PtreeUsing(Ptree* p, Ptree* q, Ptree* r, Encoding& encName, Ptree* s)
  : NonLeaf(p, new NonLeaf(q, new NonLeaf(r, new NonLeaf(s, 0))))
{
    Deprecated("PtreeUsing::PtreeUsing(Ptree* p, Ptree* q, Ptree* r, Encoding& encName, Ptree* s)",
               "PtreeUsing(p,q,r,encName.Get(),s)");
    name = encName.Get();
}

PtreeUsing::PtreeUsing(Ptree* p, Ptree* q, Ptree* r, char* encName, Ptree* s)
  : NonLeaf(p, new NonLeaf(q, new NonLeaf(r, new NonLeaf(s, 0))))
  , name(encName)
{
}

char* PtreeUsing::GetEncodedName()
{
  return name;
}

int PtreeUsing::What()
{
    return ntUsing;
}

Ptree* PtreeUsing::Translate(AbstractTranslatingWalker* w)
{
    return w->TranslateUsing(this);
}

}
