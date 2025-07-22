//=============================================================================
// ChunkRemover copies all the IFF file chunks except of specified one. Chunk
// is skipped by clearing 'copyThisChunk' flag.
//=============================================================================

#include "chunkcopier.h"
#include "extchunkid.h"


class ChunkRemover : public ChunkCopier
{
	ExtendedChunkID id;
	int32 chunkCounter;

	public:

	ChunkRemover(const char *sourceName, const char *destName, const char *chunk);
	virtual bool PreChunkWork(ContextNode *cn);
	virtual bool FormEndWork();
};
