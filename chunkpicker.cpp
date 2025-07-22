#include "locale.h"
#include "chunkpicker.h"

#include <proto/dos.h>

//=============================================================================
// ChunkPicker::ChunkPicker()
//=============================================================================

ChunkPicker::ChunkPicker(const char *filepath, const char *chunkid) :
	IFFReader(filepath), chunkId(chunkid)
{
	if (ready)
	{
		if (!chunkId.valid()) ready = FALSE;
	}
}

//=============================================================================
// ChunkPicker::Parse()
//=============================================================================

bool ChunkPicker::Parse()
{
	bool success = TRUE;
	int32 error;
	int32 chunkCounter = 0;

	if (!(error = StopChunk(iff, iffType, chunkId.chunkId)))
	{
		while (success)
		{
			error = ParseIFF(iff, IFFPARSE_SCAN);

			if (!error)
			{
				if (chunkCounter++ == chunkId.number)
				{
					success = ChunkWork(CurrentChunk(iff));
					break;
				}
			}
			else if (error == IFFERR_EOF)
			{
				char idbuf[16];

				Printf(Ls[MSG_CHUNK_NOT_FOUND_IN_SOURCE],
				 chunkId.ExtIDtoStr(idbuf));

				success = FALSE;
			}
		}
	}
	else success = IFFProblem(error);

	return success;
}
