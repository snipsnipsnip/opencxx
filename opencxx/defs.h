#ifndef guard_opencxx_defs_h
#define guard_opencxx_defs_h

//@beginlicenses@
//@license{chiba-tokyo}{}@
//@license{xerox}{}@
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

#define	OPENCXX_NAMESPACE		Opencxx
#define OPENCXX_NAMESPACE_QUAL(id)	::Opencxx::id
#define USING_OPENCXX_NAMESPACE 	using namespace Opencxx ;
#define	BEGIN_OPENCXX_NAMESPACE		namespace Opencxx {
#define END_OPENCXX_NAMESPACE 		}

#if defined(sun) && (defined(sparc) || defined(__sparc))
#  include <errno.h>
#  ifdef ECHRNG
#    define SUNOS5
#  else
#    define SUNOS4
#  endif
#endif

#define USE_DLOADER		1

#if !defined (_MSC_VER)
#define USE_SO			1
#else
#define USE_SO			0	// Use DLL
#endif

#if USE_DLOADER && defined(_MSC_VER)
#define OCXXMOP __declspec(dllexport)
#pragma warning(disable:4275)
#else
#define OCXXMOP
#endif

#if (defined(__FreeBSD__) && __FreeBSD__ <= 2) || defined (__OpenBSD__) || defined (__NetBSD__)
#define DLSYM_NEED_UNDERSCORE	1
#define SHARED_OPTION		0
#else	// Solaris, SunOS, Linux, Irix, FreeBSD 3.x, MacOS X
#define DLSYM_NEED_UNDERSCORE	0
#define SHARED_OPTION		1
#endif

#if USE_DLOADER && USE_SO

// Some operating systems like FreeBSD and SunOS do not define RTLD_GLOBAL
// or RTLD_LAZY

#ifndef RTLD_LAZY
#define RTLD_LAZY		1
#endif

#ifndef RTLD_GLOBAL
#define RTLD_GLOBAL		0
#endif

#endif /* USE_DLOADER */

#endif /* ! guard_opencxx_defs_h */
