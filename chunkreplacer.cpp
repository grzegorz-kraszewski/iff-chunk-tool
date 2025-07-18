#include "chunkreplacer.h"
#include "locale.h"

//=============================================================================
// ChunkReplacer::ChunkReplacer()
//=============================================================================

ChunkReplacer::ChunkReplacer(const char *sourceName, const char *destName,
 const char *chunk, ChunkDataSource *data) : ChunkCopier(sourceName, destName),
 data(data), found(FALSE)
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
		Printf(LS(MSG_CHUNK_NOT_FOUND_IN_SOURCE, "Chunk '%s' not found.\n"),
		 IDtoStr(chunkId, buf));
		return FALSE;
	}
	else return TRUE;
}
