#ifndef VERSION_H
#define VERSION_H

namespace novaengine{

	//Date Version Types
	static const char NOVAENGINE_DATE[] = "16";
	static const char NOVAENGINE_MONTH[] = "02";
	static const char NOVAENGINE_YEAR[] = "2016";
	static const char NOVAENGINE_UBUNTU_VERSION_STYLE[] =  "16.02";

	//Software Status
	static const char NOVAENGINE_STATUS[] =  "BlackSquare";
	static const char NOVAENGINE_STATUS_SHORT[] =  "bs";

	//Standard Version Type
	static const long NOVAENGINE_MAJOR  = 0;
	static const long NOVAENGINE_MINOR  = 1;
	static const long NOVAENGINE_BUILD  = 422;
	static const long NOVAENGINE_REVISION  = 20;

	//Miscellaneous Version Types
	static const long NOVAENGINE_BUILDS_COUNT  = 1137;
	#define NOVAENGINE_RC_FILEVERSION 0,0,422,20
	#define NOVAENGINE_RC_FILEVERSION_STRING "0, 0, 422, 20\0"
	static const char NOVAENGINE_FULLVERSION_STRING [] = "0.1.422.20";

	//These values are to keep track of your versioning state, don't modify them.
	static const long NOVAENGINE_BUILD_HISTORY  = 17;


}
#endif //VERSION_H
