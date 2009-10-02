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

#include <cstdarg>
#include <opencxx/parser/PtreeUtil.h>
#include <opencxx/parser/Ptree.h>
#include <opencxx/parser/Leaf.h>
#include <opencxx/parser/PtreeDeclarator.h>
#include <opencxx/parser/token-names.h>
#include <opencxx/parser/PtreeArray.h>
#include <opencxx/parser/TheErrorLog.h>
#include <opencxx/parser/ErrorLog.h>
#include <opencxx/parser/MopMsg.h>
#include <opencxx/parser/GC.h>

namespace Opencxx
{

namespace PtreeUtil {

void
SetDeclaratorComments(Ptree* def, Ptree* comments)
{
    if (def == 0 || !def->IsA(ntDeclaration))
	return;

    Ptree* decl;
    int n = 0;
    for (;;) {
	int i = n++;
	decl = NthDeclarator(def, i);
	if (decl == 0)
	    break;
	else if (decl->IsA(ntDeclarator))
	    ((PtreeDeclarator*)decl)->SetComments(comments);
    }
}

Ptree* 
NthDeclarator(Ptree* def, int& nth)
{
    Ptree* decls = PtreeUtil::Third(def);
    if(decls == 0 || decls->IsLeaf())
	return 0;

    if(decls->IsA(ntDeclarator)){	// if it is a function
	if(nth-- == 0)
	    return decls;
    }
    else
	while(decls != 0 && !decls->IsLeaf()){
	    if(nth-- == 0)
		return decls->Car();

	    if((decls = decls->Cdr()) != 0)
		decls = decls->Cdr();		// skip ,
	}

    return 0;
}

bool 
GetArgDeclList(PtreeDeclarator* decl, Ptree*& args)
{
    Ptree* p = decl;
    while(p != 0){
	Ptree* q = p->Car();
	if(q != 0)
	    if(q->IsLeaf()){
		if(Eq(q, '(')){
		    args = p->Cdr()->Car();
		    return true;
		}
		else if (Eq(q, '['))
		    break;
	    }
	    else if(Eq(q->Car(), '(')) {	// e.g. int (*p)[];
		p = q->Cdr()->Car();
		if (p->IsLeaf())
		    break;
	    }

	p = p->Cdr();
    }

    args = 0;
    return false;
}


Ptree* Cons(Ptree* p, Ptree* q)
{
    return new NonLeaf(p, q);
}


Ptree* CopyList(Ptree* p)
{
    return Append(p, 0);
}

bool Eq(Ptree* p, char c)
{
    if (p) {
        return p->IsLeaf() && p->GetLength()==1 && *(p->GetPosition())==c;
    }
    else {
        return false;
    }
}

bool Eq(Ptree* p, const char* str)
{
    return Eq(p, str, strlen(str));
}

bool Eq(Ptree* p, const char* str, int n)
{
    if(p && p->IsLeaf()){
        char* text = p->GetPosition();
        int len = p->GetLength();
        if(len == n){
            int i;   
            for(i = 0; i < n; ++i)
                if(text[i] != str[i])
                    return false; 

            return true;
        }
    }

    return false;
}

bool Eq(Ptree* p, Ptree* q)
{
    if(p == q)
	return true;
    else if(p == 0 || q == 0)
	return false;
    else if(p->IsLeaf() && q->IsLeaf()){
	int plen = p->GetLength();
	int qlen = q->GetLength();
	if(plen == qlen){
	    char* pstr = p->GetPosition();
	    char* qstr = q->GetPosition();
	    while(--plen >= 0)
		if(pstr[plen] != qstr[plen])
		    return false;

	    return true;
	}
    }

    return false;
}

bool Equiv(Ptree* p, Ptree* q)
{
    if(p == q)
	return true;
    else if(p == 0 || q == 0)
	return false;
    else if(p->IsLeaf() || q->IsLeaf())
	return Eq(p, q);
    else{
	while(p != 0 && q != 0)
	    if(p->Car() != q->Car())
		return false;
	    else{
		p = p->Cdr();
		q = q->Cdr();
	    }

	return p == 0 && q == 0;
    }
}

bool Equal(Ptree* p, Ptree* q)
{
    if(p == q)
	return true;
    else if(p == 0 || q == 0)
	return false;
    else if(p->IsLeaf() || q->IsLeaf())
	return Eq(p, q);
    else
	return Equal(p->Car(), q->Car()) && Equal(p->Cdr(), q->Cdr());
}

Ptree* Last(Ptree* p)	// return the last cons cell.
{
    Ptree* next;
    if(p == 0)
	return 0;

    while((next = p->Cdr()) != 0)
	p = next;

    return p;
}

Ptree* First(Ptree* p)
{
    if (!p) return 0;
    return p->Car();
}

Ptree* Rest(Ptree* p)
{
    if(!p) return 0;
    return p->Cdr();
}

Ptree* Second(Ptree* p)
{
    if (p != 0) {
	p = p->Cdr();
	if (p != 0)
	    return p->Car();
    }

    return p;
}

Ptree* Third(Ptree* p)
{
    if(p != 0){
	p = p->Cdr();
	if(p != 0){
	    p = p->Cdr();
	    if(p != 0)
		return p->Car();
	}
    }

    return p;
}

/*
  Nth(lst, 0) is equivalent to First(lst).
*/
Ptree* Nth(Ptree* p, int n)
{
    while(p != 0 && n-- > 0)
	p = p->Cdr();

    if(p != 0)
	return p->Car();
    else
	return p;
}

int Length(Ptree* p)
{
    int i = 0;

    if(p != 0 && p->IsLeaf())
	return -2;	/* p is not a pair */

    while(p != 0){
	++i;
	if(p->IsLeaf())
	    return -1;	/* p is a pair, but not a list. */
	else
	    p = p->Cdr();
    }

    return i;
}

Ptree* ListTail(Ptree* p, int k)
{
    while(p != 0 && k-- > 0)
	p = p->Cdr();

    return p;
}

Ptree* List(Ptree* p)
{
    return new NonLeaf(p, 0);
}

Ptree* List()
{
    return 0;
}

Ptree* List(Ptree* p, Ptree* q)
{
    return new NonLeaf(p, new NonLeaf(q, 0));
}

Ptree* List(Ptree* p1, Ptree* p2, Ptree* p3)
{
    return new NonLeaf(p1, new NonLeaf(p2, new NonLeaf(p3, 0)));
}

Ptree* List(Ptree* p1, Ptree* p2, Ptree* p3, Ptree* p4)
{
    return new NonLeaf(p1, List(p2, p3, p4));
}

Ptree* List(Ptree* p1, Ptree* p2, Ptree* p3, Ptree* p4, Ptree* p5)
{
    return Nconc(List(p1, p2), List(p3, p4, p5));
}

Ptree* List(Ptree* p1, Ptree* p2, Ptree* p3, Ptree* p4, Ptree* p5,
		   Ptree* p6)
{
    return Nconc(List(p1, p2, p3), List(p4, p5, p6));
}

Ptree* List(Ptree* p1, Ptree* p2, Ptree* p3, Ptree* p4, Ptree* p5,
		   Ptree* p6, Ptree* p7)
{
    return Nconc(List(p1, p2, p3), List(p4, p5, p6, p7));
}

Ptree* List(Ptree* p1, Ptree* p2, Ptree* p3, Ptree* p4, Ptree* p5,
		   Ptree* p6, Ptree* p7, Ptree* p8)
{
    return Nconc(List(p1, p2, p3, p4), List(p5, p6, p7, p8));
}

Ptree* Append(Ptree* p, Ptree* q)
{
    Ptree *result, *tail;

    if(p == 0)
	if(q->IsLeaf())
	    return Cons(q, 0);
	else
	    return q;

    result = tail = Cons(p->Car(), 0);
    p = p->Cdr();
    while(p != 0){
	Ptree* cell = Cons(p->Car(), 0);
	tail->SetCdr(cell);
	tail = cell;
	p = p->Cdr();
    }

    if(q != 0 && q->IsLeaf())
	tail->SetCdr(Cons(q, 0));
    else
	tail->SetCdr(q);

    return result;
}

/*
  ReplaceAll() substitutes SUBST for all occurences of ORIG in LIST.
  It recursively searches LIST for ORIG.
*/
Ptree* ReplaceAll(Ptree* list, Ptree* orig, Ptree* subst)
{
    if(Eq(list, orig))
	return subst;
    else if(list == 0 || list->IsLeaf())
	return list;
    else{
	PtreeArray newlist;
	bool changed = false;
	Ptree* rest = list;
	while(rest != 0){
	    Ptree* p = rest->Car();
	    Ptree* q = ReplaceAll(p, orig, subst);
	    newlist.Append(q);
	    if(p != q)
		changed = true;

	    rest = rest->Cdr();
	}

	if(changed)
	    return newlist.All();
	else
	    return list;
    }
}

Ptree* Subst(Ptree* newone, Ptree* old, Ptree* tree)
{
    if(old == tree)
	return newone;
    else if(tree== 0 || tree->IsLeaf())
	return tree;
    else{
	Ptree* head = tree->Car();
	Ptree* head2 = Subst(newone, old, head);
	Ptree* tail = tree->Cdr();
	Ptree* tail2 = (tail == 0) ? tail : Subst(newone, old, tail);
	if(head == head2 && tail == tail2)
	    return tree;
	else
	    return Cons(head2, tail2);
    }
}

Ptree* Subst(Ptree* newone1, Ptree* old1, Ptree* newone2, Ptree* old2,
		    Ptree* tree)
{
    if(old1 == tree)
	return newone1;
    else if(old2 == tree)
	return newone2;
    else if(tree == 0 || tree->IsLeaf())
	return tree;
    else{
	Ptree* head = tree->Car();
	Ptree* head2 = Subst(newone1, old1, newone2, old2, head);
	Ptree* tail = tree->Cdr();
	Ptree* tail2 = (tail == 0) ? tail
			: Subst(newone1, old1, newone2, old2, tail);
	if(head == head2 && tail == tail2)
	    return tree;
	else
	    return Cons(head2, tail2);
    }
}

Ptree* Subst(Ptree* newone1, Ptree* old1, Ptree* newone2, Ptree* old2,
		    Ptree* newone3, Ptree* old3, Ptree* tree)
{
    if(old1 == tree)
	return newone1;
    else if(old2 == tree)
	return newone2;
    else if(old3 == tree)
	return newone3;
    else if(tree == 0 || tree->IsLeaf())
	return tree;
    else{
	Ptree* head = tree->Car();
	Ptree* head2 = Subst(newone1, old1, newone2, old2,
			     newone3, old3, head);
	Ptree* tail = tree->Cdr();
	Ptree* tail2 = (tail == 0) ? tail :
			Subst(newone1, old1, newone2, old2,
			      newone3, old3, tail);
	if(head == head2 && tail == tail2)
	    return tree;
	else
	    return Cons(head2, tail2);
    }
}

// ShallowSubst() doesn't recursively apply substitution to a subtree.

Ptree* ShallowSubst(Ptree* newone, Ptree* old, Ptree* tree)
{
    if(old == tree)
	return newone;
    else if(tree== 0 || tree->IsLeaf())
	return tree;
    else{
	Ptree *head, *head2;
	head = tree->Car();
	if(old == head)
	    head2 = newone;
	else
	    head2 = head;

	Ptree* tail = tree->Cdr();
	Ptree* tail2 = (tail == 0) ? tail : ShallowSubst(newone, old, tail);
	if(head == head2 && tail == tail2)
	    return tree;
	else
	    return Cons(head2, tail2);
    }
}

Ptree* ShallowSubst(Ptree* newone1, Ptree* old1,
			   Ptree* newone2, Ptree* old2, Ptree* tree)
{
    if(old1 == tree)
	return newone1;
    else if(old2 == tree)
	return newone2;
    else if(tree == 0 || tree->IsLeaf())
	return tree;
    else{
	Ptree *head, *head2;
	head = tree->Car();
	if(old1 == head)
	    head2 = newone1;
	else if(old2 == head)
	    head2 = newone2;
	else
	    head2 = head;

	Ptree* tail = tree->Cdr();
	Ptree* tail2 = (tail == 0) ? tail :
			ShallowSubst(newone1, old1, newone2, old2, tail);
	if(head == head2 && tail == tail2)
	    return tree;
	else
	    return Cons(head2, tail2);
    }
}

Ptree* ShallowSubst(Ptree* newone1, Ptree* old1,
			   Ptree* newone2, Ptree* old2,
			   Ptree* newone3, Ptree* old3, Ptree* tree)
{
    if(old1 == tree)
	return newone1;
    else if(old2 == tree)
	return newone2;
    else if(old3 == tree)
	return newone3;
    else if(tree == 0 || tree->IsLeaf())
	return tree;
    else{
	Ptree *head, *head2;
	head = tree->Car();
	if(old1 == head)
	    head2 = newone1;
	else if(old2 == head)
	    head2 = newone2;
	else if(old3 == head)
	    head2 = newone3;
	else
	    head2 = head;

	Ptree* tail = tree->Cdr();
	Ptree* tail2 = (tail == 0) ? tail :
			ShallowSubst(newone1, old1, newone2, old2,
				     newone3, old3, tail);
	if(head == head2 && tail == tail2)
	    return tree;
	else
	    return Cons(head2, tail2);
    }
}

Ptree* ShallowSubst(Ptree* newone1, Ptree* old1,
			   Ptree* newone2, Ptree* old2,
			   Ptree* newone3, Ptree* old3,
			   Ptree* newone4, Ptree* old4, Ptree* tree)
{
    if(old1 == tree)
	return newone1;
    else if(old2 == tree)
	return newone2;
    else if(old3 == tree)
	return newone3;
    else if(old4 == tree)
	return newone4;
    else if(tree == 0 || tree->IsLeaf())
	return tree;
    else{
	Ptree *head, *head2;
	head = tree->Car();
	if(old1 == head)
	    head2 = newone1;
	else if(old2 == head)
	    head2 = newone2;
	else if(old3 == head)
	    head2 = newone3;
	else if(old4 == head)
	    head2 = newone4;
	else
	    head2 = head;

	Ptree* tail = tree->Cdr();
	Ptree* tail2 =  (tail == 0) ? tail :
			ShallowSubst(newone1, old1, newone2, old2,
				     newone3, old3, newone4, old4, tail);
	if(head == head2 && tail == tail2)
	    return tree;
	else
	    return Cons(head2, tail2);
    }
}

Ptree* SubstSublist(Ptree* newsub, Ptree* oldsub, Ptree* lst)
{
    if(lst == oldsub)
	return newsub;
    else
	return Cons(lst->Car(), SubstSublist(newsub, oldsub, lst->Cdr()));
}

/** Mutably makes list <code>q</code> the last element of list <code>p</code>

    @pre <tt><pre>
    
    p=[p1,p2,p3]       q=[q1,q2,q3]
             
    p--> cons          q--> cons
         /  \               /  \
       p1  cons           q1  cons
           /  \               /  \
         p2  cons           q2  cons
             /  \               /  \
           p3   nil           q3   nil
        
    </pre></tt>

    @post <tt><pre>
    
    p=[p1,p2,p3,[q1,q2,q3]]
             
    p--> cons
         /  \             
       p1  cons           
           /  \           
         p2  cons         
             /  \
           p3   cons (new one)
                /  \
        q--> cons  nil
             /  \
           q1  cons     
               /  \     
             q2  cons   
                 /  \   
               q3   nil 
    </pre></tt>
*/
Ptree* Snoc(Ptree* p, Ptree* q)
{
    return Nconc(p, Cons(q, 0));
}

/** Mutable append.

    @pre <tt><pre>
             
    p=[p1,p2,p3]       q=[q1,q2,q3]

    p--> cons          q--> cons
         /  \               /  \
       p1  cons           q1  cons
           /  \               /  \
         p2  cons           q2  cons
             /  \               /  \
           p3   nil           q3   nil
        
    </pre></tt>

    @post <tt><pre>
             
    p=[p1,p2,p3,q1,q2,q3]
    
    p--> cons             
         /  \             
       p1  cons           
           /  \           
         p2  cons         
             /  \         
           p3  cons <-- q           
               /  \
             q1  cons
                 /  \
               q2  cons
                   /  \ 
                 q3   nil
    </pre></tt>

  */
Ptree* Nconc(Ptree* p, Ptree* q)
{
    if(p == 0)
	return q;
    else{
	Last(p)->SetCdr(q);
	return p;
    }
}

Ptree* Nconc(Ptree* p, Ptree* q, Ptree* r)
{
    return Nconc(p, Nconc(q, r));
}

Ptree* Ca_ar(Ptree* p)
{
    while(p != 0 && !p->IsLeaf())
        p = p->Car();
    return p;
}

char* LeftMost(Ptree* p)
{
    if (! p) {
        return 0;
    }
    
    if(p->IsLeaf()) {
        return p->GetPosition();
    }
    else{
	while (p) {
	    char* i = LeftMost(p->Car());
	    if (i) {
		return i;
	    }
	    else {
		p = p->Cdr();
            }
	}
	return 0;
    }
}

char* RightMost(Ptree* p)
{
    if(! p) {
	return 0;
    }
    
    if(p->IsLeaf()) {
	return p->GetPosition() + p->GetLength();
    }
    else{
	int n = Length(p);
	while (n > 0) {
	    char* i = RightMost(Nth(p,--n));
	    if (i) {
		return i;
            }
	}
	return 0;
    }
}



const int MAX = 32;

Ptree** resultsArgs[MAX];
int resultsIndex;

static int CountArgs(char* pat);
static char* SkipSpaces(char* pat);
static char* MatchList(Ptree* list, char* pat);
static char* MatchWord(Ptree* list, char* pat);


static int CountArgs(char* pat)
{
    int n = 0;

    for(char c = *pat; c != '\0'; c = *++pat)
        if(c == '%'){
            c = *++pat;
            if(c == '?' || c == 'r')
                ++n;
        }

    return n;
}

static char* MatchPat(Ptree* list, char* pat)
{
    switch(*pat){
    case '[' :          /* [] means 0 */
        if(list != 0 && list->IsLeaf())
            return 0;
        else
            return MatchList(list, pat + 1);
    case '%' :
        switch(pat[1]){
        case '?' :
            *resultsArgs[resultsIndex++] = list;
            return(pat + 2);
        case '*' :
            return(pat + 2);
        case '_' :
        case 'r' :      /* %_ and %r must be appear in a list */
            return 0;
        default :
            break;
        }
    }

    if(list != 0 && list->IsLeaf())
        return MatchWord(list, pat);
    else
        return 0;
}

static char* MatchList(Ptree* list, char* pat)
{
    char c, d;
    pat = SkipSpaces(pat);
    while((c = *pat) != '\0'){
        if(c == ']')
            if(list == 0)
                return(pat + 1);
            else
                return 0;
        else if(c == '%' && (d = pat[1], (d == 'r' || d == '_'))){
            /* %r or %_ */
            if(d == 'r') 
                *resultsArgs[resultsIndex++] = list;

            list = 0;
            pat = pat + 2;
        }
        else if(list == 0)
            return 0;
        else{
            pat = MatchPat(list->Car(), pat);
            if(pat == 0)
                return 0;

            list = list->Cdr();
        }

        pat = SkipSpaces(pat);
    }

    TheErrorLog().Report(MopMsg(Msg::Fatal, "Ptree::Match()", "unmatched bracket"));
    return 0;
}

static char* MatchWord(Ptree* list, char* pat)
{
    char* str = list->GetPosition();
    int str_len = list->GetLength();

    for(int j = 0; ; ++pat){
        char c = *pat;
        switch(c){
        case '\0' :
        case ' ' :
        case '\t' :
        case '[' :
        case ']' :
            if(j == str_len)
                return pat;
            else
                return 0;
        case '%' :
            c = *++pat;
            switch(c){
            case '[' :
            case ']' :
            case '%' :
                if(j >= str_len || c != str[j++])
                    return 0;

                break;
            default :
                if(j == str_len)
                    return pat;
                else
                    return 0;
            }
            break;
        default :
            if(j >= str_len || c != str[j++])
                return 0;
        }
    }
}

static char* SkipSpaces(char* pat)
{
    while(*pat == ' ' || *pat == '\t')
        ++pat;

    return pat;
}


bool Match(Ptree* list, char* pattern, ...)
{
    va_list args;
    int n = CountArgs(pattern);
    if(n >= MAX)
	TheErrorLog().Report(MopMsg(Msg::Fatal, "Ptree::Match()", "bomb! too many arguments"));

    va_start(args, pattern);
    for(int i = 0; i < n; ++i)
	resultsArgs[i] = va_arg(args, Ptree**);

    va_end(args);

    char* pat = pattern;
    resultsIndex = 0;
    pat = SkipSpaces(pat);
    pat = MatchPat(list, pat);
    if(pat == 0)
	return false;
    else{
	pat = SkipSpaces(pat);
	if(*pat == '\0')
	    return true;
	else{
	    TheErrorLog().Report(MopMsg(Msg::Warning, "Ptree::Match()", std::string("forgotten [ ]?") + pattern));
	    return false;
	}
    }
}


Ptree* GenSym()
{
    static char head[] = "_sym";
    static int seed = 1;
    int len1, len2;

    IntegerToString(seed, len1);

    static unsigned time = 0;
    time++;
    unsigned rnum = time & 0xffff;
    char* num = IntegerToString(rnum, len2);

    int size = len1 + len2 + sizeof(head) - 1 + 1;
    char* name = new (GC) char[size];
    memmove(name, head, sizeof(head) - 1);
    memmove(&name[sizeof(head) - 1], num, len2);
    name[sizeof(head) - 1 + len2] = '_';
    num = IntegerToString(seed++, len1);
    memmove(&name[sizeof(head) - 1 + len2 + 1], num, len1);
    return new Leaf(name, size);
}



} // namespace PtreeUtil

}

