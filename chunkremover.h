//=============================================================================
// ChunkRemover copies all the IFF file chunks except of specified one. Chunk
// is skipped by clearing 'copyThisChunk' flag.
//=============================================================================

#include "chunkcopier.h"


class ChunkRemover : public ChunkCopier
{
	uint32 chunkId;

	public:

	ChunkRemover(const char *sourceName, const char *destName, const char *chunk);
	virtual bool PreChunkWork(ContextNode *cn) { if (cn->cn_ID == chunkId) copyThisChunk = FALSE; }
};
