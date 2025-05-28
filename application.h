//==============
// IFFChunkTool
//==============

#include "iffreader.h"
#include "iffwriter.h"
#include "callargs.h"

class Application
{
	CallArgs &arguments;
	IFFReader reader;
	IFFWriter writer;
	uint32 chunk;
	bool ListChunks();
	bool AppendChunk();
	bool InsertChunk();
	bool ReplaceChunk();
	bool RemoveChunk();
	bool ExtractChunk();

	public:

	Application(CallArgs &args) : arguments(args) {}
	bool Process();
	~Application() {}
};
