#include "chunkpicker.h"

#include <proto/dos.h>

bool ChunkPicker::Parse()
{
	bool result = FALSE;
	int32 error;

	if (!(error = StopChunk(iff, iffType, chunkId)))
	{
		error = ParseIFF(iff, IFFPARSE_SCAN);

		if (!error) result = ChunkWork(CurrentChunk(iff));
		else if (error == IFFERR_EOF)
		{
			char idbuf[6];

			Printf("Chunk '%s' not found.\n", IDtoStr(chunkId, idbuf));
		}
	}
	else result = IFFProblem(error);

	return result;
}
