#ifndef guard_opencxx_PtreeUtil_h
#define guard_opencxx_PtreeUtil_h

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

#include <opencxx/parser/auxil.h>

namespace Opencxx
{

class Ptree;
class PtreeDeclarator;

namespace PtreeUtil
{
    void SetDeclaratorComments(Ptree* def, Ptree* comments);
    Ptree* NthDeclarator(Ptree* def, int& nth);
    bool GetArgDeclList(PtreeDeclarator* decl, Ptree*& args);

    Ptree* Cons(Ptree* p, Ptree* q);
    Ptree* CopyList(Ptree* p);

    /** Returns true iff <code>p</code> represents character <code>c</code>. 
        @pre <code>p</code> is a leaf
      */
    bool Eq(Ptree* p, char c);

    /** Returns true iff <code>p</code> represents string <code>str</code>. 
        @pre <code>p</code> is a leaf
    */
    bool Eq(Ptree* p, const char* str);

    /** Returns true iff p reprsents <code>n</code>-character prefix of <code>str</core>
        @pre p is a leaf
      */
    bool Eq(Ptree* p, const char* str, int len);
    
    /** Returns true iff <code>p</code> and <code>q</code> 
        represent the same text.
        @pre <code>p</code> and <code>q</code> are leafs.
      */
    bool Eq(Ptree* p, Ptree* q);

    /**
      Returns true even if p and q are lists and all the elements
      are equal in terms of <code>Equal()</code>.
    */
    bool Equiv(Ptree* p, Ptree* q);
    
    /**
      Tree equality based on <code>Eq()</code> for leaves.
      */
    bool Equal(Ptree* p, Ptree* q);

    /** Returns the last cons cell */
    Ptree* Last(Ptree* p);
    
    /** @name Accessors */
    /*@{*/
    Ptree* First(Ptree* p);
    Ptree* Rest(Ptree* p);
    Ptree* Second(Ptree* p);
    Ptree* Third(Ptree* p);
    /*@}*/

    /*
      Nth(lst, 0) is equivalent to First(lst).
    */
    Ptree* Nth(Ptree* p, int n);

    /*
      Returns length of list <code>p</code>.
      Returns negative number if <code>p</code> is not a list.
    */
    int Length(Ptree* p);

    Ptree* ListTail(Ptree* p, int k);
    
    /** @name Construction */
    /*@{*/
    Ptree* List(Ptree* p);
    Ptree* List();
    Ptree* List(Ptree* p, Ptree* q);
    Ptree* List(Ptree* p1, Ptree* p2, Ptree* p3);
    Ptree* List(Ptree* p1, Ptree* p2, Ptree* p3, Ptree* p4);
    Ptree* List(Ptree* p1, Ptree* p2, Ptree* p3, Ptree* p4, Ptree* p5);
    Ptree* List(Ptree* p1, Ptree* p2, Ptree* p3, Ptree* p4, Ptree* p5, Ptree* p6);
    Ptree* List(Ptree* p1, Ptree* p2, Ptree* p3, Ptree* p4, Ptree* p5,
    		   Ptree* p6, Ptree* p7);
    Ptree* List(Ptree* p1, Ptree* p2, Ptree* p3, Ptree* p4, Ptree* p5,
    		   Ptree* p6, Ptree* p7, Ptree* p8);
    /*@}*/
    
    Ptree* Append(Ptree* p, Ptree* q);

    /**
      ReplaceAll() substitutes SUBST for all occurences of ORIG in LIST.
      It recursively searches LIST for ORIG.
    */
    Ptree* ReplaceAll(Ptree* list, Ptree* orig, Ptree* subst);

    Ptree* Subst(Ptree* newone, Ptree* old, Ptree* tree);

    Ptree* Subst(Ptree* newone1, Ptree* old1, Ptree* newone2, Ptree* old2,
    		    Ptree* tree);

    Ptree* Subst(Ptree* newone1, Ptree* old1, Ptree* newone2, Ptree* old2,
    		    Ptree* newone3, Ptree* old3, Ptree* tree);

    Ptree* ShallowSubst(Ptree* newone, Ptree* old, Ptree* tree);

    Ptree* ShallowSubst(Ptree* newone1, Ptree* old1,
    			   Ptree* newone2, Ptree* old2, Ptree* tree);

    Ptree* ShallowSubst(Ptree* newone1, Ptree* old1,
    			   Ptree* newone2, Ptree* old2,
    			   Ptree* newone3, Ptree* old3, Ptree* tree);

    Ptree* ShallowSubst(Ptree* newone1, Ptree* old1,
    			   Ptree* newone2, Ptree* old2,
    			   Ptree* newone3, Ptree* old3,
    			   Ptree* newone4, Ptree* old4, Ptree* tree);

    Ptree* SubstSublist(Ptree* newsub, Ptree* oldsub, Ptree* lst);

    Ptree* Snoc(Ptree* p, Ptree* q);

    Ptree* Nconc(Ptree* p, Ptree* q);

    Ptree* Nconc(Ptree* p, Ptree* q, Ptree* r);

    /** Computes Ca...ar */
    Ptree* Ca_ar(Ptree* p);

    char* LeftMost(Ptree* p);

    char* RightMost(Ptree* p);

    /** Compares the pattern and list. If they match, this function
        returns true and binds the sublists to appropriate sublists of the
        list, as specified by the pattern. Note that the type of sublist is
        pointer to Ptree*.
        
        @deprecated Use PtreeUtil
      */
    bool Match(Ptree*, char*, ...);


    /** Generates a unique symbol name (aka identifier) and returns it. The
        returned symbol name is used as the name of a temporary variable,
        for example.
      */
    Ptree* GenSym();
}

}

#endif /* ! guard_opencxx_PtreeUtil_h */
