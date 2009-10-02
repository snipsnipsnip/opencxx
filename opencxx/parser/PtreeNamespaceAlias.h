#ifndef guard_opencxx_PtreeNamespaceAlias_h
#define guard_opencxx_PtreeNamespaceAlias_h

//@beginlicenses@
//@license{chiba-tokyo}{}@
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
//@endlicenses@

#include <opencxx/parser/NonLeaf.h>

namespace Opencxx
{

class AbstractTranslatingWalker;

class PtreeNamespaceAlias : public NonLeaf 
{
public:
  PtreeNamespaceAlias(Ptree* p, Ptree* q) : NonLeaf(p, q) {}
  int What();
  Ptree *Translate(AbstractTranslatingWalker*);
};

}

#endif /* ! guard_opencxx_PtreeNamespaceAlias_h */
