//@beginlicenses@
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
//@endlicenses@

#include <cstring>
#include <iostream>
#include <opencxx/parser/token-names.h>
#include <opencxx/parser/Token.h>
#include <opencxx/parser/Leaf.h>

using namespace std;

namespace Opencxx
{

// class Leaf

Leaf::Leaf(char* ptr, int len)
{
    data.leaf.position = ptr;
    data.leaf.length = len;
}

Leaf::Leaf(Token& tk)
{
    data.leaf.position = tk.ptr;
    data.leaf.length = tk.len;
}

bool Leaf::IsLeaf() const
{
    return true;
}

void Leaf::Print(ostream& s, int, int)
{
    char* p = data.leaf.position;
    int n = data.leaf.length;

    // Recall that [, ], and @ are special characters.

    if(n < 1)
	return;
    else if(n == 1 && *p == '@'){
	s << "\\@";
	return;
    }

    char c = *p++;
    if(c == '[' || c == ']')
	s << '\\' << c;		// [ and ] at the beginning are escaped.
    else
	s << c;

    while(--n > 0)
	s << *p++;
}

int Leaf::Write(ostream& out, int indent)
{
    int n = 0;
    char* ptr = data.leaf.position;
    int len = data.leaf.length;
    while(len-- > 0){
	char c = *ptr++;
	if(c == '\n'){
	    PrintIndent(out, indent);
	    ++n;
	}
	else
	    out << c;
    }

    return n;
}

void Leaf::PrintOn(std::ostream& out) const
{
    // :TODO: reimplement with 'std::copy'
    char* ptr = data.leaf.position;
    int len = data.leaf.length;
    while(len-- > 0)
    {
	out << *ptr++;
    }
}

}

