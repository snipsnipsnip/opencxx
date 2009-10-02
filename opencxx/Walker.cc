//@beginlicenses@
//@license{chiba-tokyo}{}@
//@license{contributors}{}@
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
//  Other Contributors (see file AUTHORS) make(s) no representations about the suitability of this
//  software for any purpose. It is provided "as is" without express or implied
//  warranty.
//  
//  Copyright (C)  Other Contributors (see file AUTHORS)
//
//@endlicenses@

#include <opencxx/Walker.h>
#include <cassert>
#include <string>
#include <strstream>
#include <opencxx/Bind.h>
#include <opencxx/parser/Encoding.h>
#include <opencxx/EncodingUtil.h>
#include <opencxx/Environment.h>
#include <opencxx/parser/ErrorLog.h>
#include <opencxx/GenericMsg.h>
#include <opencxx/Metaclass.h>
#include <opencxx/TheMetaclassRegistry.h>
#include <opencxx/parser/MopMsg.h>
#include <opencxx/parser/Parser.h>
#include <opencxx/PtreeTypeUtil.h>
#include <opencxx/parser/PtreeUtil.h>
#include <opencxx/parser/SourceLocation.h>
#include <opencxx/parser/TheErrorLog.h>
#include <opencxx/TypeInfo.h>
#include <opencxx/parser/deprecated.h>
#include <opencxx/memberAll.h>
#include <opencxx/parser/PtreeBlock.h>
#include <opencxx/parser/PtreeBrace.h>
#include <opencxx/parser/PtreeClassBody.h>
#include <opencxx/parser/PtreeClassSpec.h>
#include <opencxx/parser/PtreeDeclaration.h>
#include <opencxx/parser/PtreeDeclarator.h>
#include <opencxx/parser/PtreeExprStatement.h>
#include <opencxx/parser/PtreeFstyleCastExpr.h>
#include <opencxx/parser/PtreeLinkageSpec.h>
#include <opencxx/parser/PtreeNamespaceSpec.h>
#include <opencxx/parser/PtreeNamespaceAlias.h>
#include <opencxx/parser/PtreeTemplateDecl.h>
#include <opencxx/parser/PtreeTypedef.h>
#include <opencxx/parser/PtreeUsing.h>
#include <opencxx/parser/ptreeAll.h>
#include <opencxx/parser/token-names.h>

using namespace std;

BEGIN_OPENCXX_NAMESPACE

Parser* Walker::default_parser = 0;
char* Walker::default_metaclass = 0;
char* Walker::default_template_metaclass = 0;

Walker::Walker(Parser* p)
{
    env = new Environment(this);
    parser = p;
    if(default_parser == 0)
	default_parser = p;
}

Walker::Walker(Parser* p, Environment* e)
{
    env = new Environment(e, this);
    parser = p;
    if(default_parser == 0)
	default_parser = p;
}

Walker::Walker(Environment* e)
{
    env = new Environment(e, this);
    if(default_parser == 0)
	TheErrorLog().Report(MopMsg(Msg::Fatal, "Walker::Walker()", "no default parser"));

    parser = default_parser;
}

Walker::Walker(Walker* w)
{
    env = w->env;
    parser = w->parser;
}

void Walker::NewScope()
{
    env = new Environment(env);
}

void Walker::NewScope(Environment* e)
{
    if (e != 0)
	env = e;
    else
	env = new Environment(env);
}

void Walker::NewScope(Class* metaobject)
{
    env = new Environment(env);
    if(metaobject != 0)
	metaobject->SetEnvironment(env);
}

Environment* Walker::ExitScope()
{
    Environment* old_env = env;
    env = old_env->GetOuterEnvironment();
    return old_env;
}

void Walker::RecordBaseclassEnv(Ptree* bases)
{
    while(bases != 0){
	bases = bases->Cdr();		// skip : or ,
	Ptree* base_class = PtreeUtil::Last(bases->Car())->Car();
	Class* metaobject = env->LookupClassMetaobject(base_class);
	if(metaobject != 0){
	    Environment* e = metaobject->GetEnvironment();
	    if(e != 0)
		env->AddBaseclassEnv(e);
	}

	bases = bases->Cdr();
    }
}

Walker::NameScope Walker::ChangeScope(Environment* e)
{
    NameScope scope;
    scope.walker = e->GetWalker();
    e->SetWalker(this);
    scope.env = env;
    env = e;
    return scope;
}

void Walker::RestoreScope(Walker::NameScope& scope)
{
    env->SetWalker(scope.walker);
    env = scope.env;
}

bool Walker::IsClassWalker()
{
    return false;
}

Ptree* Walker::Translate(Ptree* p)
{
    if(p == 0)
	return p;
    else
	return p->Translate(this);
}

void Walker::Typeof(Ptree* p, TypeInfo& t)
{
    if(p != 0)
	p->Typeof(this, t);
}

// default translation

Ptree* Walker::TranslatePtree(Ptree* p)
{
    return p;
}

void Walker::TypeofPtree(Ptree*, TypeInfo& t)
{
    t.Unknown();
}

// translation for each class of node

Ptree* Walker::TranslateTypedef(Ptree* def)
{
    Ptree *tspec, *tspec2;

    tspec = PtreeUtil::Second(def);
    tspec2 = TranslateTypespecifier(tspec);
    env->RecordTypedefName(PtreeUtil::Third(def));
    if(tspec == tspec2)
	return def;
    else
	return new PtreeTypedef(PtreeUtil::First(def),
				PtreeUtil::List(tspec2,
					    PtreeUtil::ListTail(def, 2)));
}

Ptree* Walker::TranslateTemplateDecl(Ptree* def)
{
    Ptree* body = PtreeUtil::Nth(def, 4);
    Ptree* class_spec = PtreeTypeUtil::GetClassTemplateSpec(body);
    if(class_spec->IsA(ntClassSpec))
	return TranslateTemplateClass(def, class_spec);
    else
	return TranslateTemplateFunction(def, body);
}

Ptree* Walker::TranslateExternTemplate(Ptree* def)
{
    return def;
}

Ptree* Walker::TranslateTemplateClass(Ptree* temp_def, Ptree* class_spec)
{
    Ptree* userkey;
    Ptree* class_def;

    if(class_spec->Car()->IsLeaf()){
	userkey = 0;
	class_def = class_spec;
    }
    else{
	userkey = class_spec->Car();
	class_def = class_spec->Cdr();
    }

    Class* metaobject = 0;
    if(PtreeUtil::Length(class_def) == 4)
	metaobject = MakeTemplateClassMetaobject(temp_def, userkey, class_def);

    env->RecordTemplateClass(class_spec, metaobject);
    Ptree* class_spec2 = TranslateClassSpec(class_spec, userkey, class_def,
					    metaobject);
    if(class_spec == class_spec2)
	return temp_def;
    else
	return new PtreeTemplateDecl(temp_def->Car(),
				     PtreeUtil::Subst(class_spec2, class_spec,
						  temp_def->Cdr()));
}

