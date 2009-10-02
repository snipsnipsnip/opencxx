#ifndef guard_opencxx_PtreeConstants_h
#define guard_opencxx_PtreeConstants_h
//@beginlicenses@
//@license{chiba-tokyo}{}@
//@license{Grzegorz Jakacki}{2004}@
//
//  Copyright (C) 1997-2001 Shigeru Chiba, Tokyo Institute of Technology.
//
//  Permission to use, copy, distribute and modify this software and
//  its documentation for any purpose is hereby granted without fee,
//  provided that the above copyright notice appears in all copies and that
//  both that copyright notice and this permission notice appear in
//  supporting documentation.
//
//  Shigeru Chiba makes no representations about the suitability of this
//  software for any purpose.  It is provided "as is" without express or
//  implied warranty.
//
//  -----------------------------------------------------------------
//
//  Permission to use, copy, distribute and modify this software and its  
//  documentation for any purpose is hereby granted without fee, provided that
//  the above copyright notice appears in all copies and that both that copyright
//  notice and this permission notice appear in supporting documentation.
// 
//  Grzegorz Jakacki make(s) no representations about the suitability of this
//  software for any purpose. It is provided "as is" without express or implied
//  warranty.
//  
//  Copyright (C) 2004 Grzegorz Jakacki
//
//@endlicenses@

namespace Opencxx
{

class Ptree;

namespace PtreeConstants {

extern Ptree* bool_t;
extern Ptree* char_t;
extern Ptree* wchar_t_t;
extern Ptree* int_t;
extern Ptree* short_t;
extern Ptree* long_t;
extern Ptree* float_t;
extern Ptree* double_t;
extern Ptree* void_t;

extern Ptree* signed_t;
extern Ptree* unsigned_t;
extern Ptree* const_t;
extern Ptree* volatile_t;

extern Ptree* operator_name;
extern Ptree* new_operator;
extern Ptree* anew_operator;
extern Ptree* delete_operator;
extern Ptree* adelete_operator;

extern Ptree* star;
extern Ptree* ampersand;
extern Ptree* comma;
extern Ptree* dots;
extern Ptree* scope;
extern Ptree* tilder;
extern Ptree* left_paren;
extern Ptree* right_paren;
extern Ptree* left_bracket;
extern Ptree* right_bracket;
extern Ptree* left_angle;
extern Ptree* right_angle;

}

}

#endif /* ! guard_opencxx_PtreeConstants_h */
