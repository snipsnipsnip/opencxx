//@beginlicenses@
//@license{chiba-tokyo}{}@
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
//@endlicenses@

#include <opencxx/parser/PtreeConstants.h>
#include <opencxx/parser/ptreeAll.h>

namespace Opencxx
{

namespace PtreeConstants {

Ptree* bool_t = new LeafBOOLEAN("bool", 4);
Ptree* char_t = new LeafCHAR("char", 4);
Ptree* wchar_t_t = new LeafCHAR("wchar_t", 7);
Ptree* int_t = new LeafINT("int", 3);
Ptree* short_t = new LeafSHORT("short", 5);
Ptree* long_t = new LeafLONG("long", 4);
Ptree* float_t = new LeafFLOAT("float", 5);
Ptree* double_t = new LeafDOUBLE("double", 6);
Ptree* void_t = new LeafVOID("void", 4);

Ptree* signed_t = new LeafSIGNED("signed", 6);
Ptree* unsigned_t = new LeafUNSIGNED("unsigned", 8);
Ptree* const_t = new LeafCONST("const", 5);
Ptree* volatile_t = new LeafVOLATILE("volatile", 8);

Ptree* operator_name = new LeafReserved("operator", 8);
Ptree* new_operator = new LeafReserved("new", 3);
Ptree* anew_operator = new LeafReserved("new[]", 5);
Ptree* delete_operator = new LeafReserved("delete", 6);
Ptree* adelete_operator = new LeafReserved("delete[]", 8);

Ptree* star = new Leaf("*", 1);
Ptree* ampersand = new Leaf("&", 1);
Ptree* comma = new Leaf(",", 1);
Ptree* dots = new Leaf("...", 3);
Ptree* scope = new Leaf("::", 2);
Ptree* tilder = new Leaf("~", 1);
Ptree* left_paren = new Leaf("(", 1);
Ptree* right_paren = new Leaf(")", 1);
Ptree* left_bracket = new Leaf("[", 1);
Ptree* right_bracket = new Leaf("]", 1);
Ptree* left_angle = new Leaf("<", 1);
Ptree* right_angle = new Leaf(">", 1);

}

}

