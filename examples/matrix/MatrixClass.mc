/* -*-Mode: C++;-*-

  Copyright (C) 1997,1998 Shigeru Chiba, University of Tsukuba.

  Permission to use, copy, distribute and modify this software and   
  its documentation for any purpose is hereby granted without fee,        
  provided that the above copyright notice appear in all copies and that 
  both that copyright notice and this permission notice appear in 
  supporting documentation.

  Shigeru Chiba makes no representations about the suitability of this 
  software for any purpose.  It is provided "as is" without express or
  implied warranty.

  July 1997: rewritten by Toru Takimoto for version 2.5.
*/
/*
  Copyright (c) 1995, 1996 Xerox Corporation.
  All Rights Reserved.

  Use and copying of this software and preparation of derivative works
  based upon this software are permitted. Any copy of this software or
  of any derivative work must include the above copyright notice of
  Xerox Corporation, this paragraph and the one after it.  Any
  distribution of this software or derivative works must comply with all
  applicable United States export control laws.

  This software is made available AS IS, and XEROX CORPORATION DISCLAIMS
  ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
  PURPOSE, AND NOTWITHSTANDING ANY OTHER PROVISION CONTAINED HEREIN, ANY
  LIABILITY FOR DAMAGES RESULTING FROM THE SOFTWARE OR ITS USE IS
  EXPRESSLY DISCLAIMED, WHETHER ARISING IN CONTRACT, TORT (INCLUDING
  NEGLIGENCE) OR STRICT LIABILITY, EVEN IF XEROX CORPORATION IS ADVISED
  OF THE POSSIBILITY OF SUCH DAMAGES.
*/

#include <opencxx/mop.h>

using namespace Opencxx;

const char* SIZE = "N";
const int MAX = 32;

static struct { Ptree* expr; int k; } termTable[MAX];
static int numOfTerms;

static struct { Ptree *lexpr, *rexpr; int k; bool scalar; } mulTermTable[MAX];
static int numOfMulTerms;

static Ptree* DoOptimize0(Ptree*);
static Ptree* DoOptimize1(Ptree*);
static Ptree* MakeInlineExpr(Ptree*);
static bool ParseTerms(Environment*, Ptree*, int);
static bool IsScalarProduct(Environment*, Ptree*);
static int NumOfVectorProducts();
static int IndexOfVectorProducts();

class MatrixClass : public Class {
public:
    Ptree* TranslateInitializer(Environment*, Ptree*, Ptree*);
    Ptree* TranslateAssign(Environment*, Ptree*, Ptree*, Ptree*);
    Ptree* TranslateUserStatement(Environment*, Ptree*, Ptree*,
				  Ptree*, Ptree*);
    static bool Initialize();
};

bool MatrixClass::Initialize()
{
    RegisterNewWhileStatement("forall");
    return true;
}

Ptree* MatrixClass::TranslateInitializer(Environment* env, Ptree* name,
					 Ptree* expr)
{
    Ptree* sep = PtreeUtil::First(expr);
    Ptree* val = PtreeUtil::Second(expr);
    if(PtreeUtil::Eq(sep,'=') && PtreeUtil::Match(val, "[{ %* }]")) {
	Ptree* tmp = PtreeUtil::GenSym();
	InsertBeforeStatement(env, Ptree::qMake("double `tmp`[] = `val`;\n"));
	return Ptree::Make("= %p", tmp);
    }
    else
        return Class::TranslateInitializer(env, name, expr);
}

Ptree* MatrixClass::TranslateUserStatement(Environment* env, Ptree* object,
					   Ptree* op,
					   Ptree* keyword, Ptree* rest)
{
    Ptree *tmp, *body, *index;

    if(!PtreeUtil::Match(rest, "[([%?]) %?]", &tmp, &body)){
	ErrorMessage(env, "invalid forall statement", nil, keyword);
	return nil;
    }

    index = PtreeUtil::GenSym();
    return Ptree::qMake(
	"for(int `index` = 0; `index` < `SIZE` * `SIZE`; ++`index`){\n"
	"    double& `tmp` = `object``op` element[`index`];\n"
	"    `TranslateExpression(env, body)` }\n");
}

Ptree* MatrixClass::TranslateAssign(Environment* env, Ptree* object,
				    Ptree* op, Ptree* expr)
{
    if(!object->IsLeaf() || ! PtreeUtil::Eq(op,'='))
	goto giveup;

    if(expr->IsLeaf())	// e.g. a = b;
	goto giveup;

    numOfTerms = 0;
    numOfMulTerms = 0;
    if(!ParseTerms(env, expr, 1))
	goto giveup;

    switch(NumOfVectorProducts()){
    case 0 :
	return DoOptimize0(object);
    case 1 :
	return DoOptimize1(object);
    default :
	goto giveup;	// give up optimization if the number of
			// vector products are more than one because
			// the gain by inlining is relatively zero.
    }

giveup:
    return Class::TranslateAssign(env, object, op, expr);
}

// Optimization for expressions that include no vector product.

