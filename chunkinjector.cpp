#include "chunkinjector.h"

#include <proto/dos.h>

//=============================================================================
// ChunkInjector::ChunkInjector()
//=============================================================================

ChunkInjector::ChunkInjector(const char *sourceName, const char *destName,
 const char *chunk, ChunkDataSource *data, const char *after) :
 ChunkCopier(sourceName, destName), chunkId(0), chunkAfter(0), found(FALSE),
 data(data)
{
	if (ready)
	{
		ready = FALSE;

		if (chunk)
		{
			chunkId = ValidateChunkID(chunk);

			if (chunkId)
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
		else PutStr(LS(MSG_CHUNK_ARGUMENT_REQUIRED, "CHUNK argument is "
		"required for this operation mode.\n"));
	}
}

//=============================================================================
// ChunkInjector::FormStartWork
//=============================================================================

bool ChunkInjector::FormStartWork()
{
	if (chunkAfter == 0)
	{
		found = TRUE;
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
		found = TRUE;
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

	if (!found)
	{
		Printf(LS(MSG_CHUNK_NOT_FOUND_IN_SOURCE, "Chunk '%s' not found.\n"),
		 IDtoStr(chunkAfter, buf));
		return FALSE;
	}
	else return TRUE;
}