Class* Walker::MakeTemplateClassMetaobject(Ptree* def, Ptree* userkey,
					   Ptree* class_def)
{
    Class* metaobject = LookupMetaclass(def, userkey, class_def, true);
    if(metaobject == 0 && default_template_metaclass != 0){
	metaobject
	    = TheMetaclassRegistry().New(default_template_metaclass, def, 0);
	if(metaobject == 0)
	    TheErrorLog().Report(MopMsg(Msg::Fatal, 
	        (string("the default template metaclass cannot be loaded: ")
	        +default_template_metaclass)));
    }

    if(metaobject == 0)
	metaobject = new TemplateClass;
    else
	if(metaobject->AcceptTemplate())
	    return metaobject;
	else{

	    ErrorMessage("the specified metaclass is not for templates.",
			 0, def);
	    metaobject = new TemplateClass;
	}

    metaobject->InitializeInstance(def, 0);
    return metaobject;
}

Ptree* Walker::TranslateTemplateFunction(Ptree* temp_def, Ptree* fun)
{
    env->RecordTemplateFunction(temp_def, fun);
    return temp_def;
}

Ptree* Walker::TranslateTemplateInstantiation(Ptree *inst_spec)
{
    Ptree* userkey;
    Ptree* class_spec;
    Ptree* full_class_spec = PtreeUtil::First(inst_spec);

    if(full_class_spec->Car()->IsLeaf()){
	userkey = 0;
	class_spec = full_class_spec;
    }
    else{
	userkey = full_class_spec->Car();
	class_spec = full_class_spec->Cdr();
    }

    Class* metaobject = 0;
    metaobject = MakeTemplateInstantiationMetaobject(full_class_spec,
						     userkey, class_spec);
    return TranslateTemplateInstantiation(inst_spec, userkey,
					  class_spec, metaobject);
}

Class* Walker::MakeTemplateInstantiationMetaobject(
    Ptree* full_class_spec, Ptree* userkey, Ptree* class_spec)
{
    // [class [foo [< ... >]]] -> [class foo]
    Ptree* class_name = PtreeUtil::First(PtreeUtil::Second(class_spec));
    Bind* binding = 0;
    if (!env->Lookup(class_name,binding))
	return 0;

    Class* metaobject = 0;
    if (binding->What() != Bind::isTemplateClass) {
	ErrorMessage("not declarated as a template class?!?",
		     0, full_class_spec);
	metaobject = 0;
    }
    else
	metaobject = binding->ClassMetaobject();

    if (metaobject == 0)
	metaobject = new TemplateClass;
    else
	if(metaobject->AcceptTemplate())
	    return metaobject;
	else{
	    ErrorMessage("the specified metaclass is not for templates.",
			 0, full_class_spec);
	    metaobject = new TemplateClass;
	}

    return metaobject;
}

Ptree* Walker::TranslateTemplateInstantiation(Ptree* inst_spec,
		Ptree* userkey, Ptree* class_spec, Class* metaobject)
{
    if (metaobject == 0)
	return inst_spec;
    else {
	Ptree *class_spec2 = TranslateClassSpec(class_spec);
	if (class_spec == class_spec2)
	    return inst_spec;
	else
	    return class_spec2;
    }
}

Ptree* Walker::TranslateMetaclassDecl(Ptree* decl)
{
    env->RecordMetaclassName(decl);
    return decl;
}

Ptree* Walker::TranslateLinkageSpec(Ptree* def)
{
    Ptree* body = PtreeUtil::Third(def);
    Ptree* body2 = Translate(body);
    if(body == body2)
	return def;
    else
	return new PtreeLinkageSpec(PtreeUtil::First(def),
				    PtreeUtil::List(PtreeUtil::Second(def),
						body2));
}

Ptree* Walker::TranslateNamespaceSpec(Ptree* def)
{
    Ptree* name = PtreeUtil::Second(def);
    Ptree* body = PtreeUtil::Third(def);

    Environment* name_space = env->LookupNamespace0(name);
    NewScope(name_space);
    Ptree* body2 = Translate(body);
    Environment* name_space2 = ExitScope();

    if (name_space == 0)	// if this is the first time
	env->RecordNamespace(name_space2, name);

    if(body == body2)
	return def;
    else
	return new PtreeNamespaceSpec(PtreeUtil::First(def),
				      PtreeUtil::List(name, body2));
}

Ptree *Walker::TranslateNamespaceAlias(Ptree *def)
{
  // FIXME: need to store the new alias somewhere...
  return def;
}

Ptree* Walker::TranslateUsing(Ptree* def)
{
    if (((PtreeUsing*)def)->isNamespace())
	env->RecordUsingNamespace(def);
    else
	env->RecordUsing(def);

    return def;
}

Ptree* Walker::TranslateDeclaration(Ptree* def)
{
    Ptree* decls = PtreeUtil::Third(def);
    if(decls->IsA(ntDeclarator))	// if it is a function
	return TranslateFunctionImplementation(def);
    else{
	// if it is a function prototype or a variable declaration.
	Ptree* decls2;
	Ptree* sspec = PtreeUtil::First(def);
	Ptree* sspec2 = TranslateStorageSpecifiers(sspec);
	Ptree* tspec = PtreeUtil::Second(def);
	Ptree* tspec2 = TranslateTypespecifier(tspec);
	if(decls->IsLeaf())	// if it is ";"
	    decls2 = decls;
	else
	    decls2 = TranslateDeclarators(decls);

	if(sspec == sspec2 && tspec == tspec2 && decls == decls2)
	    return def;
	else if(decls2 == 0)
	    return new PtreeDeclaration(0, PtreeUtil::List(0,
							 Class::semicolon_t));
	else
	    return new PtreeDeclaration(sspec2,
					PtreeUtil::ShallowSubst(tspec2, tspec,
							    decls2, decls,
							    def->Cdr()));
    }
}

// TranslateStorageSpecifiers() also deals with inline, virtual, etc.

Ptree* Walker::TranslateStorageSpecifiers(Ptree* spec)
{
    return spec;
}

Ptree* Walker::TranslateDeclarators(Ptree* decls)
{
    return TranslateDeclarators(decls, true);
}

