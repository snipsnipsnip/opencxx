//@beginlicenses@
//@license{chiba_tokyo}{}@
//@license{xerox}{}@
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
//
//  Copyright (c) 1995, 1996 Xerox Corporation.
//  All Rights Reserved.
//
//  Use and copying of this software and preparation of derivative works
//  based upon this software are permitted. Any copy of this software or
//  of any derivative work must include the above copyright notice of   
//  Xerox Corporation, this paragraph and the one after it.  Any
//  distribution of this software or derivative works must comply with all
//  applicable United States export control laws.
//
//  This software is made available AS IS, and XEROX CORPORATION DISCLAIMS
//  ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE  
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR    
//  PURPOSE, AND NOTWITHSTANDING ANY OTHER PROVISION CONTAINED HEREIN, ANY
//  LIABILITY FOR DAMAGES RESULTING FROM THE SOFTWARE OR ITS USE IS
//  EXPRESSLY DISCLAIMED, WHETHER ARISING IN CONTRACT, TORT (INCLUDING
//  NEGLIGENCE) OR STRICT LIABILITY, EVEN IF XEROX CORPORATION IS ADVISED
//  OF THE POSSIBILITY OF SUCH DAMAGES.
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

#include <iostream>
#include <opencxx/parser/Program.h>
#include <opencxx/parser/Ptree.h>
#include <opencxx/parser/auxil.h>
#include <opencxx/parser/token-names.h>
#include <opencxx/parser/PtreeUtil.h>

