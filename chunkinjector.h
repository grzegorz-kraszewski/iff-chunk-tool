//=============================================================================
// ChunkInjector copies all the IFF file chunks and injects additional chunk
// after a specified one, or as the first chunk in the FORM if 'after' chunk
// is NULL.
//=============================================================================

#include "chunkcopier.h"


class ChunkInjector : public ChunkCopier
{
	uint32 chunkId;
	uint32 chunkAfter;
	bool found;
	ChunkDataSource *data;

	public:

	ChunkInjector(const char *sourceName, const char *destName, const char
	 *chunk, ChunkDataSource *data, const char *after);
	virtual bool FormStartWork();
	virtual bool FormEndWork();
	virtual bool PostChunkWork(ContextNode *cn);
};