Ptree* Walker::TranslateDeclarators(Ptree* decls, bool record)
{
    using namespace PtreeUtil;
    PtreeArray array;
    bool changed = false;
    Ptree* rest = decls;
    while(rest != 0){
	Ptree *p, *q;
	int len;
	p = q = rest->Car();
	if(p->IsA(ntDeclarator)){
	    Ptree *exp, *exp2;

	    if(record)
		env->RecordDeclarator(p);

	    len = PtreeUtil::Length(p);
	    exp = exp2 = 0;
	    if(len >= 2 && Eq(Nth(p, len - 2), '=')){
		exp = ListTail(p, len - 2);
		exp2 = TranslateAssignInitializer((PtreeDeclarator*)p, exp);
	    }
	    else{
		Ptree* last = Last(p)->Car();
		if(last != 0 && !last->IsLeaf() && PtreeUtil::Eq(last->Car(), '(')){
		    exp = last;
		    exp2 = TranslateInitializeArgs((PtreeDeclarator*)p, last);
		}
	    }

	    q = TranslateDeclarator(false, (PtreeDeclarator*)p);
	    if(exp != exp2){
		// exp2 should be a list, but...
		if(exp2 != 0 && exp2->IsLeaf())
		    exp2 = PtreeUtil::List(exp2);

		if(p == q){
		    q = PtreeUtil::SubstSublist(exp2, exp, p->Cdr());
		    q = new PtreeDeclarator((PtreeDeclarator*)p, p->Car(), q);
		}
		else if(q != 0 && !q->IsLeaf())
		    q = new PtreeDeclarator((PtreeDeclarator*)p, q->Car(),
					    PtreeUtil::Subst(exp2, exp, q->Cdr()));
	    }
	}

	if(q == 0){
	    changed = true;
	    rest = rest->Cdr();
	    if(rest != 0)
		rest = rest->Cdr();
	}
	else{
	    array.Append(q);
	    if(p != q)
		changed = true;

	    rest = rest->Cdr();
	    if(rest != 0){
		array.Append(rest->Car());
		rest = rest->Cdr();
	    }
	}
    }

    if(changed)
	return array.All();
    else
	return decls;
}

Ptree* Walker::TranslateDeclarator(bool record, PtreeDeclarator* decl)
{
    // if record is true, the formal arguments are recorded in the
    // current environment.

    Ptree* args;
    if(PtreeUtil::GetArgDeclList(decl, args)){
	Ptree* args2 = TranslateArgDeclList(record, decl, args);
	if(args == args2)
	    return decl;
	else
	    return new PtreeDeclarator(decl, decl->Car(),
				       PtreeUtil::Subst(args2, args,
						    decl->Cdr()));
    }
    else
	return decl;
}

bool Walker::GetArgDeclList(PtreeDeclarator* decl, Ptree*& args)
{
    Deprecated("Walker::GetArgDeclList(PtreeDeclarator* decl, Ptree*& args)",
               "PtreeUtil::GetArgDeclList(decl, args)");
    return PtreeUtil::GetArgDeclList(decl, args);
}


Ptree* Walker::TranslateArgDeclList(bool record, Ptree*, Ptree* args)
{
    return TranslateArgDeclList2(record, env, false, false, 0, args);
}

// If translate is true, this function eliminates a user-defined keyword.

Ptree* Walker::TranslateArgDeclList2(bool record, Environment* e,
				     bool translate,
				     bool fill_args, int arg_name,
				     Ptree* args)
{
    Ptree* rest;
    Ptree* rest2;

    if(args == 0)
	return args;
    else{
	Ptree *a, *a2;
	a = a2 = args->Car();
	if(args->Cdr() == 0)
	    rest = rest2 = 0;
	else{
	    rest = args->Cdr()->Cdr();	// skip ","
	    rest2 = TranslateArgDeclList2(record, e, translate, fill_args,
					  arg_name + 1, rest);
	    if(rest == rest2)
		rest = rest2 = args->Cdr();
	    else
		rest2 = PtreeUtil::Cons(args->Cdr()->Car(), rest2);
	}

	bool is_ellipsis = a->IsLeaf();		// a may be "..."
	if(is_ellipsis)
	    /* do nothing */;
	else if(a->Car()->IsA(ntUserdefKeyword)){
	    if(record)
		e->RecordDeclarator(PtreeUtil::Third(a));

	    if(translate){
		a2 = a->Cdr();
		if(fill_args)
		    a2 = FillArgumentName(a2, PtreeUtil::Second(a2), arg_name);
	    }
	}
	else if(a->Car()->IsA(REGISTER)){
	    if(record)
		e->RecordDeclarator(PtreeUtil::Third(a));

	    if(translate && fill_args){
		a2 = FillArgumentName(a, PtreeUtil::Third(a), arg_name);
		if(a != a2)
		    a2 = PtreeUtil::Cons(PtreeUtil::First(a), a2);
	    }
	}
	else{
	    if(record)
		e->RecordDeclarator(PtreeUtil::Second(a));

	    if(translate && fill_args)
		a2 = FillArgumentName(a, PtreeUtil::Second(a), arg_name);
	}

	if(a != a2 || rest != rest2)
	    return PtreeUtil::Cons(a2, rest2);
	else
	    return args;
    }
}

Ptree* Walker::FillArgumentName(Ptree* arg, Ptree* d, int arg_name)
{
    Deprecated("Walker::FillArgumentName(Ptree* arg, Ptree* d, int arg_name)",
               "PtreeUtil::FillArgumentName(arg,d,arg_name)");
    return PtreeTypeUtil::FillArgumentName(arg, d, arg_name);
}

Ptree* Walker::TranslateAssignInitializer(PtreeDeclarator*, Ptree* init)
{
    Ptree* exp = PtreeUtil::Second(init);
    Ptree* exp2 = Translate(exp);
    if(exp == exp2)
	return init;
    else
	return PtreeUtil::List(init->Car(), exp2);
}

Ptree* Walker::TranslateInitializeArgs(PtreeDeclarator*, Ptree* init)
{
    return TranslateArguments(init);
}

Ptree* Walker::TranslateFunctionImplementation(Ptree* impl)
{
    Ptree* sspec = PtreeUtil::First(impl);
    Ptree* sspec2 = TranslateStorageSpecifiers(sspec);
    Ptree* tspec = PtreeUtil::Second(impl);
    Ptree* decl = PtreeUtil::Third(impl);
    Ptree* body = PtreeUtil::Nth(impl, 3);
    Ptree* decl2;
    Ptree* body2;

    Ptree* tspec2 = TranslateTypespecifier(tspec);
    Environment* fenv = env->RecordDeclarator(decl);
    if(fenv == 0){
	// reach here if resolving the qualified name fails. error?
	NewScope();
	decl2 = TranslateDeclarator(true, (PtreeDeclarator*)decl);
	body2 = Translate(body);
	ExitScope();
    }
    else{
	NameScope old_env = ChangeScope(fenv);
	NewScope();
	decl2 = TranslateDeclarator(true, (PtreeDeclarator*)decl);
	body2 = Translate(body);
	ExitScope();
	RestoreScope(old_env);
    }

    if(sspec == sspec2 && tspec == tspec2 && decl == decl2 && body == body2)
	return impl;
    else
	return new PtreeDeclaration(sspec2,
				    PtreeUtil::List(tspec2, decl2, body2));
}

Ptree* Walker::RecordArgsAndTranslateFbody(Class*, Ptree* args, Ptree* body)
{
    NewScope();
    TranslateArgDeclList2(true, env, false, false, 0, args);
    Ptree* body2 = TranslateFunctionBody(body);
    ExitScope();
    return body2;
}

Ptree* Walker::TranslateFunctionBody(Ptree* body)
{
    return Translate(body);
}

Ptree* Walker::TranslateBrace(Ptree* block)
{
    PtreeArray array;
    bool changed = false;
    Ptree* body = PtreeUtil::Second(block);
    Ptree* rest = body;
    while(rest != 0){
	Ptree* p = rest->Car();
	Ptree* q = Translate(p);
	array.Append(q);
	if(p != q)
	    changed = true;

	rest = rest->Cdr();
    }

    if(changed)
	return new PtreeBrace(PtreeUtil::First(block), array.All(),
			      PtreeUtil::Third(block));
    else
	return block;
}

