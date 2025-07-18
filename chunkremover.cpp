#include "chunkremover.h"

#include <proto/dos.h>

ChunkRemover::ChunkRemover(const char *sourceName, const char *destName,
 const char *chunk) : ChunkCopier(sourceName, destName)
{
	if (ready)
	{
		ready = FALSE;

		if (chunk)
		{
			if (chunkId = ValidateChunkID(chunk)) ready = TRUE;
		}
		else PutStr(LS(MSG_CHUNK_ARGUMENT_REQUIRED, "'CHUNK' argument "
		"required for this operation mode.\n"));
	}
}
