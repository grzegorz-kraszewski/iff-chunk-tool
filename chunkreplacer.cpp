#include "chunkreplacer.h"

//=============================================================================
// ChunkReplacer::ChunkReplacer()
//=============================================================================

ChunkReplacer::ChunkReplacer(const char *sourceName, const char *destName,
 const char *chunk, ChunkDataSource *data) : ChunkCopier(sourceName, destName),
 data(data), found(FALSE)
{
	DC("ChunkReplacer");

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
		found = TRUE;
		return PushChunkFromDataSource(chunkId, data);
	}
	else return TRUE;
}

//=============================================================================
// ChunkReplacer::FormEndWork()
//=============================================================================

bool ChunkReplacer::FormEndWork()
{
	char buf[6];

	if (!found)
	{
		Printf("Chunk '%s' not found, replace failed.\n", IDtoStr(chunkId, buf));
		return FALSE;
	}
	else return TRUE;
}
