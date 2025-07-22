#include "chunkinjector.h"
#include "locale.h"

#include <proto/dos.h>

//=============================================================================
// ChunkInjector::ChunkInjector()
//=============================================================================

ChunkInjector::ChunkInjector(const char *sourceName, const char *destName,
const char *chunk, ChunkDataSource *data, const char *after) :
	ChunkCopier(sourceName, destName),
	chunkAfter(after),
	data(data),
	injectedChunk(0),
	chunkCounter(0)
{
	if (ready)
	{
		ready = FALSE;

		if (injectedChunk = ValidateChunkID(chunk))
		{
			if (data && data->ready)
			{
				if (after)
				{
					ready = chunkAfter.valid();
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
	if (!chunkAfter.valid())
	{
		chunkFound = TRUE;
		return PushChunkFromDataSource(injectedChunk, data);
	}
	else return TRUE;
}

//=============================================================================
// ChunkInjector::PostChunkWork()
//=============================================================================

bool ChunkInjector::PostChunkWork(ContextNode *cn)
{
	if ((chunkAfter.chunkId == cn->cn_ID) && (chunkAfter.number == chunkCounter++))
	{
		chunkFound = TRUE;
		return PushChunkFromDataSource(injectedChunk, data);
	}
	else return TRUE;
}

//=============================================================================
// ChunkInjector::FormEndWork()
//=============================================================================

bool ChunkInjector::FormEndWork()
{
	char buf[16];

	if (!chunkFound)
	{
		Printf(Ls[MSG_CHUNK_NOT_FOUND_IN_SOURCE], chunkAfter.ExtIDtoStr(buf));
		return FALSE;
	}
	else return TRUE;
}




