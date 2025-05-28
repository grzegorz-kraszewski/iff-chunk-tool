//==============
// IFFChunkTool
//==============

#include "iffreader.h"
#include "iffwriter.h"
#include "callargs.h"

class Application
{
	CallArgs &arguments;

	public:

	bool ready;
	Application(CallArgs &args) : arguments(args), ready(TRUE);
	void Process();
	~Application() {}
};
