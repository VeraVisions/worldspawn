
#include "pooledstring.h"
#include "globaldefs.h"
#include "generic/static.h"

#if GDEF_DEBUG

namespace ExamplePooledString
{
void testStuff(){
	PooledString< LazyStatic<StringPool> > a, b;
	a = "monkey";
	b = "monkey";
	a = "";
}

struct Always
{
	Always(){
		testStuff();
	}
} always;
}

#endif