Ptree* Walker::TranslateBlock(Ptree* block)
{
    Ptree* block2;

    NewScope();

    PtreeArray array;
    bool changed = false;
    Ptree* body = PtreeUtil::Second(block);
    Ptree* rest = body;
    while(rest != 0){
	Ptree* p = rest->Car();
	Ptree* q = Translate(p);
	array.Append(q);
	if(p != q)
	    changed = true;

	rest = rest->Cdr();
    }

    if(changed)
	block2 = new PtreeBlock(PtreeUtil::First(block), array.All(),
				PtreeUtil::Third(block));
    else
	block2 = block;

    ExitScope();
    return block2;
}

Ptree* Walker::TranslateClassBody(Ptree* block, Ptree* bases,
				  Class* metaobject)
{
    Ptree* block2;

    NewScope(metaobject);
    RecordBaseclassEnv(bases);

    PtreeArray array;
    bool changed = false;
    Ptree* body = PtreeUtil::Second(block);
    Ptree* rest = body;
    while(rest != 0){
	Ptree* p = rest->Car();
	Ptree* q = Translate(p);
	array.Append(q);
	if(p != q)
	    changed = true;

	rest = rest->Cdr();
    }

    if(changed)
	block2 = new PtreeClassBody(PtreeUtil::First(block), array.All(),
				    PtreeUtil::Third(block));
    else
	block2 = block;

    ExitScope();
    return block2;
}

Ptree* Walker::TranslateClassSpec(Ptree* spec)
{
    Ptree* userkey;
    Ptree* class_def;

    if(spec->Car()->IsLeaf()){
	userkey = 0;
	class_def = spec;
    }
    else{
	userkey = spec->Car();
	class_def = spec->Cdr();
    }

    Class* metaobject = 0;
    if(PtreeUtil::Length(class_def) == 4) // "class", classname, inheritance, body
	metaobject = MakeClassMetaobject(spec, userkey, class_def);

    env->RecordClassName(spec->GetEncodedName(), metaobject);
    return TranslateClassSpec(spec, userkey, class_def, metaobject);
}

Class* Walker::MakeClassMetaobject(Ptree* def, Ptree* userkey,
				   Ptree* class_def)
{
    Class* metaobject = LookupMetaclass(def, userkey, class_def, false);

    if(metaobject == 0 && default_metaclass != 0){
	metaobject = TheMetaclassRegistry().New(default_metaclass, class_def,
						0);
	if(metaobject == 0)
	{
	    TheErrorLog().Report(MopMsg(Msg::Fatal, 
	        (string("the default metaclass cannot be loaded: ")
	         +default_metaclass)));
	}
    }

    if(metaobject == 0)
	metaobject = new Class;
    else{
	if(!metaobject->AcceptTemplate())
	    return metaobject;
	else{
	    ErrorMessage("the specified metaclass is for templates.",
			 0, def);
	    metaobject = new Class;
	}
    }

    metaobject->InitializeInstance(class_def, 0);
    return metaobject;
}

void Walker::ChangeDefaultMetaclass(char* name)
{
    default_metaclass = name;
}

void Walker::ChangeDefaultTemplateMetaclass(char* name)
{
    default_template_metaclass = name;
}

// LookupMetaclass() returns 0 if no metaclass is found.

Class* Walker::LookupMetaclass(Ptree* def, Ptree* userkey, Ptree* class_def,
			       bool is_template)
{
    Ptree *mclass, *margs;
    Class* metaobject;

    Ptree* class_name = PtreeUtil::Second(class_def);

    // for bootstrapping
    if(Metaclass::IsBuiltinMetaclass(class_name)){
	metaobject = new Metaclass;
	metaobject->InitializeInstance(def, 0);
	return metaobject;
    }

    Ptree* mdecl = env->LookupMetaclass(class_name);
    if(mdecl != 0){
	mclass = PtreeUtil::Second(mdecl);
	margs = PtreeUtil::Nth(mdecl,4);
	metaobject = TheMetaclassRegistry().New(mclass, def, margs);
	if(metaobject == 0)
	    ErrorMessage("the metaclass is not loaded: ", mclass, class_def);
	else if(userkey != 0)
	    ErrorMessage("the metaclass declaration conflicts"
			 " with the keyword: ", mclass, class_def);

	return metaobject;
    }

    if(userkey != 0){
	mclass = env->LookupClasskeyword(userkey->Car());
	if(mclass == 0)
	    ErrorMessage("invalid keyword: ", userkey, class_def);
	else{
	    metaobject = TheMetaclassRegistry().New(mclass, class_def,
						    PtreeUtil::Third(userkey));
	    if(metaobject == 0)
		ErrorMessage("the metaclass associated with the"
			     " keyword is not loaded: ", userkey, class_def);

	    return metaobject;
	}
    }

    return LookupBaseMetaclass(def, class_def, is_template);
}

Class* Walker::LookupBaseMetaclass(Ptree* def, Ptree* class_def,
				   bool is_template)
{
    Class* metaobject = 0;
    Ptree* bases = PtreeUtil::Third(class_def);
    while(bases != 0){
	bases = bases->Cdr();
	Ptree* base = PtreeUtil::Last(bases->Car())->Car();
	bases = bases->Cdr();
	Class* m = env->LookupClassMetaobject(base);
	if(m != 0){
	    if(metaobject == 0)
		metaobject = m;
	    else if(m == 0 || strcmp(metaobject->MetaclassName(),
				       m->MetaclassName()) != 0){
		ErrorMessage("inherited metaclasses conflict: ",
			     PtreeUtil::Second(class_def), class_def);
		return 0;
	    }
	}
    }

    if(metaobject == 0)
	return 0;

    bool accept_template = metaobject->AcceptTemplate();
    if((is_template && accept_template) || (!is_template && !accept_template))
	return TheMetaclassRegistry().New(metaobject->MetaclassName(),
					  def, 0);
    else
	return 0;
}

Ptree* Walker::TranslateClassSpec(Ptree* spec, Ptree*,
				  Ptree* class_def, Class* metaobject)
{
    if(metaobject == 0)
	return spec;
    else{
	// a class body is specified.
	Ptree* body = PtreeUtil::Nth(class_def,3);
	Ptree* body2 = TranslateClassBody(body, PtreeUtil::Third(class_def),
					  metaobject);
	if(body == body2)
	    return spec;
	else
	    return new PtreeClassSpec(spec->Car(),
				      PtreeUtil::ShallowSubst(body2, body,
							  spec->Cdr()),
				      0, spec->GetEncodedName());
    }
}


Ptree* Walker::TranslateEnumSpec(Ptree* spec)
{
    env->RecordEnumName(spec);
    return spec;
}

Ptree* Walker::TranslateAccessSpec(Ptree* p)
{
    return p;
}

Ptree* Walker::TranslateAccessDecl(Ptree* p)
{
    return p;
}

