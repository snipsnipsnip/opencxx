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

#include <cstring>
#include <opencxx/parser/ProgramString.h>
#include <opencxx/parser/GC.h>

namespace Opencxx
{

const int Quantum = 4;
const int BufSize = 1 << Quantum;

ProgramString::ProgramString()
: Program("unknown")
{
    buf = new (GC) char[BufSize];
    buf[0] = '\0';
    size = BufSize;
    index = str_length = 0;
}

ProgramString::~ProgramString()
{
//  delete [] buf;
    buf = 0;
}

ProgramString& ProgramString::operator << (const char* str)
{
    int len = strlen(str) + 1;

    if(str_length + len < size){
	memmove(&buf[str_length], str, len);
	str_length += len - 1;
    }
    else{
	size = (str_length + len + BufSize) & ~(BufSize - 1);
	char* newbuf = new (GC) char[size];
	memmove(newbuf, buf, size_t(str_length));
	memmove(&newbuf[str_length], str, len);
//	delete [] buf;
	buf = newbuf;
	str_length += len - 1;
    }

    return *this;
}

ProgramString& ProgramString::operator << (const char c)
{
    if(str_length + 2 < size){
	buf[str_length] = c;
	buf[++str_length] = '\0';
    }
    else{
	size = (str_length + 2 + BufSize) & ~(BufSize - 1);
	char* newbuf = new (GC) char[size];
	memmove(newbuf, buf, size_t(str_length));
	newbuf[str_length] = c;
	newbuf[++str_length] = '\0';
//	delete [] buf;
	buf = newbuf;
    }

    return *this;
}

}
