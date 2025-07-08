// ChunkCopier is an abstract class, which opens two IFF streams, source and destination. Its
// main task is to copy chunks from source to destination, one by one. Subclasses can perform
// additional operations at start or at end of given chunk.

// Application
//   +--> ChunkCopier
//          +--> IFFFile : IFFReader
//          +--> IFFFile : IFFWriter

#include "main.h"

#include <libraries/iffparse.h>

class ChunkCopier
{
	protected:

	bool copyThisChunk;      // subclasses can use this flag to skip a chunk when copying

	public:

	ChunkCopier(const char *srcname, const char *dstname, uint32 chunkid);
	~ChunkCopier();
	bool Parse();
	virtual bool PreChunkWork(ContextNode *cn) = 0;
	virtual bool PostChunkWork(ContextNode *cn) = 0;
};