Ptree* Walker::TranslateUserAccessSpec(Ptree* p)
{
    return p;
}

Ptree* Walker::TranslateIf(Ptree* s)
{
    Ptree* cond = PtreeUtil::Third(s);
    Ptree* cond2 = Translate(cond);
    Ptree* then_part = PtreeUtil::Nth(s,4);
    Ptree* then_part2 = Translate(then_part);
    Ptree* else_part = PtreeUtil::Nth(s,6);
    Ptree* else_part2 = Translate(else_part);

    if(cond == cond2 && then_part == then_part2 && else_part == else_part2)
	return s;
    else{
	Ptree* rest = PtreeUtil::ShallowSubst(cond2, cond, then_part2, then_part,
					  else_part2, else_part, s->Cdr());
	return new PtreeIfStatement(s->Car(), rest);
    }
}

Ptree* Walker::TranslateSwitch(Ptree* s)
{
    Ptree* cond = PtreeUtil::Third(s);
    Ptree* cond2 = Translate(cond);
    Ptree* body = PtreeUtil::Nth(s,4);
    Ptree* body2 = Translate(body);
    if(cond == cond2 && body == body2)
	return s;
    else{
	Ptree* rest = PtreeUtil::ShallowSubst(cond2, cond, body2, body, s->Cdr());
	return new PtreeSwitchStatement(s->Car(), rest);
    }
}

Ptree* Walker::TranslateWhile(Ptree* s)
{
    Ptree* cond = PtreeUtil::Third(s);
    Ptree* cond2 = Translate(cond);
    Ptree* body = PtreeUtil::Nth(s,4);
    Ptree* body2 = Translate(body);
    if(cond == cond2 && body == body2)
	return s;
    else{
	Ptree* rest = PtreeUtil::ShallowSubst(cond2, cond, body2, body, s->Cdr());
	return new PtreeWhileStatement(s->Car(), rest);
    }
}

Ptree* Walker::TranslateDo(Ptree* s)
{
    Ptree* body = PtreeUtil::Second(s);
    Ptree* body2 = Translate(body);
    Ptree* cond = PtreeUtil::Nth(s,4);
    Ptree* cond2 = Translate(cond);
    if(cond == cond2 && body == body2)
	return s;
    else{
	Ptree* rest = PtreeUtil::ShallowSubst(body2, body, cond2, cond, s->Cdr());
	return new PtreeDoStatement(s->Car(), rest);
    }
}

Ptree* Walker::TranslateFor(Ptree* s)
{
    NewScope();
    Ptree* exp1 = PtreeUtil::Third(s);
    Ptree* exp1t = Translate(exp1);
    Ptree* exp2 = PtreeUtil::Nth(s,3);
    Ptree* exp2t = Translate(exp2);
    Ptree* exp3 = PtreeUtil::Nth(s,5);
    Ptree* exp3t = Translate(exp3);
    Ptree* body = PtreeUtil::Nth(s,7);
    Ptree* body2 = Translate(body);
    ExitScope();

    if(exp1 == exp1t && exp2 == exp2t && exp3 == exp3t && body == body2)
	return s;
    else{
	Ptree* rest = PtreeUtil::ShallowSubst(exp1t, exp1, exp2t, exp2,
					  exp3t, exp3, body2, body, s->Cdr());
	return new PtreeForStatement(s->Car(), rest);
    }
}

Ptree* Walker::TranslateTry(Ptree* s)
{
    Ptree* try_block = PtreeUtil::Second(s);
    Ptree* try_block2 = Translate(try_block);

    PtreeArray array;
    Ptree* handlers = s->Cdr()->Cdr();
    bool changed = false;

    while(handlers != 0){
	Ptree* handle = handlers->Car();
	Ptree* body = PtreeUtil::Nth(handle,4);
	Ptree* body2 = Translate(body);
	if(body == body2)
	    array.Append(handle);
	else{
	    array.Append(PtreeUtil::ShallowSubst(body2, body, handle));
	    changed = true;
	}

	handlers = handlers->Cdr();
    }

    if(try_block == try_block2 && !changed)
	return s;
    else
	return new PtreeTryStatement(s->Car(),
				     PtreeUtil::Cons(try_block2, array.All()));
}

Ptree* Walker::TranslateBreak(Ptree* s)
{
    return s;
}

Ptree* Walker::TranslateContinue(Ptree* s)
{
    return s;
}

Ptree* Walker::TranslateReturn(Ptree* s)
{
    if(PtreeUtil::Length(s) == 2)
	return s;
    else{
	Ptree* exp = PtreeUtil::Second(s);
	Ptree* exp2 = Translate(exp);
	if(exp == exp2)
	    return s;
	else
	    return new PtreeReturnStatement(s->Car(),
					    PtreeUtil::ShallowSubst(exp2, exp,
								s->Cdr()));
    }
}

Ptree* Walker::TranslateGoto(Ptree* s)
{
    return s;
}

Ptree* Walker::TranslateCase(Ptree* s)
{
    Ptree* st = PtreeUtil::Nth(s,3);
    Ptree* st2 = Translate(st);
    if(st == st2)
	return s;
    else
	return new PtreeCaseStatement(s->Car(),
				      PtreeUtil::ShallowSubst(st2, st, s->Cdr()));
}

Ptree* Walker::TranslateDefault(Ptree* s)
{
    Ptree* st = PtreeUtil::Third(s);
    Ptree* st2 = Translate(st);
    if(st == st2)
	return s;
    else
	return new PtreeDefaultStatement(s->Car(),
					 PtreeUtil::List(s->Cdr()->Car(), st2));
}

Ptree* Walker::TranslateLabel(Ptree* s)
{
    Ptree* st = PtreeUtil::Third(s);
    Ptree* st2 = Translate(st);
    if(st == st2)
	return s;
    else
	return new PtreeLabelStatement(s->Car(),
				       PtreeUtil::List(s->Cdr()->Car(), st2));
}

Ptree* Walker::TranslateExprStatement(Ptree* s)
{
    Ptree* exp = PtreeUtil::First(s);
    Ptree* exp2 = Translate(exp);
    if(exp == exp2)
	return s;
    else
	return new PtreeExprStatement(exp2, s->Cdr());
}

Ptree* Walker::TranslateTypespecifier(Ptree* tspec)
{
    Ptree *class_spec, *class_spec2;

    class_spec = PtreeTypeUtil::GetClassOrEnumSpec(tspec);
    if(class_spec == 0)
	class_spec2 = 0;
    else
	class_spec2 = Translate(class_spec);

    if(class_spec == class_spec2)
	return tspec;
    else
	return PtreeUtil::ShallowSubst(class_spec2, class_spec, tspec);
}

Ptree* Walker::GetClassOrEnumSpec(Ptree* typespec)
{
    Deprecated("Ptree* Walker::GetClassOrEnumSpec(Ptree* typespec)",
               "PtreeUtil::GetClassOrEnumSpec(typespec)");
    return PtreeTypeUtil::GetClassOrEnumSpec(typespec);
}

