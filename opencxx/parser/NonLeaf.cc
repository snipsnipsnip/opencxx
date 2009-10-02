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
#include <opencxx/parser/NonLeaf.h>
#include <opencxx/parser/ErrorLog.h>
#include <opencxx/parser/TheErrorLog.h>
#include <opencxx/parser/Encoding.h>
#include <opencxx/parser/MopMsg.h>
#include <opencxx/parser/token-names.h>

namespace Opencxx
{

// class NonLeaf

NonLeaf::NonLeaf(Ptree* p, Ptree* q)
{
    data.nonleaf.child = p;
    data.nonleaf.next = q;
}

bool NonLeaf::IsLeaf() const
{
    return false;
}

void NonLeaf::Print(std::ostream& s, int indent, int depth)
{
    if(TooDeep(s, depth))
	return;

    Ptree* rest = this;
    s << '[';
    while(rest != 0){
	if(rest->IsLeaf()){
	    s << "@ ";
	    rest->Print(s, indent, depth + 1);
	    rest = 0;
	}
	else{
	    Ptree* head = rest->data.nonleaf.child;
	    if(head == 0)
		s << "0";
	    else
		head->Print(s, indent, depth + 1);
	    rest = rest->data.nonleaf.next;
	    if(rest != 0)
		s << ' ';
	}
    }

    s << ']';
}

bool NonLeaf::TooDeep(std::ostream& s, int depth)
{
    if(depth >= 32){
	s << " ** too many nestings ** ";
	return true;
    }
    else
	return false;
}

int NonLeaf::Write(std::ostream& out, int indent)
{
    int n = 0;
    Ptree* p = this;
    for(;;){
	Ptree* head = p->Car();
	if(head != 0)
	    n += head->Write(out, indent);

	p = p->Cdr();
	if(p == 0)
	    break;
	else if(p->IsLeaf()){
	    TheErrorLog().Report(
	        MopMsg(Msg::Warning, "NonLeaf::Write()", "not list"));
	    break;
	}
	else
	    out << ' ';
    }

    return n;
}

void NonLeaf::PrintWithEncodeds(std::ostream& s, int indent, int depth)
{
    char* encode = GetEncodedType();
    if(encode != 0){
	s << '#';
	Encoding::Print(s, encode);
    }

    encode = GetEncodedName();
    if(encode != 0){
	s << '@';
	Encoding::Print(s, encode);
    }

    NonLeaf::Print(s, indent, depth);
}

void NonLeaf::PrintOn(std::ostream& out) const
{
    const Ptree* p = this;
    for(;;){
	const Ptree* head = p->Car();
	if(head != 0)
	    head->PrintOn(out);

	p = p->Cdr();
	if(p == 0)
	    break;
	else if(p->IsLeaf()){
	    TheErrorLog().Report(
	        MopMsg(Msg::Fatal, "NonLeaf::PrintOn()", "not list"));
	    break;
	}
	else
	    out << ' ';
    }
}

}

