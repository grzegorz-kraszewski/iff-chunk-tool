#include "chunkremover.h"
#include "locale.h"

#include <proto/dos.h>

//=============================================================================
// ChunkRemover::ChunkRemover()
//=============================================================================

ChunkRemover::ChunkRemover(const char *sourceName, const char *destName,
 const char *chunk) : ChunkCopier(sourceName, destName)
{
	if (ready)
	{
		ready = FALSE;

		if (chunkId = ValidateChunkID(chunk)) ready = TRUE;
	}
}

//=============================================================================
// ChunkRemover::PreChunkWork()
//=============================================================================

bool ChunkRemover::PreChunkWork(ContextNode *cn)
{
	if (cn->cn_ID == chunkId)
	{
		copyThisChunk = FALSE;
		chunkFound = TRUE;
	}
}