Ptree* Walker::GetClassTemplateSpec(Ptree* body)
{
    Deprecated("Walker::GetClassTemplateSpec(Ptree* body)",
               "PtreeUtil::GetClassTemplateSpec(body)");
    return PtreeTypeUtil::GetClassTemplateSpec(body);
}

Ptree* Walker::StripCvFromIntegralType(Ptree* integral)
{
    Deprecated("Walker::StripCvFromIntegralType(Ptree* integral)",
               "PtreeTypeUtil::StripCvFromIntegralType(integral)");
    return PtreeTypeUtil::StripCvFromIntegralType(integral);
}

Ptree* Walker::TranslateComma(Ptree* exp)
{
    Ptree* left = PtreeUtil::First(exp);
    Ptree* left2 = Translate(left);
    Ptree* right = PtreeUtil::Third(exp);
    Ptree* right2 = Translate(right);
    if(left == left2 && right == right2)
	return exp;
    else
	return new PtreeCommaExpr(left2, PtreeUtil::List(PtreeUtil::Second(exp), right2));
}

void Walker::TypeofComma(Ptree* exp, TypeInfo& t)
{
    Typeof(PtreeUtil::Third(exp), t);
}

Ptree* Walker::TranslateAssign(Ptree* exp)
{
    Ptree* left = PtreeUtil::First(exp);
    Ptree* left2 = Translate(left);
    Ptree* right = PtreeUtil::Third(exp);
    Ptree* right2 = Translate(right);
    if(left == left2 && right == right2)
	return exp;
    else
	return new PtreeAssignExpr(left2, PtreeUtil::List(PtreeUtil::Second(exp), right2));
}

void Walker::TypeofAssign(Ptree* exp, TypeInfo& t)
{
    Typeof(PtreeUtil::First(exp), t);
}

Ptree* Walker::TranslateCond(Ptree* exp)
{
    Ptree* c = PtreeUtil::First(exp);
    Ptree* c2 = Translate(c);
    Ptree* t = PtreeUtil::Third(exp);
    Ptree* t2 = Translate(t);
    Ptree* e = PtreeUtil::Nth(exp,4);
    Ptree* e2 = Translate(e);
    if(c == c2 && t == t2 && e == e2)
	return exp;
    else
	return new PtreeCondExpr(c2, PtreeUtil::List(PtreeUtil::Second(exp), t2,
						 PtreeUtil::Nth(exp,3), e2));
}

void Walker::TypeofCond(Ptree* exp, TypeInfo& t)
{
    Typeof(PtreeUtil::Third(exp), t);
}

Ptree* Walker::TranslateInfix(Ptree* exp)
{
    Ptree* left = PtreeUtil::First(exp);
    Ptree* left2 = Translate(left);
    Ptree* right = PtreeUtil::Third(exp);
    Ptree* right2 = Translate(right);
    if(left == left2 && right == right2)
	return exp;
    else
	return new PtreeInfixExpr(left2, PtreeUtil::List(PtreeUtil::Second(exp), right2));
}

void Walker::TypeofInfix(Ptree* exp, TypeInfo& t)
{
    Typeof(PtreeUtil::First(exp), t); // :TODO: implement correct type resolution for user-defined operators
}

Ptree* Walker::TranslatePm(Ptree* exp)
{
    Ptree* left = PtreeUtil::First(exp);
    Ptree* left2 = Translate(left);
    Ptree* right = PtreeUtil::Third(exp);
    Ptree* right2 = Translate(right);
    if(left == left2 && right == right2)
	return exp;
    else
	return new PtreePmExpr(left2, PtreeUtil::List(PtreeUtil::Second(exp), right2));
}

void Walker::TypeofPm(Ptree* exp, TypeInfo& t)
{
    Typeof(PtreeUtil::Third(exp), t);
    t.Dereference();
}

Ptree* Walker::TranslateCast(Ptree* exp)
{
    Ptree* e = PtreeUtil::Nth(exp,3);
    Ptree* e2 = Translate(e);
    if(e == e2)
	return exp;
    else
	return new PtreeCastExpr(PtreeUtil::First(exp),
				 PtreeUtil::ShallowSubst(e2, e, exp->Cdr()));
}

void Walker::TypeofCast(Ptree* exp, TypeInfo& t)
{
    using PtreeUtil::Second;
    t.Set(Second(Second(exp))->GetEncodedType(), env);
}

Ptree* Walker::TranslateUnary(Ptree* exp)
{
    Ptree* oprnd = PtreeUtil::Second(exp);
    Ptree* oprnd2 = Translate(oprnd);
    if(oprnd == oprnd2)
	return exp;
    else
	return new PtreeUnaryExpr(PtreeUtil::First(exp), PtreeUtil::List(oprnd2));
}

void Walker::TypeofUnary(Ptree* exp, TypeInfo& t)
{
    Typeof(PtreeUtil::Second(exp), t);

    Ptree* op = PtreeUtil::First(exp);
    if(PtreeUtil::Eq(op,'*'))
	t.Dereference();
    else if(PtreeUtil::Eq(op,'&'))
	t.Reference();
}

Ptree* Walker::TranslateThrow(Ptree* exp)
{
    Ptree* oprnd = PtreeUtil::Second(exp);
    Ptree* oprnd2 = Translate(oprnd);
    if(oprnd == oprnd2)
	return exp;
    else
	return new PtreeThrowExpr(PtreeUtil::First(exp), PtreeUtil::List(oprnd2));
}

void Walker::TypeofThrow(Ptree*, TypeInfo& t)
{
    t.SetVoid();
}

Ptree* Walker::TranslateSizeof(Ptree* exp)
{
    Ptree* e = PtreeUtil::Second(exp);
    if(PtreeUtil::Eq(e,'('))
	e = PtreeUtil::Third(exp);

    Ptree* e2 = Translate(e);
    if(e == e2)
	return exp;
    else
	return new PtreeSizeofExpr(PtreeUtil::First(exp),
				   PtreeUtil::ShallowSubst(e2, e, exp->Cdr()));
}

void Walker::TypeofSizeof(Ptree*, TypeInfo& t)
{
    t.SetInt();
}

Ptree* Walker::TranslateTypeid(Ptree* exp)
{
    Ptree* e = PtreeUtil::Second(exp);
    if(PtreeUtil::Eq(e,'('))
	e = PtreeUtil::Third(exp);

    Ptree* e2 = Translate(e);
    if(e == e2)
	return exp;
    else
	return new PtreeTypeidExpr(PtreeUtil::First(exp),
				   PtreeUtil::ShallowSubst(e2, e, exp->Cdr()));
}

void Walker::TypeofTypeid(Ptree *exp, TypeInfo& t)
{
    t.SetTypeInfo(env -> GetBottom());
        // needs to pass the bottom environment so that SetTypeInfo()
	// can construct the representation of '::std::type_info' type.
}

Ptree* Walker::TranslateTypeof(Ptree* exp)
{
    Ptree* e = PtreeUtil::Second(exp);
    if(PtreeUtil::Eq(e,'('))
	e = PtreeUtil::Third(exp);

    Ptree* e2 = Translate(e);
    if(e == e2)
	return exp;
    else
	return new PtreeTypeofExpr(PtreeUtil::First(exp),
				   PtreeUtil::ShallowSubst(e2, e, exp->Cdr()));
}

