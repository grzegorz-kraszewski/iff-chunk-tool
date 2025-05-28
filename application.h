//==============
// IFFChunkTool
//==============

#include "iffreader.h"
#include "iffwriter.h"
#include "callargs.h"

class Application
{
	IFFReader reader;
	IFFWriter writer;
	CallArgs &arguments;

	public:

	bool ready;
	Application(CallArgs &args);
	bool Process() {}
	~Application() {}
};
