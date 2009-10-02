//@beginlicenses@
//@license{chiba-tokyo}{}@
//@license{contributors}{}@
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
//  Other Contributors (see file AUTHORS) make(s) no representations about the suitability of this
//  software for any purpose. It is provided "as is" without express or implied
//  warranty.
//  
//  Copyright (C)  Other Contributors (see file AUTHORS)
//
//@endlicenses@

#include <cstring>
#include <opencxx/ClassArray.h>
#include <opencxx/parser/GC.h>
#include "opencxx/parser/CerrErrorLog.h"
#include <opencxx/parser/ErrorLog.h>
#include <opencxx/parser/TheErrorLog.h>
#include <opencxx/parser/MopMsg.h>

namespace Opencxx
{

ClassArray::ClassArray(int s)
{
    num = 0;
    if(s < 1)
	s = 1;

    size = s;
    array = new (GC) Class*[s];
}

void ClassArray::Append(Class* p)
{
    if(num >= size){
	size += 16;
	Class** a = new (GC) Class*[size];
	memmove(a, array, size_t(num * sizeof(Class*)));
	array = a;
    }

    array[num++] = p;
}

Class*& ClassArray::Ref(unsigned i)
{
    if(i < num)
	return array[i];
    else{
	TheErrorLog().Report(MopMsg(Msg::Fatal, "ClassArray", "out of range"));
	return array[0];
    }
}

}
