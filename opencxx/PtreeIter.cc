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

#include <opencxx/PtreeIter.h>
#include <opencxx/parser/Ptree.h>

namespace Opencxx
{

Ptree* PtreeIter::Pop()
{
    if(ptree == 0)
	return 0;
    else{
	Ptree* p = ptree->Car();
	ptree = ptree->Cdr();
	return p;
    }
}

bool PtreeIter::Next(Ptree*& car)
{
    if(ptree == 0)
	return false;
    else{
	car = ptree->Car();
	ptree = ptree->Cdr();
	return true;
    }
}

Ptree* PtreeIter::This()
{
    if(ptree == 0)
	return 0;
    else
	return ptree->Car();
}

}
