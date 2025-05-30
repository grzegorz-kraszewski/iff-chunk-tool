//==============
// IFFChunkTool
//==============

#include "iffreader.h"
#include "iffwriter.h"
#include "callargs.h"

#define COPY_BUFFER_SIZE 16384


class Application
{
	CallArgs &arguments;
	IFFReader reader;
	IFFWriter writer;
	uint32 chunk;
	void *copyBuffer;
	bool ListChunks();
	bool AppendChunk();
	bool InsertChunk();
	bool ReplaceChunk();
	bool RemoveChunk();
	bool ExtractChunk(uint32 id);
	uint32 ValidateChunkID(const char *str);
	bool WriteChunkContents(ContextNode *cn);
	bool CopyChunk(IFFReader &src, IFFWriter &dest, ContextNode *cn);

	public:

	Application(CallArgs &args) : arguments(args) {}
	bool Process();
	~Application() {}
};
