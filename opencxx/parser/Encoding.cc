//@beginlicenses@
//@license{chiba_tokyo}{}@
//@license{contributors}{}@
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
//  Other Contributors (see file AUTHORS) make(s) no representations about the suitability of this
//  software for any purpose. It is provided "as is" without express or implied
//  warranty.
//  
//  Copyright (C)  Other Contributors (see file AUTHORS)
//
//@endlicenses@

/*
  This encoding is also interpreted by TypeInfo.  If you modify this
  file, check typeinfo.{h,cc} out as well.

  'b' boolean
  'c' char
  'w' wchar_t
  'i' int (signed, unsigned)
  's' short (short int)
  'l' long (long int)
  'j' long long
  'f' float
  'd' double
  'r' long double
  'v' void

  'T' template class (e.g. Foo<int,char> ==> T[3]Foo[2]ic.  [2] means
      the length of "ic".  It doesn't mean the number of template
      arguments.
  'e' ...
  '?' no return type.  the return type of constructors
  '*' non-type template parameter

  'S' singned
  'U' unsigned
  'C' const
  'V' volatile

  'P' pointer
  'R' reference
  'A' []
  'F' function (e.g. char foo(int) ==> Fi_c)
  'M' pointer to member (e.g. Type::* ==> M[4]Type)

  'Q' qualified class (e.g. X::YY ==> Q[2][1]X[2]YY, ::YY ==> Q[2][0][2]YY)

  [x] means (0x80 + x)
  '0' means :: (global scope)

  Special function names:

  operator + ==> +
  operator new[] ==> new[]
  operator <type> ==> @<encoded type>		cast operator
*/

#include <string.h>
#include <iostream>
#include <opencxx/parser/Encoding.h>
#include <opencxx/parser/ptreeAll.h>
#include <opencxx/parser/auxil.h>
#include <opencxx/parser/GC.h>
#include <opencxx/parser/ErrorLog.h>
#include <opencxx/parser/TheErrorLog.h>
#include <opencxx/parser/MopMsg.h>
#include <opencxx/parser/PtreeConstants.h>

using namespace std;

