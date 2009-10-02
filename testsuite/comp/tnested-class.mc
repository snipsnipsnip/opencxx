//(Automatic CVS control) $Revision: 1.6.2.1 $ $Date: 2004/07/29 20:04:51 $

#include <opencxx/mop.h>
#include <iostream>

using namespace std;
using namespace Opencxx;

class NestedClass : public Class
{
public:
    static bool Initialize()
    {
        RegisterMetaclass( "nested", "NestedClass" );
        // Documentation versus example inconsistency:
        // The subclasses of Class may define their own Initialize() but
        // they must not call their base classes' Initialize().
        return Class::Initialize(); // only returns true
    }

    // This method is run twice, once for Outer_1::Inner and once for
    // Outer_2::Inner. In both cases, it should look up Outer_1::Inner, but
    // if Outer_2 inherits Outer_1, then it will look up Outer_2::Inner 
    // instead of Outer_1::Inner.
    
    virtual void TranslateClass( Environment * env )
    {
        Class::TranslateClass( env );

        // GetEnvironment() should resemble our class' scope, its parent-
        // environment should be the global scope. So in the global
        // environment, we should be able to find the classes Outer_1 and
        // Outer_2.
        Environment *outerEnv = GetEnvironment()->GetOuterEnvironment();

        Ptree *outer1_name = Ptree::Make("Outer_1");
        Class *outer1_class = outerEnv->LookupClassMetaobject( outer1_name );

        if ( !outer1_class )
            cout << "### ERROR: Outer_1 not found" << endl;
        
        Ptree *inner_name = Ptree::Make("Inner");
        Class *inner_1 = outer1_class->GetEnvironment()->LookupClassMetaobject( inner_name );

        if ( !inner_name )
            cout << "### ERROR: Inner not found" << endl;
        
        inner_1->Definition()->Write( cout );
        cout << endl;
    }

};
