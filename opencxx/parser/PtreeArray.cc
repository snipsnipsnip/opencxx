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

#include <opencxx/parser/PtreeArray.h>
#include <opencxx/parser/NonLeaf.h>
#include <opencxx/parser/TheErrorLog.h>
#include <opencxx/parser/ErrorLog.h>
#include <opencxx/parser/MopMsg.h>
#include <opencxx/parser/GC.h>

namespace Opencxx
{

PtreeArray::PtreeArray(int s)
{
    num = 0;
    if(s > 8){
	size = s;
	array = new (GC) Ptree*[s];
    }
    else{
	size = 8;
	array = default_buf;
    }
}

void PtreeArray::Append(Ptree* p)
{
    if(num >= size){
	size = size * 2;
	Ptree** a = new (GC) Ptree*[size];
	memmove(a, array, size_t(num * sizeof(Ptree*)));
	array = a;
    }

    array[num++] = p;
}

Ptree*& PtreeArray::Ref(unsigned i)
{
    if(i < num)
	return array[i];
    else{
	TheErrorLog().Report(MopMsg(Msg::Fatal, "PtreeArray", "out of range"));
	return array[0];
    }
}

Ptree* PtreeArray::All()
{
    Ptree* lst = 0;

    for(int i = Number() - 1; i >= 0; --i)
	lst = new NonLeaf(Ref(i), lst);

    return lst;
}

}
