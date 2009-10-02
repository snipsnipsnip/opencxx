//@beginlicenses@
//@license{chiba-tokyo}{}@
//@license{Grzegorz Jakacki}{2004}@
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
//  -----------------------------------------------------------------
//
//  Permission to use, copy, distribute and modify this software and its  
//  documentation for any purpose is hereby granted without fee, provided that
//  the above copyright notice appears in all copies and that both that copyright
//  notice and this permission notice appear in supporting documentation.
// 
//  Grzegorz Jakacki make(s) no representations about the suitability of this
//  software for any purpose. It is provided "as is" without express or implied
//  warranty.
//  
//  Copyright (C) 2004 Grzegorz Jakacki
//
//@endlicenses@

#include <../opencxx/PtreeMaker.h>
#include <cstring>
#include <../opencxx/defs.h>
#include <../opencxx/parser/Ptree.h>
#include <../opencxx/parser/DupLeaf.h>
#include <../opencxx/parser/PtreeUtil.h>

BEGIN_OPENCXX_NAMESPACE

using PtreeUtil::Snoc;
using PtreeUtil::Length;
using PtreeUtil::Last;

PtreeMaker& PtreeMaker::operator << (Ptree* p)
{
    ptree_ = Append(ptree_, p);
    return *this;
}

PtreeMaker& PtreeMaker::operator << (const char* str)
{
    if(*str != '\0')
	ptree_ =  Append(ptree_, (char*)str, strlen(str));

    return *this;
}

PtreeMaker& PtreeMaker::operator << (char* str)
{
    if(*str != '\0')
	ptree_ =  Append(ptree_, str, strlen(str));

    return *this;
}

PtreeMaker& PtreeMaker::operator << (char c)
{
    ptree_ =  Append(ptree_, &c, 1);
    return *this;
}

PtreeMaker& PtreeMaker::operator << (int n)
{
    int len;
    char* str = IntegerToString(n, len);
    ptree_ =  Append(ptree_, str, len);
    return *this;
}

Ptree* PtreeMaker::Append(Ptree* lst, Ptree* tail)
{
    Ptree* last;
    Ptree* p;
    Ptree* q;

    if(tail == 0)
	return lst;

    if(!tail->IsLeaf() && Length(tail) == 1){
	tail = tail->Car();
	if(tail == 0)
	    return lst;
    }

    if(tail->IsLeaf() && lst != 0){
	last = Last(lst);
	if(last != 0){
	    p = last->Car();
	    if(p != 0 && p->IsLeaf()){
		q = new DupLeaf(p->GetPosition(), p->GetLength(),
				 tail->GetPosition(), tail->GetLength());
		last->SetCar(q);
		return lst;
	    }
	}
    }

    return Snoc(lst, tail);
}

Ptree* PtreeMaker::Append(Ptree* lst, char* str, int len)
{
    Ptree* last;
    Ptree* p;
    Ptree* q;

    if(lst != 0){
	last = Last(lst);
	if(last != 0){
	    p = last->Car();
	    if(p != 0 && p->IsLeaf()){
		q = new DupLeaf(p->GetPosition(), p->GetLength(),
				 str, len);
		last->SetCar(q);
		return lst;
	    }
	}
    }

    return Snoc(lst, new DupLeaf(str, len));
}

END_OPENCXX_NAMESPACE
