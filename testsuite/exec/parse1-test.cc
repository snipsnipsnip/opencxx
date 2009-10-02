//(Automatic CVS control) $Revision: 1.7.2.1 $ $Date: 2004/07/31 00:53:04 $

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

Other Contributors are: Marcus Bartholomeu.
------------------------------------------------------------------------
*/

#include <iostream>

using namespace std;

// create decimal constants from digits at choosen base x
template <unsigned base, unsigned digits>
inline unsigned convert_base() {
    return digits ? (base * convert_base<base,(digits/10)>() + digits%10) : 0 ;
}

int main() {
  const int b3=121;
  const int dec = convert_base<3,b3>();
  cout << "base3(" << b3 << ") = " << dec << "." << endl;
}
