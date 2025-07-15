//==============
// IFFChunkTool
//==============

#include "callargs.h"
#include "chunkcopier.h"

class Application
{
	CallArgs &arguments;

	public:

	Application(CallArgs &args) : arguments(args) {}
	bool Process();
	ChunkDataSource* PrepareDataSource();
	~Application() {}
};