void Walker::TypeofTypeof(Ptree *exp, TypeInfo& t)
{
    t.SetInt(); // FIXME: Should be the type_info type (exp->Third()->Second()->GetEncodedType(), env);
}

Ptree* Walker::TranslateNew(Ptree* exp)
{
    Ptree *p;
    Ptree *userkey, *scope, *op, *placement, *type, *init;

    p = exp;
    userkey = p->Car();
    if(userkey == 0 || !userkey->IsLeaf())
	p = exp->Cdr();		// user keyword
    else
	userkey = 0;

    if(PtreeUtil::Eq(p->Car(), "::")){
	scope = p->Car();
	p = p->Cdr();
    }
    else
	scope = 0;

    op = p->Car();
    placement = p->Cdr()->Car();
    type = PtreeUtil::Third(p);
    init = PtreeUtil::Nth(p,3);
    return TranslateNew2(exp, userkey, scope, op, placement, type, init);
}

Ptree* Walker::TranslateNew2(Ptree* exp, Ptree*, Ptree*,
			     Ptree*, Ptree* placement,
			     Ptree* type, Ptree* init)
{
    Ptree* placement2 = TranslateArguments(placement);
    Ptree* type2 = TranslateNew3(type);
    Ptree* init2 = TranslateArguments(init);
    if(placement == placement2 && init == init2)
	return exp;
    else
	return new PtreeNewExpr(exp->Car(),
				PtreeUtil::ShallowSubst(placement2, placement,
						    type2, type,
						    init2, init,
						    exp->Cdr()));
}

Ptree* Walker::TranslateNew3(Ptree* type)
{
    Ptree* p = type;
    if(PtreeUtil::Eq(p->Car(), '('))
	p = PtreeUtil::Second(p);

    Ptree* decl = PtreeUtil::Second(p);
    Ptree* decl2 = TranslateNewDeclarator(decl);
    if(decl == decl2)
	return type;
    else
	return PtreeUtil::Subst(decl2, decl, type);
}

void Walker::TypeofNew(Ptree* exp, TypeInfo& t)
{
    using PtreeUtil::Second;
    Ptree *p, *userkey, *type;

    p = exp;
    userkey = p->Car();
    if(userkey == 0 || !userkey->IsLeaf())
	p = exp->Cdr();		// user keyword

    if(PtreeUtil::Eq(p->Car(), "::"))
	p = p->Cdr();

    type = PtreeUtil::Third(p);

    if(PtreeUtil::Eq(type->Car(), '('))
	t.Set(Second(Second(type))->GetEncodedType(), env);
    else
	t.Set(Second(type)->GetEncodedType(), env);

    t.Reference();
}

Ptree* Walker::TranslateDelete(Ptree* exp)
{
    Ptree* obj = PtreeUtil::Last(exp)->Car();
    Ptree* obj2 = Translate(obj);
    if(obj == obj2)
	return exp;
    else
	return new PtreeDeleteExpr(exp->Car(),
				   PtreeUtil::ShallowSubst(obj2, obj,
						       exp->Cdr()));
}

void Walker::TypeofDelete(Ptree*, TypeInfo& t)
{
    t.SetVoid();
}

Ptree* Walker::TranslateThis(Ptree* exp)
{
    return exp;
}

void Walker::TypeofThis(Ptree*, TypeInfo& t)
{
    t.Set(env->LookupThis());
}

Ptree* Walker::TranslateVariable(Ptree* exp)
{
    return exp;
}

/*
  This may be a class name if the expression is a function-style cast.
*/
void Walker::TypeofVariable(Ptree* exp, TypeInfo& t)
{
    bool is_type_name;

    if(env->Lookup(exp, is_type_name, t))
	if(is_type_name)	// if exp is a class name
	    t.Reference();	// see TypeofFuncall
}

/*
  TranslateFstyleCast() deals with function-style casts
  to an integral type
*/
Ptree* Walker::TranslateFstyleCast(Ptree* exp)
{
    Ptree* args = exp->Cdr();
    Ptree* args2 = TranslateArguments(args);
    if(args == args2)
	return exp;
    else
	return new PtreeFstyleCastExpr(exp->GetEncodedType(), exp->Car(),
				       args2);
}

void Walker::TypeofFstyleCast(Ptree* exp, TypeInfo& t)
{
    t.Set(exp->GetEncodedType(), env);
}

Ptree* Walker::TranslateArray(Ptree* exp)
{
    Ptree* array = exp->Car();
    Ptree* array2 = Translate(array);
    Ptree* index = PtreeUtil::Third(exp);
    Ptree* index2 = Translate(index);
    if(array == array2 && index == index2)
	return exp;
    else
	return new PtreeArrayExpr(array2, PtreeUtil::Subst(index2, index,
						       exp->Cdr()));
}

void Walker::TypeofArray(Ptree* exp, TypeInfo& t)
{
    Typeof(exp->Car(), t);
    t.Dereference();
}

/*
  TranslateFuncall() also deals with function-style casts to a class.
*/
Ptree* Walker::TranslateFuncall(Ptree* exp)
{
    Ptree* fun = exp->Car();
    Ptree* fun2 = Translate(fun);
    Ptree* args = exp->Cdr();
    Ptree* args2 = TranslateArguments(args);
    if(fun == fun2 && args == args2)
	return exp;
    else
	return new PtreeFuncallExpr(fun2, args2);
}

void Walker::TypeofFuncall(Ptree* exp, TypeInfo& t)
{
    Typeof(exp->Car(), t);
    if(!t.IsFunction())
	t.Dereference();	// maybe a pointer to a function

    t.Dereference();
}

Ptree* Walker::TranslatePostfix(Ptree* exp)
{
    Ptree* left = exp->Car();
    Ptree* left2 = Translate(left);
    if(left == left2)
	return exp;
    else
	return new PtreePostfixExpr(left2, exp->Cdr());
}

void Walker::TypeofPostfix(Ptree* exp, TypeInfo& t)
{
    Typeof(exp->Car(), t);
}

Ptree* Walker::TranslateUserStatement(Ptree* exp)
{
    return exp;
}

void Walker::TypeofUserStatement(Ptree*, TypeInfo& t)
{
    t.Unknown();
}

Ptree* Walker::TranslateDotMember(Ptree* exp)
{
    Ptree* left = exp->Car();
    Ptree* left2 = Translate(left);
    if(left == left2)
	return exp;
    else
	return new PtreeDotMemberExpr(left2, exp->Cdr());
}

void Walker::TypeofDotMember(Ptree* exp, TypeInfo& t)
{
    Typeof(exp->Car(), t);
    t.SetMember(PtreeUtil::Third(exp));
}

Ptree* Walker::TranslateArrowMember(Ptree* exp)
{
    Ptree* left = exp->Car();
    Ptree* left2 = Translate(left);
    if(left == left2)
	return exp;
    else
	return new PtreeArrowMemberExpr(left2, exp->Cdr());
}

