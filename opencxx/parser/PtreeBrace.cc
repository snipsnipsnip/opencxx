
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
#include <opencxx/parser/PtreeBrace.h>
#include <opencxx/parser/AbstractTranslatingWalker.h>
#include <opencxx/parser/token-names.h>
#include <opencxx/parser/MopMsg.h>
#include <opencxx/parser/TheErrorLog.h>
#include <opencxx/parser/ErrorLog.h>

namespace Opencxx
{

void PtreeBrace::Print(std::ostream& s, int indent, int depth)
{
    if(TooDeep(s, depth))
	return;

    int indent2 = indent + 1;
    s << "[{";
    Ptree* body = PtreeUtil::Rest(this);
    if(body == 0){
	PrintIndent(s, indent2);
	s << "0";
    }
    else
	while(body != 0){
	    PrintIndent(s, indent2);
	    if(body->IsLeaf()){
		s << "@ ";
		body->Print(s, indent + 1, depth + 1);
	    }
	    else{
		Ptree* head = body->Car();
		if(head == 0)
		    s << "0";
		else
		    head->Print(s, indent + 1, depth + 1);
	    }

	    body = body->Cdr();
	}

    PrintIndent(s, indent);
    s << "}]";
}

int PtreeBrace::Write(std::ostream& out, int indent)
{
    int n = 0;

    out << '{';
    Ptree* p = this->Cdr()->Car();
    while(p != 0){
	if(p->IsLeaf()){
	    TheErrorLog().Report(
	        MopMsg(Msg::Fatal, "PtreeBrace::Write()", "non list"));
	    break;
	}
	else{
	    PrintIndent(out, indent + 1);
	    ++n;
	    Ptree* q = p->Car();
	    p = p->Cdr();
	    if(q != 0)
		n += q->Write(out, indent + 1);
	}
    }

    PrintIndent(out, indent);
    ++n;
    out << '}';
    return n;
}

Ptree* PtreeBrace::Translate(AbstractTranslatingWalker* w)
{
    return w->TranslateBrace(this);
}

}
