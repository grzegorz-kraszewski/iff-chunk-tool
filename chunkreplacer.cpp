#include "chunkreplacer.h"
#include "locale.h"

//=============================================================================
// ChunkReplacer::ChunkReplacer()
//=============================================================================

ChunkReplacer::ChunkReplacer(const char *sourceName, const char *destName,
 const char *chunk, ChunkDataSource *data) : ChunkCopier(sourceName, destName),
 data(data)
 {
	if (ready)
	{
		ready = FALSE;

		if (data && data->ready)
		{
			if (chunkId = ValidateChunkID(chunk)) ready = TRUE;
		}
	}
}

//=============================================================================
// ChunkReplacer::PreChunkWork()
//=============================================================================

bool ChunkReplacer::PreChunkWork(ContextNode *cn)
{
	if (cn->cn_ID == chunkId)
	{
		copyThisChunk = FALSE;
		chunkFound = TRUE;
		return PushChunkFromDataSource(chunkId, data);
	}
	else return TRUE;
}
