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

#include <opencxx/EncodingUtil.h>
#include <opencxx/Class.h>
#include <opencxx/Bind.h>
#include <opencxx/Environment.h>
#include <opencxx/TypeInfo.h>

namespace Opencxx
{

namespace EncodingUtil {

unsigned char* 
GetTemplateArguments(unsigned char* name, int& len)
{
    int m = name[0] - 0x80;
    if(m <= 0){
	len = name[1] - 0x80;
	return &name[2];
    }
    else{
	len = name[m + 1] - 0x80;
	return &name[m + 2];
    }
}

int 
GetBaseNameIfTemplate(unsigned char* name, Environment*& e)
{
    int m = name[0] - 0x80;
    if(m <= 0)
	return name[1] - 0x80 + 2;

    Bind* b;
    if(e != 0 && e->LookupType((char*)&name[1], m, b))
	if(b != 0 && b->What() == Bind::isTemplateClass){
	    Class* c = b->ClassMetaobject();
	    if(c != 0){
		e = c->GetEnvironment();
		return m + (name[m + 1] - 0x80) + 2;
	    }
	}

    // the template name was not found.
    e = 0;
    return m + (name[m + 1] - 0x80) + 2;
}


// GetBaseName() returns "Foo" if ENCODE is "Q[2][1]X[3]Foo", for example.
// If an error occurs, the function returns 0.

char* 
GetBaseName(char* encode, int& len, Environment*& env)
{
    if(encode == 0){
	len = 0;
	return 0;
    }

    Environment* e = env;
    unsigned char* p = (unsigned char*)encode;
    if(*p == 'Q'){
	int n = p[1] - 0x80;
	p += 2;
	while(n-- > 1){
	    int m = *p++;
	    if(m == 'T')
		m = GetBaseNameIfTemplate(p, e);
	    else if(m < 0x80){		// error?
		len = 0;
		return 0;
	    }
	    else{			// class name
		m -= 0x80;
		if(m <= 0){		// if global scope (e.g. ::Foo)
		    if(e != 0)
			e = e->GetBottom();
		}
		else
		    e = ResolveTypedefName(e, (char*)p, m);
	    }

	    p += m;
	}

	env = e;
    }

    if(*p == 'T'){		// template class
	int m = p[1] - 0x80;
	int n = p[m + 2] - 0x80;
	len = m + n + 3;
	return (char*)p;
    }
    else if(*p < 0x80){		// error?
	len = 0;
	return 0;
    }
    else{
	len = *p - 0x80;
	return (char*)p + 1;
    }
}

Environment* 
ResolveTypedefName(Environment* env, char* name, int len)
{
    TypeInfo tinfo;
    Bind* bind;
    Class* c = 0;

    if(env != 0)
	if (env->LookupType(name, len, bind) && bind != 0)
	    switch(bind->What()){
	    case Bind::isClassName :
		c = bind->ClassMetaobject();
		break;
	    case Bind::isTypedefName :
		bind->GetType(tinfo, env);
		c = tinfo.ClassMetaobject();
		/* if (c == 0) */
		    env = 0;
		break;
	    default :
		break;
	    }
	else {
	    env = env->LookupNamespace(name, len);
	    /* env is 0 if name is an unknown typedef name or namespace.
	     */
	}

    if(c != 0)
	return c->GetEnvironment();
    else
	return env;
}

}

}
