#include "chunkadder.h"

//=============================================================================
// ChunkAdder::ChunkAdder()
//=============================================================================

ChunkAdder::ChunkAdder(const char *sourceName, const char *destName,
 const char *chunk, ChunkDataSource *data) : ChunkCopier(sourceName, destName),
 data(data)
{
	DC("ChunkAdder");

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
// ChunkAdder::FormEndWork()
//=============================================================================

bool ChunkAdder::FormEndWork()
{
	return PushChunkFromDataSource(chunkId, data);
}
