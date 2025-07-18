#include "chunkremover.h"

#include <proto/dos.h>

ChunkRemover::ChunkRemover(const char *sourceName, const char *destName,
 const char *chunk) : ChunkCopier(sourceName, destName)
{
	if (ready)
	{
		ready = FALSE;

		if (chunkId = ValidateChunkID(chunk)) ready = TRUE;
	}
}
