#include "locale.h"
#include "chunkextractor.h"

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/iffparse.h>

//=============================================================================
// ChunkExtractor::ChunkExtractor()
//=============================================================================

ChunkExtractor::ChunkExtractor(const char *filepath, const char *chunkid,
 const char *destname) : ChunkPicker(filepath, chunkid)
{
	if (ready)
	{
		ready = FALSE;

		if (destname)
		{
			outname = destname;
			ready = TRUE;
		}
		else PutStr(Ls[MSG_DATA_ARGUMENT_REQUIRED]);
	}
}

//=============================================================================
// ChunkWork() for ChunkExtractor copies current chunk contents to a file in
// a loop using a fixed size buffer. Buffer size is defined in "main.h". This
// way even very large chunks may be processes without requiring large memory
// area.
//=============================================================================

bool ChunkExtractor::ChunkWork(ContextNode *cn)
{
	bool success = FALSE;

	if (copybuf = AllocMem(COPYBUF_SIZE, MEMF_ANY))
	{
		if (output = Open(outname, MODE_NEWFILE))
		{
			success = CopyLoop(cn);
			if (!Close(output)) SysProblem(outname);
		}
		else success = SysProblem(outname);

		FreeMem(copybuf, COPYBUF_SIZE);
	}
	else success = Problem(Ls[MSG_OUT_OF_MEMORY]);

	return success;
}

//=============================================================================
// Data copying loop.
//=============================================================================

bool ChunkExtractor::CopyLoop(ContextNode *cn)
{
	int32 blockSize, bytesLeft, bytesMoved;
	bool success = TRUE;

	bytesLeft = cn->cn_Size;

	while (success && (bytesLeft > 0))
	{
		blockSize = bytesLeft;
		if (blockSize > COPYBUF_SIZE) blockSize = COPYBUF_SIZE;
		bytesMoved = ReadChunkBytes(iff, copybuf, blockSize);

		if (bytesMoved >= 0)
		{
			bytesMoved = Write(output, copybuf, bytesMoved);

			if (bytesMoved >= 0)
			{
				bytesLeft -= bytesMoved;
			}
			else success = SysProblem(outname);
		}
		else success = IFFProblem(bytesMoved);
	}

	return success;
}
