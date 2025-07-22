#include "chunkreplacer.h"
#include "locale.h"

//=============================================================================
// ChunkReplacer::ChunkReplacer()
//=============================================================================

ChunkReplacer::ChunkReplacer(const char *sourceName, const char *destName,
 const char *chunk, ChunkDataSource *data) : ChunkCopier(sourceName, destName),
 data(data), id(chunk), chunkCounter(0)
 {
	if (ready)
	{
		ready = FALSE;

		if (data && data->ready)
		{
			ready = id.valid();
		}
	}
}

//=============================================================================
// ChunkReplacer::PreChunkWork()
//=============================================================================

bool ChunkReplacer::PreChunkWork(ContextNode *cn)
{
	if ((id.chunkId == cn->cn_ID) && (id.number == chunkCounter++))
	{
		copyThisChunk = FALSE;
		chunkFound = TRUE;
		return PushChunkFromDataSource(id.chunkId, data);
	}
	else return TRUE;
}

//=============================================================================
// ChunkReplacer::FormEndWork()
//=============================================================================

bool ChunkReplacer::FormEndWork()
{
	char buf[16];

	if (!chunkFound) Printf(Ls[MSG_CHUNK_NOT_FOUND_IN_SOURCE],
		id.ExtIDtoStr(buf));
	return TRUE;
}

