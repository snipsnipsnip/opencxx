#ifndef guard_opencxx_Program_h
#define guard_opencxx_Program_h

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

#include <iosfwd>
#include <opencxx/parser/GC.h>

namespace Opencxx
{

class Ptree;

class Program : public Object {
public:
    Program(char *name) {
	replacement = 0;
	defaultname = name;
    }

    virtual ~Program() {}

    void Rewind() { index = 0; }
    void Rewind(unsigned i) { index = i; }
    unsigned GetSize() { return size; }
    void Unget() { --index; }

    char Ref(unsigned position) { return buf[position]; }
    void Set(char c, unsigned position) { buf[position] = c; }

    /* The location of the last character obtained by Get(). */
    unsigned GetCurPos() { return index - 1; }

    /* The location of the next character obtained by Get(). */
    unsigned GetNextPos() { return index; }

    /* The result of Read() must be the same for each call. */
    const char* Read(unsigned p) { return &buf[p]; }

    virtual char Get();

    void Subst(Ptree* newtext, Ptree* oldtext);
    void Insert(Ptree* pos, Ptree* before_text, Ptree* after_text);
    void Replace(char*, char*, Ptree*);
    void MinimumSubst(Ptree* newtext, Ptree* oldtext);

    unsigned LineNumber(char*, char*&, int&);

    void Write(std::ostream&, const char*);
    int ReadLineDirective(unsigned, int, unsigned&, int&);

private:
    bool MinimumSubst2(Ptree* newtext, Ptree* oldtext);

protected:
    char*	buf;
    unsigned	size, index;
    char	*defaultname;

private:
    class Replacement : public LightObject {
    public:
	Replacement(Replacement*, unsigned, unsigned, Ptree*);
	Replacement* next;
	unsigned startpos;
	unsigned endpos;
	Ptree* text;
    };

    Replacement* replacement;
};

}

#endif /* ! guard_opencxx_Program_h */
