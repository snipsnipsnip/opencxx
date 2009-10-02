//(Automatic CVS control) $Revision: 1.5 $ $Date: 2004/06/09 14:56:15 $

#include <iostream>

using namespace std;

metaclass NestedClass;

class Outer_1
{
public:
    Outer_1()
    {
        cerr << "Outer_1::Outer_1()" << endl;
    }

    nested class Inner
    {
    public:
        Inner() {
            cerr << "-- Outer_1::Inner()" << endl;
        }
    };
};

// The bug does only show up if Outer_2 inherits Outer_1!
class Outer_2 : public Outer_1
{
public:
    nested class Inner
    {
    public:
        Inner( const char *foo )
        {
            cerr << "-- Outer_2::Inner(): " << foo << endl;
        }
    };

};

int main()
{
    return 0;
}
