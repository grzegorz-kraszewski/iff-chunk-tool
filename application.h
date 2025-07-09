//==============
// IFFChunkTool
//==============

#include "callargs.h"

class Application
{
	CallArgs &arguments;

	uint32 ValidateChunkID(const char *str);

	public:

	Application(CallArgs &args) : arguments(args) {}
	bool Process();
	~Application() {}
};