static Ptree* DoOptimize0(Ptree* object)
{
    Ptree* index = PtreeUtil::GenSym();
    return Ptree::qMake(
	"for(int `index` = 0; `index` < `SIZE` * `SIZE`; ++`index`)\n"
	"    `object`.element[`index`] = `MakeInlineExpr(index)`;");
}

// Optimization for expressions that include only one vector product.

static Ptree* DoOptimize1(Ptree* object)
{
    char op;
    Ptree* index1 = PtreeUtil::GenSym();
    Ptree* index2 = PtreeUtil::GenSym();
    Ptree* index3 = PtreeUtil::GenSym();
    Ptree* tmp = PtreeUtil::GenSym();
    Ptree* index4 = PtreeUtil::GenSym();

    int v = IndexOfVectorProducts();
    if(mulTermTable[v].k > 0)
	op = '+';
    else
	op = '-';

    Ptree* inlined_expr = Ptree::qMake(
	"`mulTermTable[v].lexpr`.element[`index1` * `SIZE` + `index3`]"
	"* `mulTermTable[v].rexpr`.element[`index3` * `SIZE` + `index2`]");

    return Ptree::qMake(
	"for(int `index1` = 0; `index1` < `SIZE`; ++`index1`)\n"
	"  for(int `index2` = 0; `index2` < `SIZE`; ++`index2`){\n"
	"    double `tmp` = 0.0;\n"
	"    for(int `index3` = 0; `index3` < `SIZE`; ++`index3`)\n"
	"      `tmp` += `inlined_expr`;\n"
	"    int `index4` = `index1` * `SIZE` + `index2`;\n"
	"    `object`.element[`index4`]"
	"       = `MakeInlineExpr(index4)``op``tmp`;}\n");
}

static Ptree* MakeInlineExpr(Ptree* index_var)
{
    int i;
    Ptree* expr;
    Ptree* inline_expr = nil;

    for(i = numOfMulTerms - 1; i >= 0; --i)
	if(mulTermTable[i].scalar){
	    char op;
	    if(mulTermTable[i].k > 0)
		op = '+';
	    else
		op = '-';

	    expr = Ptree::Make("%c %p * %p.element[%p]",
			       op, mulTermTable[i].lexpr,
			       mulTermTable[i].rexpr, index_var);
	    inline_expr = PtreeUtil::Cons(expr, inline_expr);
	}

    for(i = numOfTerms - 1; i > 0; --i){
	char op;
	if(termTable[i].k > 0)
	    op = '+';
	else
	    op = '-';

	expr = Ptree::Make("%c %p.element[%p]",
			   op, termTable[i].expr, index_var);
	inline_expr = PtreeUtil::Cons(expr, inline_expr);
    }

    if(numOfTerms > 0){
	if(termTable[0].k > 0)
	    expr = Ptree::Make("%p.element[%p]",
			       termTable[0].expr, index_var);
	else
	    expr = Ptree::Make("- %p.element[%p]",
			       termTable[0].expr, index_var);

	inline_expr = PtreeUtil::Cons(expr, inline_expr);
    }

    return inline_expr;
}

static bool ParseTerms(Environment* env, Ptree* expr, int k)
{
    Ptree* lexpr;
    Ptree* rexpr;

    if(expr->IsLeaf()){
	termTable[numOfTerms].expr = expr;
	termTable[numOfTerms].k = k;
	++numOfTerms;
	return true;
    }
    else if(PtreeUtil::Match(expr, "[%? + %?]", &lexpr, &rexpr))
	return ParseTerms(env, lexpr, k) && ParseTerms(env, rexpr, k);
    else if(PtreeUtil::Match(expr, "[%? - %?]", &lexpr, &rexpr))
	return ParseTerms(env, lexpr, k) && ParseTerms(env, rexpr, -k);
    else if(PtreeUtil::Match(expr, "[( %? )]", &lexpr))
	return ParseTerms(env, lexpr, k);
    else if(PtreeUtil::Match(expr, "[- %?]", &rexpr))
	return ParseTerms(env, rexpr, -k);
    else if(PtreeUtil::Match(expr, "[%? * %?]", &lexpr, &rexpr))
	if(lexpr->IsLeaf() && rexpr->IsLeaf()){
	    mulTermTable[numOfMulTerms].lexpr = lexpr;
	    mulTermTable[numOfMulTerms].rexpr = rexpr;
	    mulTermTable[numOfMulTerms].k = k;
	    mulTermTable[numOfMulTerms].scalar = IsScalarProduct(env, lexpr);
	    ++numOfMulTerms;
	    return true;
	}
	else
	    return false;
    else
	return false;
}

// Is the expression P a scalar value?

static bool IsScalarProduct(Environment* env, Ptree* p)
{
    TypeInfo t;

    if(env->Lookup(p, t))
	if(t.IsBuiltInType())
	    return true;

    return false;
}

// How many vector products are included in the right-side expression?

static int NumOfVectorProducts()
{
    int n = 0;

    for(int i = 0; i < numOfMulTerms; ++i)
	if(!mulTermTable[i].scalar)
	    ++n;

    return n;
}

static int IndexOfVectorProducts()
{
    int n = 0;

    for(int i = 0; i < numOfMulTerms; ++i)
	if(!mulTermTable[i].scalar)
	    return i;

    return 0;
}