namespace Opencxx
{

char Program::Get()
{
    if(buf[index] == '\0')
	return buf[index];
    else
	return buf[index++];
}

void Program::Subst(Ptree* newtext, Ptree* oldtext)
{
    Replace(PtreeUtil::LeftMost(oldtext), PtreeUtil::RightMost(oldtext), newtext);
}

void Program::Insert(Ptree* pos, Ptree* before_text, Ptree* after_text)
{
    char* p;

    if(before_text != 0){
	p = PtreeUtil::LeftMost(pos);
	Replace(p, p, before_text);
    }

    if(after_text != 0){
	p = PtreeUtil::RightMost(pos);
	Replace(p, p, after_text);
    }
}

void Program::MinimumSubst(Ptree* newtext, Ptree* oldtext)
{
    if(MinimumSubst2(newtext, oldtext))
	Subst(newtext, oldtext);
}

bool Program::MinimumSubst2(Ptree* newtext, Ptree* oldtext)
{
    int what;
    if(oldtext == newtext) {
	return false;
    }
    else if(oldtext == 0 || newtext == 0) {
	return true;
    }
    else if(what = newtext->What(),
	    (what == ntExprStatement || what == ntTypedef)) {
	return true;
    }
    else if(oldtext->IsLeaf() || newtext->IsLeaf()) {
	return true;
    }
    else if(oldtext->Car() == 0 && oldtext->Cdr() == 0) {
	return true;
    }
    else if(oldtext == newtext->Cdr()) {
	Insert(oldtext, newtext->Car(), 0);
	return false;
    }
    else if(oldtext->Car() != 0 && oldtext->Car() == PtreeUtil::Second(newtext)) {
	Insert(oldtext->Car(), newtext->Car(), 0);
	newtext = PtreeUtil::ListTail(newtext, 2);
	if(MinimumSubst2(newtext, oldtext->Cdr())) {
	    if(oldtext->Cdr() == 0) {
		Insert(oldtext->Car(), 0, newtext);
	    }
	    else {
		Subst(newtext, oldtext->Cdr());
	    }
        }
	return false;
    }
    else{
	bool dirty1 = MinimumSubst2(newtext->Car(), oldtext->Car());
	bool dirty2 = MinimumSubst2(newtext->Cdr(), oldtext->Cdr());
	if(dirty1 == dirty2) {
	    return dirty1;
	}
	else if(dirty1) {
	    if(oldtext->Cdr() == 0 && newtext->Cdr() == 0) {
		return true;
	    }
	    else if(oldtext->Car() == 0) {
		Insert(oldtext->Cdr(), newtext->Car(), 0);
	    }
	    else {
		Subst(newtext->Car(), oldtext->Car());
	    }
        }
	else {
	    if(oldtext->Car() == 0 && newtext->Car() == 0) {
		return true;
            }
	    else if(oldtext->Cdr() == 0) {
		Insert(oldtext->Car(), 0, newtext->Cdr());
	    }
	    else {
		Subst(newtext->Cdr(), oldtext->Cdr());
	    }
        }
	return false;
    }
}

void Program::Replace(char* startpos, char* endpos, Ptree* text)
{
    if(startpos == 0 || endpos == 0)
	return;

    unsigned start = unsigned(startpos - buf);
    unsigned end = unsigned(endpos - buf);
    Replacement* p = replacement;
    if(p == 0)
	replacement = new Replacement(0, start, end, text);
    else if (start < p->startpos)
	replacement = new Replacement(p, start, end, text);
    else if(p->next == 0)
	p->next = new Replacement(0, start, end, text);
    else{
	for(; p->next != 0; p = p->next)
	    if(start < p->next->startpos)
		break;

	p->next = new Replacement(p->next, start, end, text);
    }
}

/*
  LineNumber() returns the line number of the line pointed to by PTR.
*/
unsigned Program::LineNumber(char* ptr, char*& filename, int& filename_length)
{
    int n;
    int  len;
    unsigned name;

    int nline = 0;
    unsigned pos = unsigned(ptr - buf);
    if(pos > size){
	// error?
	filename = defaultname;
	filename_length = strlen(defaultname);
	return 0;
    }

    int line_number = -1;
    filename_length = 0;

    while(pos > 0){
	switch(Ref(--pos)){
	case '\n' :
	    ++nline;
	    break;
	case '#' :
	    len = 0;
	    n = ReadLineDirective(pos, -1, name, len);
	    if(n >= 0){			// unless #pragma
		if(line_number < 0)
		    line_number = n + nline;

		if(len > 0 && filename_length == 0){
		    filename = (char*)Read(name);
		    filename_length = len;
		}
	    }
	    break;
	}

	if(line_number >= 0 && filename_length > 0)
	    return line_number;
    }

    if(filename_length == 0){
	filename = defaultname;
	filename_length = strlen(defaultname);
    }

    if(line_number < 0)
	line_number = nline + 1;

    return line_number;
}

/*
  Write() saves the program as a file named FILE_NAME.
  This assumes that the first line of the program is
  a # line directive.
*/
void Program::Write(std::ostream& out, const char* file_name)
{
    Replacement* rep = replacement;
    unsigned pos;
    unsigned nlines = 1;
    unsigned line_number = 1;
    unsigned i = 0;
    char c;

    unsigned filename = 0;
    int filename_length = 0;

    if(Ref(i) == '#')
	line_number = ReadLineDirective(i, (int)line_number,
					filename, filename_length);

    for(; rep != 0; rep = rep->next){
	pos = rep->startpos;
	while(i < pos){
	    c = Ref(i++);
	    if(c == '\0'){
		--i;
		break;
	    }

	    out << c;
	    if(c == '\n'){
		++nlines;
		++line_number;
		if(Ref(i) == '#')
		    line_number = ReadLineDirective(i, (int)line_number,
						    filename, filename_length);
	    }
	}

	if(i > 0 && Ref(i - 1) != '\n'){
	    out << '\n';
	    ++nlines;
	}

#if defined(_MSC_VER) || defined(IRIX_CC)
	out << "#line " << nlines + 1 << " \"" << file_name << "\"\n";
#else
	out << "# " << nlines + 1 << " \"" << file_name << "\"\n";
#endif
	++nlines;
	nlines += rep->text->Write(out);
	pos = rep->endpos;
	if(rep->next != 0 && rep->next->startpos <= pos){
	    rep = rep->next;
	    out << '\n';
	    ++nlines;
	    nlines += rep->text->Write(out);
	    if(rep->endpos > pos)
		pos = rep->endpos;
	}

	while(i < pos){
	    c = Ref(i++);
	    if(c == '\0'){
		--i;
		break;
	    }
	    else if(c == '\n'){
		++line_number;
		if(Ref(i) == '#')
		    line_number = ReadLineDirective(i, (int)line_number,
						    filename, filename_length);
	    }
	}

#if defined(_MSC_VER) || defined(IRIX_CC)
	out << "\n#line " << line_number << ' ';
#else
	out << "\n# " << line_number << ' ';
#endif
	++nlines;
	if(filename_length > 0)
	    for(int j = 0; j < filename_length; ++j)
		out << (char)Ref(filename + j);
	else
	    out << '"' << defaultname << '"';

	out << '\n';
	++nlines;
    }

    while((c = Ref(i++)) != '\0'){
	out << c;
	if(c == '\n')
	    ++nlines;
    }

#if defined(_MSC_VER) || defined(IRIX_CC)
    out << "\n#line " << nlines + 2 << " \"" << file_name << "\"\n";
#else
    out << "\n# " << nlines + 2 << " \"" << file_name << "\"\n";
#endif
}

int Program::ReadLineDirective(unsigned i, int line_number,
				unsigned& filename, int& filename_length)
{
    char c;

    do{
	c = Ref(++i);
    }while(is_blank(c));

#if defined(_MSC_VER) || defined(IRIX_CC)
    if(i + 4 <= GetSize() && strncmp(Read(i), "line", 4) == 0){
	i += 3;
	do{
	    c = Ref(++i);
	}while(is_blank(c));
    }
#endif

    if(is_digit(c)){		/* # <line> <file> */
	unsigned num = c - '0';
	for(;;){
	    c = Ref(++i);
	    if(is_digit(c))
		num = num * 10 + c - '0';
	    else
		break;
	}

	line_number = num - 1;	/* line_number'll be incremented soon */

	if(is_blank(c)){
	    do{
		c = Ref(++i);
	    }while(is_blank(c));
	    if(c == '"'){
		unsigned fname_start = i;
		do{
		    c = Ref(++i);
		} while(c != '"');
		if(i > fname_start + 2){
		    filename = fname_start;
		    filename_length = int(i - fname_start + 1);
		}
	    }
	}
    }

    return line_number;
}

// class Program::Replacement

Program::Replacement::Replacement(Replacement* n, unsigned st,
				  unsigned ed, Ptree* t)
{
    next = n;
    startpos = st;
    endpos = ed;
    text = t;
}

}
