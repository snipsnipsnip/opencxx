#ifndef guard_opencxx_Ptree_h
#define guard_opencxx_Ptree_h

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

#include <iosfwd>
#include <opencxx/parser/GC.h>
#include <opencxx/parser/deprecated.h>
#include <opencxx/parser/PtreeUtil.h>

namespace Opencxx
{

class AbstractTranslatingWalker;
class AbstractTypingWalker;
class TypeInfo;

#ifdef __opencxx
metaclass QuoteClass Ptree;           // get qMake() available
#endif

class Ptree : public LightObject {
public:

    /** This returns true if the metaobject indicates a lexical token. */
    virtual bool IsLeaf() const = 0;

    /** Prints the metaobject on the </code>os</code> for debugging. Sublists are surrounded by [ and ]. */
    void Display(std::ostream&);
    void Display();


OPENCXX_DEPRECATED_PUBLIC:

    /** @deprecaed Use PtreeUtil */
    bool Eq(char x);

    /** @deprecaed Use PtreeUtil */
    bool Eq(const char*);

    /** @deprecaed Use PtreeUtil */
    bool Eq(const char*, int);

    /** @deprecaed Use PtreeUtil */
    bool Eq(Ptree* p) { return Eq(this, p); }

    /** @deprecated Use Display() */
    void Display2(std::ostream& os);

public:

    /** @todo document */
    virtual void Print(std::ostream&, int, int) = 0;

    /** @todo document */
    int Write(std::ostream&);

    /** Writes the metaobject to the file specified by out. Unlike Display() , sublists are not surrounded by [ and ]. This member function returns the number of written lines. */
    virtual int Write(std::ostream&, int) = 0;

    /** @todo document */
    void PrintIndent(std::ostream&, int);


    /** Converts the parse tree into a character string and returns it. */
    char* ToString();

    /** @todo document */
    virtual void PrintOn(std::ostream&) const = 0;

    /** Returns position in the char buffer when the
        text parsed as <code>this</code> starts.
         */
    char* GetPosition() { return data.leaf.position; }

    /** Returns the length of the text parsed as <code>this</code>. */
    int GetLength() { return data.leaf.length; }


    /**  */
          Ptree* Car()       { return data.nonleaf.child; }

    /**  */
    const Ptree* Car() const { return data.nonleaf.child; }

    /**  */
          Ptree* Cdr()       { return data.nonleaf.next; }

    /**  */
    const Ptree* Cdr() const { return data.nonleaf.next; }

OPENCXX_DEPRECATED_PUBLIC:

    /** @deprecated Use <code>Cdr()->Car()</code>.
    */
    Ptree* Cadr() 
    { 
        Deprecated("Ptree::Cadr()", "p->Cdr()->Car()");
        return Cdr()->Car(); 
    }

    /** @deprecated Rationale: bloats interface 
        (use <code>Cdr()->Cdr()</code>).
    */
    Ptree* Cddr() 
    { 
        Deprecated("Ptree::Cadr()", "p->Cdr()->Cdr()");
        return Cdr()->Cdr(); 
    }

    /** @deprecated Use PtreeUtil */
    Ptree* Ca_ar();

public:

    /**  */
    void SetCar(Ptree* p) { data.nonleaf.child = p; }

    /**  */
    void SetCdr(Ptree* p) { data.nonleaf.next = p; }


OPENCXX_DEPRECATED_PUBLIC:

    /** Returns position in text buffer corresponding to
        left-most character parsed as <code>this</code>.
        
        @todo Is this equivalent to GetPosition()? */
    char* LeftMost();

    /** Returns position in text buffer corresponding to
        left-most character parsed as <code>this</code>.
     */
    char* RightMost();

public:
    /**  */
    bool IsA(int);

    /** Returns identifier of a node. 
        @see token-names.h */
    virtual int What();

    /** Returns encoded type for nodes that support it. */
    virtual char* GetEncodedType();

    /** Returns encoded name for nodes that support it. */
    virtual char* GetEncodedName();

    /** Calls appropriate Translate... method in <code>walker</code>,
        depending on dynamic type of <code>this</code>.
        
        @note This is <i>Visit</i> function of <i>Visitor Pattern</i>.
      */
    virtual Ptree* Translate(AbstractTranslatingWalker* walker);

