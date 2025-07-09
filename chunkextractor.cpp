#include "chunkextractor.h"

#include <proto/dos.h>
#include <proto/iffparse.h>

bool ChunkExtractor::ChunkWork(ContextNode *cn)
{
	bool success = FALSE;
	BPTR out;

	if (out = Open(outname, MODE_NEWFILE))
	{
		Printf("%s opened, handle = $%08lx\n", outname, out);
		if (!Close(out)) SysProblem(outname);
	}

	return success;
}