//(Automatic CVS control) $Revision: 1.4 $ $Date: 2004/06/09 14:56:15 $

/*
------------------------------------------------------------------------
Copyright (C) 2001 Other Contributors.

Permission to use, copy, distribute and modify this software and its
documentation for any purpose is hereby granted without fee, provided that
the above copyright notice appear in all copies and that both that copyright
notice and this permission notice appear in supporting documentation.

Other Contributors make no representations about the suitability of this
software for any purpose. It is provided "as is" without express or implied
warranty.

Other Contributors are: Grzegorz Jakacki.
------------------------------------------------------------------------
*/
#include <typeinfo>

template <typename T>
class SomeTemplate {};


int
main()
{
    const char *c1 = typeid(int).name();
    const char *c2 = typeid(SomeTemplate< SomeTemplate <int> >).name();
}
