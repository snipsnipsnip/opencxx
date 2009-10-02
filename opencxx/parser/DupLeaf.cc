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

#include <iostream>
#include <cstring>
#include <opencxx/parser/DupLeaf.h>
#include <opencxx/parser/token-names.h>
#include <opencxx/parser/GC.h>

namespace Opencxx
{

using std::ostream;

/** @warning Allocates new memory to the given string. */
DupLeaf::DupLeaf(char* str, int len) : Leaf(new (GC) char[len], len)
{
    memmove(data.leaf.position, str, len);
}

DupLeaf::DupLeaf(char* str1, int len1, char* str2, int len2)
: Leaf(new (GC) char[len1 + len2], len1 + len2)
{
    memmove(data.leaf.position, str1, len1);
    memmove(&data.leaf.position[len1], str2, len2);
}

void DupLeaf::Print(ostream& s, int, int)
{
    int i, j;
    char* pos;

    pos = data.leaf.position;
    j = data.leaf.length;

    if(j == 1 && *pos == '@'){
	s << "\\@";
	return;
    }

    s << '`';
    for(i = 0; i < j; ++i)
	if(pos[i] == '[' || pos[i] == ']'){
	    s << '\\';
	    s << pos[i];
	}
	else
	    s << pos[i];

    s << '`';
}

}