    /** Calls appropriate Typeof... method in <code>walker</code>
        passing <code>ti</code> to it.
        
        @note This is <i>Visit</i> function of <i>Visitor Pattern</i>.
      */
    virtual void Typeof(AbstractTypingWalker* walker, TypeInfo& ti);

OPENCXX_DEPRECATED_PUBLIC:

    /**  @deprecated Use IsA(int) and ||. */
    bool IsA(int, int);

    /**  @deprecated Use IsA(int) and ||. */
    bool IsA(int, int, int);

    /** @deprecated Use PtreeUtil */
    Ptree* Last() { return Last(this); }
    
    /** @deprecated Use PtreeUtil */
    Ptree* First() { return First(this); }

    /** @deprecated Use PtreeUtil */
    Ptree* Rest() { return Rest(this); }

    /** @deprecated Use PtreeUtil */
    Ptree* Second() { return Second(this); }

    /** @deprecated Use PtreeUtil */
    Ptree* Third() { return Third(this); }

    /** @deprecated Use PtreeUtil */
    Ptree* Nth(int n) { return Nth(this, n); }

    /** @deprecated Use PtreeUtil */
    int Length() { return Length(this); }

    /** @deprecated Use PtreeUtil */
    Ptree* ListTail(int n) { return ListTail(this, n); }

    /** Returns true if lst and x are equal. @deprecated Use PtreeUtil */
    static bool Eq(Ptree* lst, char x);

    /** Returns true if lst and x are equal. @deprecated Use PtreeUtil */
    static bool Eq(Ptree* lst, const char* x);

    /** @todo document @deprecated Use PtreeUtil */
    static bool Eq(Ptree* lst, const char* x, int);

    /** Returns true if lst and x are equal. Determines the equivalence by comparing the pointers. @deprecated Use PtreeUtil */
    static bool Eq(Ptree*, Ptree*);

    /** @deprecated Use PtreeUtil  */
    static bool Equiv(Ptree*, Ptree*);

    /** Recursively compares x and y and returns true if they are equivalent. @deprecated Use PtreeUtil */
    static bool Equal(Ptree*, Ptree*);
    
    /** Returns the last cons cell, which is a 
        list containing only the last element of <code>lst</code>. 
        @deprecated Use PtreeUtil 
      */
    static Ptree* Last(Ptree*);

    /** Returns the first element of <code>lst</code>. @deprecated Use PtreeUtil */
    static Ptree* First(Ptree*);
    
    /** Returns the rest of <code>lst</code> except the first element, 
        that is, the <i>cdr</i> field of <code>lst</code>. 
        @deprecated Use PtreeUtil */
    static Ptree* Rest(Ptree*);
    
    /** Returns the second element of <code>lst</code>. 
        @deprecated Use PtreeUtil */
    static Ptree* Second(Ptree*);

    /** Returns the third element of <code>lst</code>. @deprecated Use PtreeUtil */
    static Ptree* Third(Ptree*);

    /** Returns the n-th element of lst. 
        <code>Nth(lst, 0)</code> is equivalent to <code>First(lst)</code>. 
        @deprecated Use PtreeUtil */
    static Ptree* Nth(Ptree*, int);
    
    /** Returns the number of the elements of <code>lst</code>. If lst 
        is not a list, then this returns a negative number. 
        @deprecated Use PtreeUtil */
    static int Length(Ptree*); 
    
    /** Returns a sublist of lst obtained by omitting the first <code>k</code>
        elements. <code>ListTail(lst, 1)</code> is equivalent 
        to <code>Rest(lst)</code>. @deprecated Use PtreeUtil */
    static Ptree* ListTail(Ptree*, int);

    /** Returns a cons cell whose car field is a and whose cdr is b. 
        @deprecated PtreeUtil */
    static Ptree* Cons(Ptree*, Ptree*);
    
    /*@{*/
    /** Creates a list @deprecated Use PtreeUtil */
    static Ptree* List();
    static Ptree* List(Ptree*);
    static Ptree* List(Ptree*, Ptree*);
    static Ptree* List(Ptree*, Ptree*, Ptree*);
    static Ptree* List(Ptree*, Ptree*, Ptree*, Ptree*);
    static Ptree* List(Ptree*, Ptree*, Ptree*, Ptree*, Ptree*);
    static Ptree* List(Ptree*, Ptree*, Ptree*, Ptree*, Ptree*, Ptree*);
    static Ptree* List(Ptree*, Ptree*, Ptree*, Ptree*, Ptree*, Ptree*,
		       Ptree*);
    static Ptree* List(Ptree*, Ptree*, Ptree*, Ptree*, Ptree*, Ptree*,
		       Ptree*, Ptree*);
    /*@}*/

