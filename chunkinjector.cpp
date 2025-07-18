#include "chunkinjector.h"
#include "locale.h"

#include <proto/dos.h>

//=============================================================================
// ChunkInjector::ChunkInjector()
//=============================================================================

ChunkInjector::ChunkInjector(const char *sourceName, const char *destName,
const char *chunk, ChunkDataSource *data, const char *after) :
	ChunkCopier(sourceName, destName),
	chunkAfter(0),
	data(data)
{
	if (ready)
	{
		ready = FALSE;

		if (chunkId = ValidateChunkID(chunk))
		{
			if (data && data->ready)
			{
				if (after)
				{
					chunkAfter = ValidateChunkID(after);
					if (chunkAfter) ready = TRUE;
				}
				else ready = TRUE;
			}
		}
	}
}

//=============================================================================
// ChunkInjector::FormStartWork
//=============================================================================

bool ChunkInjector::FormStartWork()
{
	if (chunkAfter == 0)
	{
		chunkFound = TRUE;
		return PushChunkFromDataSource(chunkId, data);
	}
	else return TRUE;
}

//=============================================================================
// ChunkInjector::PostChunkWork()
//=============================================================================

bool ChunkInjector::PostChunkWork(ContextNode *cn)
{
	if (chunkAfter == cn->cn_ID)
	{
		chunkFound = TRUE;
		return PushChunkFromDataSource(chunkId, data);
	}
	else return TRUE;
}

//=============================================================================
// ChunkInjector::FormEndWork()
//=============================================================================

bool ChunkInjector::FormEndWork()
{
	char buf[6];

	if (!chunkFound)
	{
		Printf(Ls[MSG_CHUNK_NOT_FOUND_IN_SOURCE], IDtoStr(chunkAfter, buf));
		return FALSE;
	}
	else return TRUE;
}