void Walker::TypeofArrowMember(Ptree* exp, TypeInfo& t)
{
    Typeof(exp->Car(), t);
    t.Dereference();
    t.SetMember(PtreeUtil::Third(exp));
}

Ptree* Walker::TranslateParen(Ptree* exp)
{
    Ptree* e = PtreeUtil::Second(exp);
    Ptree* e2 = Translate(e);
    if(e == e2)
	return exp;
    else
	return new PtreeParenExpr(exp->Car(), PtreeUtil::List(e2, PtreeUtil::Third(exp)));
}

void Walker::TypeofParen(Ptree* exp, TypeInfo& t)
{
    Typeof(PtreeUtil::Second(exp), t);
}

Ptree* Walker::TranslateStaticUserStatement(Ptree* exp)
{
    return exp;
}

void Walker::TypeofStaticUserStatement(Ptree*, TypeInfo& t)
{
    t.Unknown();
}

Ptree* Walker::TranslateNewDeclarator(Ptree* decl)
{
    Ptree* decl2 = decl;
    Ptree* p = decl;
    while(p != 0){
	Ptree* head = p->Car();
	if(head == 0)
	    return decl;
	else if(PtreeUtil::Eq(head,'[')){
	    Ptree* p2 = TranslateNewDeclarator2(p);
	    if(p == p2)
		return decl;
	    else{
		decl2 = PtreeUtil::ShallowSubst(p2, p, decl);
		break;
	    }
	}
	else if(!head->IsLeaf() && PtreeUtil::Eq(head->Car(), '(')){
	    Ptree* d = head->Cdr()->Car();
	    Ptree* d2 = TranslateNewDeclarator(d);
	    decl2 = PtreeUtil::ShallowSubst(d2, d, decl);
	    break;
	}

	p = p->Cdr();
    }

    if(p == 0)
	return decl;
    else if(decl->IsA(ntDeclarator))
	return new PtreeDeclarator((PtreeDeclarator*)decl,
				   decl2->Car(), decl2->Cdr());
    else
	return decl2;
}

Ptree* Walker::TranslateNewDeclarator2(Ptree* decl)
{
    for(Ptree* p = decl; p != 0; p = p->Cdr()){
	Ptree* head = p->Car();
	if(PtreeUtil::Eq(head,'[')){
	    Ptree* size = p->Cdr()->Car();
	    Ptree* size2 = Translate(size);
	    if(size != size2){
		Ptree* q = TranslateNewDeclarator2(PtreeUtil::ListTail(p, 3));
		return PtreeUtil::Nconc(PtreeUtil::List(p->Car(), size2, PtreeUtil::Third(p)),
				    q);
	    }
	}
	else if(PtreeUtil::Eq(head,'('))
	    break;
    }

    return decl;
}

Ptree* Walker::TranslateArguments(Ptree* arglist)
{
    if(arglist == 0)
	return arglist;

    PtreeArray array;
    bool changed = false;
    Ptree* body = PtreeUtil::Second(arglist);
    Ptree* args = body;
    while(args != 0){
	Ptree* p = args->Car();
	Ptree* q = Translate(p);
	array.Append(q);
	if(p != q)
	    changed = true;

	args = args->Cdr();
	if(args != 0){
	    array.Append(args->Car());
	    args = args->Cdr();
	}
    }

    if(changed)
	return PtreeUtil::ShallowSubst(array.All(), body, arglist);
    else
	return arglist;
}


Ptree* Walker::FindDeclarator(Ptree* def, char* name, int len,
			      char* signature, int& nth, Environment* e)
{
    Ptree* decls = PtreeUtil::Third(def);
    if(decls == 0 || decls->IsLeaf())
	return 0;

    if(decls->IsA(ntDeclarator)){	// if it is a function
	if(MatchedDeclarator(decls, name, len, signature, e))
	    return decls;

	++nth;
    }
    else
	while(decls != 0){
	    Ptree* d = decls->Car();
	    if(MatchedDeclarator(d, name, len, signature, e))
		return d;

	    ++nth;
	    if((decls = decls->Cdr()) != 0)
		decls = decls->Cdr();		// skip ,
	}

    return 0;
}

bool Walker::MatchedDeclarator(Ptree* decl, char* name, int len,
			       char* signature, Environment* e)
{
    char* str;
    int strlen;
    char* sig;

    str = decl->GetEncodedName();
    sig = decl->GetEncodedType();
    if(str == 0 || sig == 0)
	return false;

    str = EncodingUtil::GetBaseName(str, strlen, e);
    return bool(len == strlen && memcmp(name, str, len) == 0
		&& strcmp(signature, sig) == 0);
}

bool Walker::WhichDeclarator(Ptree* def, Ptree* name, int& nth,
			     Environment* env)
{
    char* str;
    int len;
    Environment* e;
    Ptree* decls = PtreeUtil::Third(def);
    if(decls == 0 || decls->IsLeaf())
	return false;

    if(decls->IsA(ntDeclarator)){	// if it is a function
	str = decls->GetEncodedName();
	e = env;
	str = EncodingUtil::GetBaseName(str, len, e);
	if(PtreeUtil::Eq(name, str, len))
	    return true;

	++nth;
    }
    else
	while(decls != 0){
	    str = decls->Car()->GetEncodedName();
	    e = env;
	    str = EncodingUtil::GetBaseName(str, len, e);
	    if(PtreeUtil::Eq(name, str, len))
		return true;

	    ++nth;
	    if((decls = decls->Cdr()) != 0)
		decls = decls->Cdr();
	}

    return false;
}

namespace {

    void Message(Msg::Severity severity, Parser* parser, 
                 char* msg, Ptree* name, Ptree* where)
    {
	assert(parser);
	assert(where);
	SourceLocation location(parser->GetLocation(where));
	strstream buf;
	buf << msg << name << ends;
	TheErrorLog().Report(GenericMsg(severity, location, buf.str()));
	buf.freeze(false);
    }

}

void Walker::ErrorMessage(char* msg, Ptree* name, Ptree* where)
{
    Message(Msg::Error, parser, msg, name, where);
}

void Walker::WarningMessage(char* msg, Ptree* name, Ptree* where)
{
    Message(Msg::Warning, parser, msg, name, where);
}

// InaccurateErrorMessage() may report a wrong line number.

void Walker::InaccurateErrorMessage(char* msg, Ptree* name, Ptree* where)
{
    if(default_parser == 0)
    {
        TheErrorLog().Report(
            MopMsg(Msg::Fatal, 
                "Walker::InaccurateErrorMessage()"
              ,  "no default parser"
            )
        );
    }
    else
    {
        Message(Msg::Error, default_parser, msg, name, where);
    }
}

void Walker::InaccurateWarningMessage(char* msg, Ptree* name, Ptree* where)
{
    if(default_parser == 0)
    {
        TheErrorLog().Report(
            MopMsg(Msg::Fatal, 
                "Walker::InaccurateWarningMessage()"
              ,  "no default parser"
            )
        );
    }
    else
    {
        Message(Msg::Warning, default_parser, msg, name, where);
    }
}

END_OPENCXX_NAMESPACE
