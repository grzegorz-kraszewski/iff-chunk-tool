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
	bool ExtractChunk(uint32 id);
	uint32 ValidateChunkID(const char *str);
	bool WriteChunkContents(ContextNode *cn);

	public:

	Application(CallArgs &args) : arguments(args) {}
	bool Process();
	~Application() {}
};
