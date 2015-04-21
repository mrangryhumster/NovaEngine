#ifndef VERSION_H
#define VERSION_H

namespace novaengine{
	
	//Date Version Types
	static const char NOVAENGINE_DATE[] = "09";
	static const char NOVAENGINE_MONTH[] = "03";
	static const char NOVAENGINE_YEAR[] = "2015";
	static const char NOVAENGINE_UBUNTU_VERSION_STYLE[] =  "15.03";
	
	//Software Status
	static const char NOVAENGINE_STATUS[] =  "FlyingAnvil";
	static const char NOVAENGINE_STATUS_SHORT[] =  "fa";
	
	//Standard Version Type
	static const long NOVAENGINE_MAJOR  = 0;
	static const long NOVAENGINE_MINOR  = 0;
	static const long NOVAENGINE_BUILD  = 422;
	static const long NOVAENGINE_REVISION  = 13;
	
	//Miscellaneous Version Types
	static const long NOVAENGINE_BUILDS_COUNT  = 990;
	#define NOVAENGINE_RC_FILEVERSION 0,0,422,13
	#define NOVAENGINE_RC_FILEVERSION_STRING "0, 0, 422, 13\0"
	static const char NOVAENGINE_FULLVERSION_STRING [] = "0.0.422.13";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long NOVAENGINE_BUILD_HISTORY  = 17;
	

}
#endif //VERSION_H