    /** Returns a new list whose elements are the same as lst 's. @deprecated Use PtreeUtil */
    static Ptree* CopyList(Ptree*);
    
    /** @todo document */
    static Ptree* Append(Ptree*, Ptree*);
    
    /** Returns a list in which all occurrences of orig in lst are replaced with subst. This is not a destructive operation. 
        @deprecated Use PtreeUtil */
    static Ptree* ReplaceAll(Ptree*, Ptree*, Ptree*);


    /** @deprecated Use PtreeUtil  */
    static Ptree* Subst(Ptree*, Ptree*, Ptree*);

    /** @deprecated Use PtreeUtil  */
    static Ptree* Subst(Ptree*, Ptree*, Ptree*, Ptree*, Ptree*);

    /** @deprecated Use PtreeUtil  */
    static Ptree* Subst(Ptree*, Ptree*, Ptree*, Ptree*,
			Ptree*, Ptree*, Ptree*);

    /** @deprecated Use PtreeUtil  */
    static Ptree* ShallowSubst(Ptree*, Ptree*, Ptree*);

    /** @deprecated Use PtreeUtil  */
    static Ptree* ShallowSubst(Ptree*, Ptree*, Ptree*, Ptree*, Ptree*);

    /** @deprecated Use PtreeUtil  */
    static Ptree* ShallowSubst(Ptree*, Ptree*, Ptree*, Ptree*,
			       Ptree*, Ptree*, Ptree*);

    /** @deprecated Use PtreeUtil  */
    static Ptree* ShallowSubst(Ptree*, Ptree*, Ptree*, Ptree*,
			       Ptree*, Ptree*, Ptree*, Ptree*, Ptree*);

    /** @deprecated Use PtreeUtil  */
    static Ptree* SubstSublist(Ptree*, Ptree*, Ptree*);

    /** @deprecated Use PtreeUtil  */
    static Ptree* Snoc(Ptree*, Ptree*);

    /** @deprecated Use PtreeUtil  */
    static Ptree* Nconc(Ptree*, Ptree*);

    /** @deprecated Use PtreeUtil  */
    static Ptree* Nconc(Ptree*, Ptree*, Ptree*);

public:

    /** 
        Makes a Ptree metaobject according to the format. The format is
        a null-terminated string. All occurrences of %c (character), %d
        (integer), %s (character string), and %p (Ptree) in the format are
        replaced with the values following the format. %% in the format is
        replaced with %. 
      */
    static Ptree* Make(const char* pat, ...);

OPENCXX_DEPRECATED_PUBLIC:

    /** @deprecated Use PtreeUtil
      */
    static Ptree* GenSym();

    /** Makes a Ptree metaobject that represents the text. Any C++
        expression surrounded by backquotes can appear in text. Its
        occurrence is replaced with the value denoted by the expression. The
        type of the expression must be Ptree*, int, or char*.
        
        @deprecated Use PtreeUtil 
        */
    static Ptree* qMake(char*);

public:

    /** Returns true if the metaobject represents an integer constant. The
        denoted value is stored in value. Note that the denoted value is
        always a positive number because a negative number such as -4
        generates two distinct tokens such as - and 4.
     */
    bool Reify(unsigned int& value);

    /** Returns true if the metaobject represents a string literal. A
        string literal is a sequence of character surrounded by double
        quotes ". The denoted null-terminated string is stored in string. It
        does not include the double quotes at the both ends. Also, the
        escape sequences are not expanded.
     */
    bool Reify(char*& string);

protected:
    union {
	struct {
	    Ptree* child;
	    Ptree* next;
	} nonleaf;
	struct {
	    char* position;
	    int  length;
	} leaf;
    }data;

    friend class NonLeaf;
};

/** The operator << can be used to write a Ptree object to an output stream. It is equivalent to Write() in terms of the result.
 */
inline std::ostream& operator << (std::ostream& s, Ptree* p)
{
    p->Write(s);
    return s;
}

}

#endif /* ! guard_opencxx_Ptree_h */

