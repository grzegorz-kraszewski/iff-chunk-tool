#include "chunkremover.h"
#include "locale.h"

#include <proto/dos.h>

//=============================================================================
// ChunkRemover::ChunkRemover()
//=============================================================================

ChunkRemover::ChunkRemover(const char *sourceName, const char *destName,
 const char *chunk) : ChunkCopier(sourceName, destName), id(chunk),
 chunkCounter(0) 
{
	if (ready)
	{
		ready = id.valid();
	}
}

//=============================================================================
// ChunkRemover::PreChunkWork()
//=============================================================================

bool ChunkRemover::PreChunkWork(ContextNode *cn)
{
	if ((id.chunkId == cn->cn_ID) && (id.number == chunkCounter++)) 
	{
		copyThisChunk = FALSE;
		chunkFound = TRUE;
	}

	return TRUE;
}

//=============================================================================
// ChunkRemover::AfterFormWork()
//=============================================================================

bool ChunkRemover::FormEndWork()
{
	char buf[16];

	if (!chunkFound) Printf(Ls[MSG_CHUNK_NOT_FOUND_IN_SOURCE],
		id.ExtIDtoStr(buf));
	return TRUE;
}
