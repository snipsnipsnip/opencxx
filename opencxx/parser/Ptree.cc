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
#include <cstdarg>
#include <stdio.h>
#include <string.h>
#include <string>
#include <strstream>
#include <sys/time.h>
#include <opencxx/parser/ErrorLog.h>
#include <opencxx/parser/MopMsg.h>
#include <opencxx/parser/TheErrorLog.h>
#include <opencxx/parser/AbstractTranslatingWalker.h>
#include <opencxx/parser/AbstractTypingWalker.h>
#include <opencxx/parser/NonLeaf.h> // :TODO: cyclic dependency!!! get rid of it!
#include <opencxx/parser/Ptree.h>
#include <opencxx/parser/token-names.h>
#include <opencxx/parser/ErrorLog.h>
#include <opencxx/parser/Lex.h>
#include <opencxx/parser/MopMsg.h>
#include <opencxx/parser/TheErrorLog.h>
#include <opencxx/parser/GC.h>
#include <opencxx/parser/ptreeAll.h>
#include <opencxx/parser/auxil.h>

namespace Opencxx
{

using std::ostream;

void Ptree::Display2(std::ostream& s)
{
    Deprecated("Ptree::Display2(ostream& os)", "p->Display(os)");
    Display(s);
}

void Ptree::Display(std::ostream& s)
{
    if(this == 0)
	s << "0\n";
    else{
	Print(s, 0, 0);
	s << '\n';
    }
}

void Ptree::Display()
{
    Display(std::cerr);
}

int Ptree::Write(ostream& s)
{
    if(this == 0)
	return 0;
    else
	return Write(s, 0);
}

void Ptree::PrintIndent(std::ostream& out, int indent)
{
    out << '\n';
    for(int i = 0; i < indent; ++i)
	out << "    ";
}

char* Ptree::ToString()
{
    if(this == 0)
    {
	return 0;  // :TODO: why? shouldn't we assert(this) ???
    }
    else{
	std::strstream buf;
	PrintOn(buf);
	buf << std::ends;
	const std::string& s = buf.str();
	char* result = new (GC) char[s.length() + 1];
	std::strcpy(result, s.c_str());
	buf.freeze(false);
	return result;
    }
}

bool Ptree::Eq(char c)
{
    Deprecated("Ptree:Eq(char c)", "PtreeUtil::Eq(ptree,c)");
    return PtreeUtil::Eq(this, c);
}

bool Ptree::Eq(const char* str)
{
    Deprecated("Ptree::Eq(const char*)", "PtreeUtil:...");
    return PtreeUtil::Eq(this, str);
}

bool Ptree::Eq(const char* str, int len)
{
    Deprecated("Ptree::Eq(const char* str, int len)", "PtreeUtil::...");
    return PtreeUtil::Eq(this, str, len);
}

Ptree* Ptree::Ca_ar()
{
    Deprecated("Ptree::Ca_ar()", "PtreeUtil::...");
    return PtreeUtil::Ca_ar(this);
}

char* Ptree::LeftMost()
{
    Deprecated("Ptree::LeftMost()", "PtreeUtil::LeftMost(ptree)");
    return PtreeUtil::LeftMost(this);
}

char* Ptree::RightMost()
{
    Deprecated("Ptree::RightMost()", "PtreeUtil::RightMost(ptree)");
    return PtreeUtil::RightMost(this);
}

int Ptree::What()
{
    return BadToken;
}

bool Ptree::IsA(int kind)
{
    if(this == 0)
	return false;
    else
	return bool(What() == kind);
}

bool Ptree::IsA(int kind1, int kind2)
{
    Deprecated("Ptree::IsA(int k1, int k2)",
               "p->IsA(k1) || p->IsA(k2)");
    if(this == 0)
	return false;
    else{
	int k = What();
	return bool(k == kind1 || k == kind2);
    }
}

bool Ptree::IsA(int kind1, int kind2, int kind3)
{
    Deprecated("Ptree::IsA(int k1, int k2)",
               "p->IsA(k1) || p->IsA(k2) || p->IsA(k2)");
    if(this == 0)
	return false;
    else{
	int k = What();
	return bool(k == kind1 || k == kind2 || k == kind3);
    }
}

Ptree* Ptree::Translate(AbstractTranslatingWalker* w)
{
    return w->TranslatePtree(this);
}

void Ptree::Typeof(AbstractTypingWalker* w, TypeInfo& t)
{
    w->TypeofPtree(this, t);
}

char* Ptree::GetEncodedType()
{
    return 0;
}

char* Ptree::GetEncodedName()
{
    return 0;
}


// static members

bool Ptree::Eq(Ptree* p, char c)
{
    Deprecated("Ptree::Eq(Ptree* p, char c)", "PtreeUtil::Eq(Ptree* p, char c)");
    return PtreeUtil::Eq(p, c);
}

bool Ptree::Eq(Ptree* p, const char* str)
{
    Deprecated("Ptree::Eq(Ptree* p, const char* str)", "PtreeUtil::Eq(Ptree* p, char* str)");
    return PtreeUtil::Eq(p, str);
}

bool Ptree::Eq(Ptree* p, const char* str, int len)
{
    Deprecated("Ptree::Eq(Ptree* p, const char* str, int len)", "PtreeUtil::Eq(Ptree* p, char* str, int len)");
    return PtreeUtil::Eq(p, str, len);
}

bool Ptree::Eq(Ptree* p, Ptree* q)
{
    Deprecated("Ptree::Eq(Ptree* p, Ptree* q)", "PtreeUtil::Eq(Ptree* p, Ptree* q)");
    return PtreeUtil::Eq(p, q);
}

bool Ptree::Equiv(Ptree* p, Ptree* q)
{
    Deprecated("Ptree::Equiv(Ptree* p, Ptree* q)", "PtreeUtil::Equiv(Ptree* p, Ptree* q)");
    return PtreeUtil::Equiv(p, q);
}

bool Ptree::Equal(Ptree* p, Ptree* q)
{
    Deprecated("Ptree::Equal(Ptree* p, Ptree* q)", "PtreeUtil::Equal(Ptree* p, Ptree* q)");
    return PtreeUtil::Equal(p, q);
}

Ptree* Ptree::Last(Ptree* p)
{
    Deprecated("Ptree::Last(Ptree* p)", "PtreeUtil::Last(Ptree* p)");
    return PtreeUtil::Last(p);
}

Ptree* Ptree::First(Ptree* p)
{
    Deprecated("Ptree::First(Ptree* p)", "PtreeUtil::First(Ptree* p)");
    return PtreeUtil::First(p);
}

Ptree* Ptree::Rest(Ptree* p)
{
    Deprecated("Ptree::Rest(Ptree* p)", "PtreeUtil::Rest(Ptree* p)");
    return PtreeUtil::Rest(p);
}

Ptree* Ptree::Second(Ptree* p)
{
    Deprecated("Ptree::Second(Ptree* p)", "PtreeUtil::Second(Ptree* p)");
    return PtreeUtil::Second(p);
}

Ptree* Ptree::Third(Ptree* p)
{
    Deprecated("Ptree::Third(Ptree* p)", "PtreeUtil::Third(Ptree* p)");
    return PtreeUtil::Third(p);
}

Ptree* Ptree::Nth(Ptree* p, int n)
{
    Deprecated("Ptree::Nth(Ptree* p, int n)", "PtreeUtil::Nth(Ptree* p, int n)");
    return PtreeUtil::Nth(p, n);
}

int Ptree::Length(Ptree* p)
{
    Deprecated("Ptree::Length(Ptree* p)", "PtreeUtil::Length(Ptree* p)");
    return PtreeUtil::Length(p);
}

Ptree* Ptree::ListTail(Ptree* p, int k)
{
    Deprecated("Ptree::ListTail(Ptree* p, int k)", "PtreeUtil::ListTail(Ptree* p, int k)");
    return PtreeUtil::ListTail(p, k);
}

Ptree* Ptree::Cons(Ptree* p, Ptree* q)
{
    Deprecated("Ptree::Cons()", "PtreeUtil::Cons()");
    return PtreeUtil::Cons(p, q);
}

Ptree* Ptree::List(Ptree* p)
{
    Deprecated("Ptree::List(Ptree* p)", "PtreeUtil::List(Ptree* p)");
    return PtreeUtil::List(p);
}

Ptree* Ptree::List()
{
    Deprecated("Ptree::List()", "PtreeUtil::List()");
    return PtreeUtil::List();
}

Ptree* Ptree::List(Ptree* p, Ptree* q)
{
    Deprecated("Ptree::List(Ptree* p, Ptree* q)", "PtreeUtil::List(Ptree* p, Ptree* q)");
    return PtreeUtil::List(p, q);
}

Ptree* Ptree::List(Ptree* p1, Ptree* p2, Ptree* p3)
{
    Deprecated("Ptree::List(Ptree* p1, Ptree* p2, Ptree* p3)", "PtreeUtil::List(Ptree* p1, Ptree* p2, Ptree* p3)");
    return PtreeUtil::List(p1, p2, p3);
}

Ptree* Ptree::List(Ptree* p1, Ptree* p2, Ptree* p3, Ptree* p4)
{
    Deprecated("Ptree::List(Ptree* p1, Ptree* p2, Ptree* p3, Ptree* p4)", "PtreeUtil::List(Ptree* p1, Ptree* p2, Ptree* p3, Ptree* p4)");
    return PtreeUtil::List(p1, p2, p3, p4);
}

Ptree* Ptree::List(Ptree* p1, Ptree* p2, Ptree* p3, Ptree* p4, Ptree* p5)
{
    Deprecated("Ptree::List(Ptree* p1, Ptree* p2, Ptree* p3, Ptree* p4, Ptree* p5)", "PtreeUtil::List(Ptree* p1, Ptree* p2, Ptree* p3, Ptree* p4, Ptree* p5)");
    return PtreeUtil::List(p1, p2, p3, p4, p5);
}

Ptree* Ptree::List(Ptree* p1, Ptree* p2, Ptree* p3, Ptree* p4, Ptree* p5,
		   Ptree* p6)
{
    Deprecated("Ptree::List(Ptree* p1, Ptree* p2, Ptree* p3, Ptree* p4, Ptree* p5, Ptree* p6)", "PtreeUtil::List(Ptree* p1, Ptree* p2, Ptree* p3, Ptree* p4, Ptree* p5, Ptree* p6)");
    return PtreeUtil::List(p1, p2, p3, p4, p5, p6);
}

Ptree* Ptree::List(Ptree* p1, Ptree* p2, Ptree* p3, Ptree* p4, Ptree* p5,
		   Ptree* p6, Ptree* p7)
{
    Deprecated("Ptree::List(Ptree* p1, Ptree* p2, Ptree* p3, Ptree* p4, Ptree* p5, Ptree* p6, Ptree* p7)", "PtreeUtil::List(Ptree* p1, Ptree* p2, Ptree* p3, Ptree* p4, Ptree* p5, Ptree* p6, Ptree* p7)");
    return PtreeUtil::List(p1, p2, p3, p4, p5, p6, p7);
}

Ptree* Ptree::List(Ptree* p1, Ptree* p2, Ptree* p3, Ptree* p4, Ptree* p5,
		   Ptree* p6, Ptree* p7, Ptree* p8)
{
    Deprecated("Ptree::List(Ptree* p1, Ptree* p2, Ptree* p3, Ptree* p4, Ptree* p5, Ptree* p6, Ptree* p7, Ptree* p8)", "PtreeUtil::List(Ptree* p1, Ptree* p2, Ptree* p3, Ptree* p4, Ptree* p5, Ptree* p6, Ptree* p7, Ptree* p8)");
    return PtreeUtil::List(p1, p2, p3, p4, p5, p6, p7, p8);
}

Ptree* Ptree::CopyList(Ptree* p)
{
    Deprecated("Ptree::CopyList(Ptree* p)", "PtreeUtil::CopyList(Ptree* p)");
    return PtreeUtil::CopyList(p);
}

Ptree* Ptree::Append(Ptree* p, Ptree* q)
{
    Deprecated("Ptree::Append(Ptree* p, Ptree* q)", "PtreeUtil::Append(Ptree* p, Ptree* q)");
    return PtreeUtil::Append(p, q);
}

Ptree* Ptree::ReplaceAll(Ptree* list, Ptree* orig, Ptree* subst)
{
    Deprecated("Ptree::ReplaceAll(Ptree* list, Ptree* orig, Ptree* subst)", "PtreeUtil::ReplaceAll(Ptree* list, Ptree* orig, Ptree* subst)");
    return PtreeUtil::ReplaceAll(list, orig, subst);
}

Ptree* Ptree::Subst(Ptree* newone, Ptree* old, Ptree* tree)
{
    Deprecated("Ptree::Subst(Ptree* newone, Ptree* old, Ptree* tree)", "PtreeUtil::Subst(Ptree* newone, Ptree* old, Ptree* tree)");
    return PtreeUtil::Subst(newone, old, tree);
}

Ptree* Ptree::Subst(Ptree* newone1, Ptree* old1, Ptree* newone2, Ptree* old2,
		    Ptree* tree)
{
    Deprecated("Ptree::Subst(Ptree* newone1, Ptree* old1, Ptree* newone2, Ptree* old2, Ptree* tree)", "PtreeUtil::Subst(Ptree* newone1, Ptree* old1, Ptree* newone2, Ptree* old2, Ptree* tree)");
    return PtreeUtil::Subst(newone1, old1, newone2, old2, tree);
}

Ptree* Ptree::Subst(Ptree* newone1, Ptree* old1, Ptree* newone2, Ptree* old2,
		    Ptree* newone3, Ptree* old3, Ptree* tree)
{
    Deprecated("Ptree::Subst(Ptree* newone1, Ptree* old1, Ptree* newone2, Ptree* old2, Ptree* newone3, Ptree* old3, Ptree* tree)", "PtreeUtil::Subst(Ptree* newone1, Ptree* old1, Ptree* newone2, Ptree* old2, Ptree* newone3, Ptree* old3, Ptree* tree)");
    return PtreeUtil::Subst(newone1, old1, newone2, old2, newone3, old3, tree);
}

Ptree* Ptree::ShallowSubst(Ptree* newone, Ptree* old, Ptree* tree)
{
    Deprecated("Ptree::ShallowSubst(Ptree* newone, Ptree* old, Ptree* tree)", "PtreeUtil::ShallowSubst(Ptree* newone, Ptree* old, Ptree* tree)");
    return PtreeUtil::ShallowSubst(newone, old, tree);
}

Ptree* Ptree::ShallowSubst(Ptree* newone1, Ptree* old1,
			   Ptree* newone2, Ptree* old2, Ptree* tree)
{
    Deprecated("Ptree::ShallowSubst(Ptree* newone1, Ptree* old1, Ptree* newone2, Ptree* old2, Ptree* tree)", "PtreeUtil::ShallowSubst(Ptree* newone1, Ptree* old1, Ptree* newone2, Ptree* old2, Ptree* tree)");
    return PtreeUtil::ShallowSubst(newone1, old1, newone2, old2, tree);
}

Ptree* Ptree::ShallowSubst(Ptree* newone1, Ptree* old1,
			   Ptree* newone2, Ptree* old2,
			   Ptree* newone3, Ptree* old3, Ptree* tree)
{
    Deprecated("Ptree::ShallowSubst(Ptree* newone1, Ptree* old1, Ptree* newone2, Ptree* old2, Ptree* newone3, Ptree* old3, Ptree* tree)", "PtreeUtil::ShallowSubst(Ptree* newone1, Ptree* old1, Ptree* newone2, Ptree* old2, Ptree* newone3, Ptree* old3, Ptree* tree)");
    return PtreeUtil::ShallowSubst(newone1, old1, newone2, old2, newone3, old3, tree);
}

Ptree* Ptree::ShallowSubst(Ptree* newone1, Ptree* old1,
			   Ptree* newone2, Ptree* old2,
			   Ptree* newone3, Ptree* old3,
			   Ptree* newone4, Ptree* old4, Ptree* tree)
{
    Deprecated("Ptree::ShallowSubst(Ptree* newone1, Ptree* old1, Ptree* newone2, Ptree* old2, Ptree* newone3, Ptree* old3, Ptree* newone4, Ptree* old4, Ptree* tree)", "PtreeUtil::ShallowSubst(Ptree* newone1, Ptree* old1, Ptree* newone2, Ptree* old2, Ptree* newone3, Ptree* old3, Ptree* newone4, Ptree* old4, Ptree* tree)");
    return PtreeUtil::ShallowSubst(newone1, old1, newone2, old2, newone3, old3, newone4, old4, tree);
}

Ptree* Ptree::SubstSublist(Ptree* newsub, Ptree* oldsub, Ptree* lst)
{
    Deprecated("Ptree::SubstSublist(Ptree* newsub, Ptree* oldsub, Ptree* lst)", "PtreeUtil::SubstSublist(Ptree* newsub, Ptree* oldsub, Ptree* lst)");
    return PtreeUtil::SubstSublist(newsub, oldsub, lst);
}

Ptree* Ptree::Snoc(Ptree* p, Ptree* q)
{
    Deprecated("Ptree::Snoc(Ptree* p, Ptree* q)", "PtreeUtil::Snoc(Ptree* p, Ptree* q)");
    return PtreeUtil::Snoc(p, q);
}

Ptree* Ptree::Nconc(Ptree* p, Ptree* q)
{
    Deprecated("Ptree::Nconc(Ptree* p, Ptree* q)", "PtreeUtil::Nconc(Ptree* p, Ptree* q)");
    return PtreeUtil::Nconc(p, q);
}

Ptree* Ptree::Nconc(Ptree* p, Ptree* q, Ptree* r)
{
    Deprecated("Ptree::Nconc(Ptree* p, Ptree* q, Ptree* r)", "PtreeUtil::Nconc(Ptree* p, Ptree* q, Ptree* r)");
    return PtreeUtil::Nconc(p, q, r);
}

Ptree* Ptree::Make(const char* pat, ...)
{
    va_list args;
    const int N = 4096;
    static char buf[N];
    char c;
    int len;
    char* ptr;
    Ptree* p;
    Ptree* q;
    int i = 0, j = 0;
    Ptree* result = 0;

    va_start(args, pat);
    while((c = pat[i++]) != '\0')
	if(c == '%'){
	    c = pat[i++];
	    if(c == '%')
		buf[j++] = c;
	    else if(c == 'd'){
		ptr = IntegerToString(va_arg(args, int), len);
		memmove(&buf[j], ptr, len);
		j += len;
	    }
	    else if(c == 's'){
		ptr = va_arg(args, char*);
		len = strlen(ptr);
		memmove(&buf[j], ptr, len);
		j += len;
	    }
	    else if(c == 'c')
		buf[j++] = va_arg(args, int);
	    else if(c == 'p'){
		p = va_arg(args, Ptree*);
		if(p == 0)
		    /* ignore */;
		else if(p->IsLeaf()){
		    memmove(&buf[j], p->GetPosition(), p->GetLength());
		    j += p->GetLength();
		}
		else{   
		    if(j > 0)
			q = PtreeUtil::List(new DupLeaf(buf, j), p);
		    else
			q = PtreeUtil::List(p);

		    j = 0;
		    result = PtreeUtil::Nconc(result, q);
		}
	    }
	    else
		TheErrorLog().Report(MopMsg(Msg::Fatal, "Ptree::Make()", "invalid format"));
	}
	else
	    buf[j++] = c;

    va_end(args);

    if(j > 0)
	if(result == 0)
	    result = new DupLeaf(buf, j);
	else
	    result = PtreeUtil::Snoc(result, new DupLeaf(buf, j));

    return result;
}

bool Ptree::Reify(unsigned int& value)
{
    return Lex::Reify(this, value);
}

bool Ptree::Reify(char*& str)
{
    return Lex::Reify(this, str);
}

Ptree* Ptree::qMake(char*)
{
    TheErrorLog().Report(MopMsg(Msg::Fatal, "PtreeUtil::qMake()", "the metaclass must be compiled by OpenC++."));
    return 0;
}

Ptree* Ptree::GenSym()
{
    Deprecated("Ptree::GenSym()", "PtreeUtil::GenSym()");
    return PtreeUtil::GenSym();
}

}

