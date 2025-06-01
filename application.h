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
	char idBuf[6];
	bool ListChunks();
	bool AppendChunk(uint32 id);
	bool InsertChunk(uint32 id);
	bool ReplaceChunk(uint32 id);
	bool RemoveChunk(uint32 id);
	bool ExtractChunk(uint32 id);
	bool PushChunkFromSource(uint32 id);
	uint32 ValidateChunkID(const char *str);
	bool WriteChunkContents(ContextNode *cn);
	bool CopyChunk(IFFReader &src, IFFWriter &dest, ContextNode *cn);
	bool PushChunkFromFile(const char *path, uint32 id);
	bool PushChunkFromString(const char *string, uint32 id);

	public:

	Application(CallArgs &args) : arguments(args) {}
	bool Process();
	~Application() {}
};
