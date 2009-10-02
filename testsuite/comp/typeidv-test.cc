//(Automatic CVS control) $Revision: 1.3 $ $Date: 2004/06/09 14:56:15 $

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
class SomeTemplate 
{
public:
    T val;
};

int
main()
{
    SomeTemplate<SomeTemplate<int> > some_value;
    
    const char *c1 = typeid(5).name();
    const char *c2 = typeid(some_value).name();
    const char *c3 = typeid(some_value.val.val + 5).name();
    const char *c4 = typeid(*c3).name();
}
