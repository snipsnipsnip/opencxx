#ifndef guard_opencxx_auxil_h
#define guard_opencxx_auxil_h

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

namespace Opencxx
{

inline bool is_blank(char c){
    return(c == ' ' || c == '\t' || c == '\f' || c == '\r');
}

inline bool is_letter(char c){
    return('A' <= c && c <= 'Z' || 'a' <= c && c <= 'z' || c == '_'
	   || c == '$');
}

inline bool is_digit(char c){ return('0' <= c && c <= '9'); }

inline bool is_xletter(char c){ return(c == 'X' || c == 'x'); }

inline bool is_eletter(char c){ return(c == 'E' || c == 'e'); }

inline bool is_hexdigit(char c){
    return(is_digit(c) || 'A' <= c && c <= 'F' || 'a' <= c && c <= 'f');
}

inline bool is_int_suffix(char c){
    return(c == 'U' || c == 'u' || c == 'L' || c == 'l');
}

inline bool is_float_suffix(char c){
    return(c == 'F' || c == 'f' || c == 'L' || c == 'l');
}

char* gc_aware_strdup(const char* str);

char* IntegerToString(int num, int& length);

}

#endif /* ! guard_opencxx_auxil_h */