namespace Opencxx
{

const int DigitOffset = 0x80;

void Encoding::Reset(Encoding& e)
{
    len = e.len;
    if(len > 0)
	memmove(name, e.name, len);
}

char* Encoding::Get()
{
    if(len == 0)
	return 0;
    else{
	char* s = new (GC) char[len + 1];
	memmove(s, name, len);
	s[len] = '\0';
	return s;
    }
}

void Encoding::Print(ostream& s, char* p)
{
    unsigned char* ptr = (unsigned char*)p;
    for(;*ptr != '\0'; ++ptr)
	if(*ptr < 0x80)
	    s << char(*ptr);
	else
	    s << char(*ptr - 0x80 + '0');
}

void Encoding::CvQualify(Ptree* cv1, Ptree* cv2)
{
    bool c = false, v = false;
    if(cv1 != 0 && !cv1->IsLeaf())
	while(cv1 != 0){
	    int kind = cv1->Car()->What();
	    cv1 = cv1->Cdr();
	    if(kind == CONST)
		c = true;
	    else if(kind == VOLATILE)
		v = true;
	}

    if(cv2 != 0 && !cv2->IsLeaf())
	while(cv2 != 0){
	    int kind = cv2->Car()->What();
	    cv2 = cv2->Cdr();
	    if(kind == CONST)
		c = true;
	    else if(kind == VOLATILE)
		v = true;
	}

    if(v)
	Insert('V');

    if(c)
	Insert('C');
}

void Encoding::GlobalScope()
{
    Append(DigitOffset);
}

// SimpleName() is also used for operator names

void Encoding::SimpleName(Ptree* id)
{
    AppendWithLen(id->GetPosition(), id->GetLength());
}

// NoName() generates a internal name for no-name enum and class
// declarations.

void Encoding::NoName()
{
    static int i = 0;
    static unsigned char name[] = "`0000";
    int n = i++;
    name[1] = n / 1000 + '0';
    name[2] = (n / 100) % 10 + '0';
    name[3] = (n / 10) % 10 + '0';
    name[4] = n % 10 + '0';
    AppendWithLen((char*)name, 5);
}

void Encoding::Template(Ptree* name, Encoding& args)
{
    Append('T');
    SimpleName(name);
    AppendWithLen(args);
}

void Encoding::Qualified(int n)
{
    if(len + 1 >= MaxNameLen)
	TheErrorLog().Report(MopMsg(Msg::Fatal, "Encoding::Qualified()", "too long encoded name"));

    memmove(name + 2, name, len);
    len += 2;
    name[0] = 'Q';
    name[1] = (unsigned char)(DigitOffset + n);
}

void Encoding::Destructor(Ptree* class_name)
{
    int len = class_name->GetLength();
    Append((unsigned char)(DigitOffset + len + 1));
    Append('~');
    Append(class_name->GetPosition(), len);
}

void Encoding::PtrOperator(int t)
{
    if(t == '*')
	Insert('P');
    else
	Insert('R');
}

void Encoding::PtrToMember(Encoding& encode, int n)
{
    if(n < 2)
	Insert((char*)encode.name, encode.len);
    else{
	Insert((char*)encode.name, encode.len);
	Insert((unsigned char)(DigitOffset + n));
	Insert('Q');
    }

    Insert('M');
}

void Encoding::CastOperator(Encoding& type)
{
    Append((unsigned char)(DigitOffset + type.len + 1));
    Append('@');
    Append((char*)type.name, type.len);
}

void Encoding::Insert(unsigned char c)
{
    if(len >= MaxNameLen)
	TheErrorLog().Report(MopMsg(Msg::Fatal, "Encoding::Insert()", "too long encoded name"));

    if(len > 0)
	memmove(name + 1, name, len);

    ++len;
    name[0] = c;
}

void Encoding::Insert(char* str, int n)
{
    if(len + n >= MaxNameLen)
	TheErrorLog().Report(MopMsg(Msg::Fatal, "Encoding::Insert()", "too long encoded name"));

    if(len > 0)
	memmove(&name[n], name, len);

    memmove(name, str, n);
    len += n;
}

void Encoding::Append(unsigned char c)
{
    if(len >= MaxNameLen)
	TheErrorLog().Report(MopMsg(Msg::Fatal, "Encoding::Append()", "too long encoded name"));

    name[len++] = c;
}

void Encoding::Append(char* str, int n)
{
    if(len + n >= MaxNameLen)
	TheErrorLog().Report(MopMsg(Msg::Fatal, "Encoding::Append(char*,int)", "too long encoded name"));

    memmove(&name[len], str, n);
    len += n;
}

void Encoding::AppendWithLen(char* str, int n)
{
    if(len + n + 1 >= MaxNameLen)
	TheErrorLog().Report(MopMsg(Msg::Fatal, "Encoding::AppendWithLen()", "too long encoded name"));

    name[len++] = (unsigned char)(DigitOffset + n);
    memmove(&name[len], str, n);
    len += n;
}

Ptree* Encoding::MakePtree(unsigned char*& encoded, Ptree* decl)
{
    using namespace PtreeConstants;
    
    Ptree* cv;
    Ptree* typespec = 0;
    if(decl != 0)
	decl = PtreeUtil::List(decl);

    for(;;){
	cv = 0;
	switch(*encoded++){
	case 'b' :
	    typespec = PtreeUtil::Snoc(typespec, bool_t);
	    goto finish;
	case 'c' :
	    typespec = PtreeUtil::Snoc(typespec, char_t);
	    goto finish;
	case 'w' :
	    typespec = PtreeUtil::Snoc(typespec, wchar_t_t);
	    goto finish;
	case 'i' :
	    typespec = PtreeUtil::Snoc(typespec, int_t);
	    goto finish;
	case 's' :
	    typespec = PtreeUtil::Snoc(typespec, short_t);
	    goto finish;
	case 'l' :
	    typespec = PtreeUtil::Snoc(typespec, long_t);
	    goto finish;
	    break;
	case 'j' :
	    typespec = PtreeUtil::Nconc(typespec, PtreeUtil::List(long_t, long_t));
	    goto finish;
	    break;
	case 'f' :
	    typespec = PtreeUtil::Snoc(typespec, float_t);
	    goto finish;
	    break;
	case 'd' :
	    typespec = PtreeUtil::Snoc(typespec, double_t);
	    goto finish;
	    break;
	case 'r' :
	    typespec = PtreeUtil::Nconc(typespec, PtreeUtil::List(long_t, double_t));
	    goto finish;
	case 'v' :
	    typespec = PtreeUtil::Snoc(typespec, void_t);
	    goto finish;
	case 'e' :
	    return dots;
	case '?' :
	    goto finish;
	case 'Q' :
	    typespec = PtreeUtil::Snoc(typespec, MakeQname(encoded));
	    goto finish;
	case 'S' :
	    typespec = PtreeUtil::Snoc(typespec, signed_t);
	    break;
	case 'U' :
	    typespec = PtreeUtil::Snoc(typespec, unsigned_t);
	    break;
	case 'C' :
	    if(*encoded == 'V'){
		++encoded;
		cv = PtreeUtil::List(const_t, volatile_t);
	    }
	    else
		cv = PtreeUtil::List(const_t);

	    goto const_or_volatile;
	case 'V' :
	    cv = PtreeUtil::List(volatile_t);
	const_or_volatile :
	    switch(*encoded) {
	    case 'M' :
	    case 'P' :
	    case 'R' :
		decl = PtreeUtil::Nconc(cv, decl);
		break;
	    case 'F' :
		++encoded;
		goto cv_function;
	    default :
		typespec = PtreeUtil::Nconc(cv, typespec);
		break;
	    }
	    break;
	case 'M' :
	    {
		Ptree* ptr;
		if(*encoded == 'Q')
		    ptr = MakeQname(++encoded);
		else
		    ptr = MakeLeaf(encoded);

		ptr = PtreeUtil::List(ptr, scope, star);
		decl = PtreeUtil::Cons(ptr, decl);
	    }

	    goto pointer_or_reference;
	case 'P' :
	    decl = PtreeUtil::Cons(star, decl);
	    goto pointer_or_reference;
	case 'R' :
	    decl = PtreeUtil::Cons(ampersand, decl);
	pointer_or_reference :
	    if(*encoded == 'A' || *encoded == 'F')
		decl = PtreeUtil::List(PtreeUtil::List(left_paren, decl,
					       right_paren));

	    break;
	case 'A' :
	    decl = PtreeUtil::Nconc(decl, PtreeUtil::List(left_bracket,
						  right_bracket));
	    break;
	case 'F' :
	cv_function :
	    {
		Ptree* args = 0;
		while(*encoded != '\0'){
		    if(*encoded == '_'){
			++encoded;
			break;
		    }
		    else if(*encoded == 'v'){
			encoded += 2;
			break;
		    }

		    if(args != 0)
			args = PtreeUtil::Snoc(args, comma);

		    args = PtreeUtil::Snoc(args, MakePtree(encoded, 0));
		}

		decl = PtreeUtil::Nconc(decl, PtreeUtil::List(left_paren, args,
						      right_paren));
		if(cv != 0)
		    decl = PtreeUtil::Nconc(decl, cv);
	    }
	    break;
	case '\0' :
	    goto finish;
	case 'T' :
	    {
    		Ptree* tlabel = MakeLeaf(encoded);      
	    	Ptree* args = 0;
	    	int n = *encoded++ - DigitOffset;
		unsigned char* stop = encoded + n;
		while(encoded < stop){
		    if(args != 0)
			args = PtreeUtil::Snoc(args, comma);

		    args = PtreeUtil::Snoc(args, MakePtree(encoded, 0));
		}

		tlabel = PtreeUtil::List(tlabel,
				PtreeUtil::List(left_angle, args, right_angle));
		typespec = PtreeUtil::Nconc(typespec, tlabel);
		goto finish;
	    }
	case '*' :
	    goto error;
	default :
	    if(*--encoded >= DigitOffset){
		if(typespec == 0)
		    typespec = MakeLeaf(encoded);
		else
		    typespec = PtreeUtil::Snoc(typespec, MakeLeaf(encoded));

		goto finish;
	    }
	error :
	    TheErrorLog().Report(MopMsg(Msg::Fatal, "Encoding::MakePtree()", "sorry, cannot handle this type"));
	    break;
	}
    }

finish :
    return PtreeUtil::List(typespec, decl);
}

Ptree* Encoding::MakeQname(unsigned char*& encoded)
{
    using namespace PtreeConstants;
    
    int n = *encoded++ - DigitOffset;
    Ptree* qname = 0;
    while(n-- > 0){
	Ptree* leaf = MakeLeaf(encoded);
	if(leaf != 0)
	    qname = PtreeUtil::Snoc(qname, leaf);

	if(n > 0)
	    qname = PtreeUtil::Snoc(qname, scope);
    }

    return qname;
}

Ptree* Encoding::MakeLeaf(unsigned char*& encoded)
{
    Ptree* leaf;
    int len = *encoded++ - DigitOffset;
    if(len > 0)
	leaf = new Leaf((char*)encoded, len);
    else
	leaf = 0;

    encoded += len;
    return leaf;
}

bool Encoding::IsSimpleName(unsigned char* p)
{
    return *p >= DigitOffset;
}

Ptree* Encoding::NameToPtree(char* name, int len)
{
    using namespace PtreeConstants;
    
    if(name == 0)
	return 0;

    if(name[0] == 'n'){
	if(len == 5 && strncmp(name, "new[]", 5) == 0)
	    return PtreeUtil::List(operator_name, anew_operator);
	else if(len == 3 && strncmp(name, "new", 3) == 0)
	    return PtreeUtil::List(operator_name, new_operator);
    }
    else if(name[0] == 'd'){
	if(len == 8 && strncmp(name, "delete[]", 8) == 0)
	    return PtreeUtil::List(operator_name, adelete_operator);
	else if(len == 6 && strncmp(name, "delete", 6) == 0)
	    return PtreeUtil::List(operator_name, delete_operator);
    }
    else if(name[0] == '~')
	return PtreeUtil::List(tilder, new Leaf(&name[1], len - 1));
    else if(name[0] == '@'){		// cast operator
	unsigned char* encoded = (unsigned char*)&name[1];
	return PtreeUtil::List(operator_name, MakePtree(encoded, 0));
    }

    if(is_letter(name[0]))
	return new Leaf(name, len);
    else
	return PtreeUtil::List(operator_name, new Leaf(name, len));
}

}
