//==============
// IFFChunkTool
//==============

#include "callargs.h"

class Application
{
	CallArgs &arguments;

	public:

	Application(CallArgs &args) : arguments(args) {}
	bool Process();
	~Application() {}
};
