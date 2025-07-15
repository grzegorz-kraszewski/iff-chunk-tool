#include "chunkcopier.h"
#include "sysfile.h"

#include <proto/exec.h>
#include <proto/dos.h>

//=============================================================================
// ChunkDataFile::CopyData()
//=============================================================================

bool ChunkDataFile::CopyData(IFFWriter *dest)
{
	bool success = TRUE;
	int32 bytesread, error;
	void *copybuf;

	if (copybuf = AllocMem(COPYBUF_SIZE, MEMF_ANY))
	{
		while (success)
		{
			bytesread = source.SfRead(copybuf, COPYBUF_SIZE);

			if (bytesread > 0)
			{
				error = WriteChunkBytes(dest->GetIff(), copybuf, bytesread);
				if (error < 0) success = dest->IFFProblem(error);
			}
			else if (bytesread == 0) break;
			else success = source.SfProblem();
		}

		FreeMem(copybuf, COPYBUF_SIZE);
	}
	else success = Problem("out of memory");

	return success;
}

//=============================================================================
// ChunkDataString::CopyData()
//=============================================================================

bool ChunkDataString::CopyData(IFFWriter *dest)
{
	int32 error;

	error = WriteChunkBytes(dest->GetIff(), string, StrLen(string));
	if (error < 0) return dest->IFFProblem(error);
	else return TRUE;
}

//=============================================================================
// ChunkCopier::ChunkCopier()
//=============================================================================

ChunkCopier::ChunkCopier(const char *sourceName, const char *destName) :
 IFFReader(sourceName), destination(NULL), copyThisChunk(TRUE),
 copyBuffer(NULL)
{
	DC("ChunkCopier");

	if (ready)
	{
		ready = FALSE;

		if (destName)
		{
			destination = new IFFWriter(destName, iffType);

			if (destination && destination->ready)
			{
				copyBuffer = AllocMem(COPYBUF_SIZE, MEMF_ANY);
				if (copyBuffer) ready = TRUE;
				else Problem("Out of memory.");
			}
		}
		else Problem("TO argument required in this operation mode");
	}
}

//=============================================================================================
// ChunkCopier::~ChunkCopier()
//=============================================================================================

ChunkCopier::~ChunkCopier()
{
	if (copyBuffer) FreeMem(copyBuffer, COPYBUF_SIZE);
	if (destination) delete destination;
	DD("ChunkCopier");
}

//=============================================================================
// ChunkCopier::CopyChunk()
//-----------------------------------------------------------------------------
// Reader object is assumed to be positioned at start of chunk described by the
// ContextNode. Copy of this chunk is pushed to writer object. It is assumed
// that copyBuffer is allocated.
//=============================================================================

bool ChunkCopier::CopyChunk(ContextNode *cn)
{
	int32 iffError, bytesToCopy;
	bool success = TRUE;

	iffError = PushChunk(destination->GetIff(), cn->cn_Type, cn->cn_ID, cn->cn_Size);

	#warning Copy loop is badly defined

	if (iffError == 0)
	{
		bytesToCopy = cn->cn_Size;

		while (success && (bytesToCopy > 0))
		{
			int32 blocksize = bytesToCopy;

			if (blocksize > COPYBUF_SIZE) blocksize = COPYBUF_SIZE;
			iffError = ReadChunkBytes(iff, copyBuffer, blocksize);

			if (iffError >= 0)
			{
				iffError = WriteChunkBytes(destination->GetIff(), copyBuffer, blocksize);
				if (iffError >= 0) bytesToCopy -= blocksize;
				else success = destination->IFFProblem(iffError);
			}
			else success = IFFProblem(iffError);
		}

		iffError = PopChunk(destination->GetIff());

		//-------------------------------------------------------------------
		// If read or write failed (success == FALSE), successful PopChunk()
		// should not set 'success' to TRUE.
		//-------------------------------------------------------------------

		if (success && (iffError < 0)) success = destination->IFFProblem(iffError);
	}
	else success = destination->IFFProblem(iffError);

	return success;
}

//=============================================================================================
// ChunkCopier::Parse()
//=============================================================================================

bool ChunkCopier::Parse()
{
	ContextNode *cn;
	int32 iffError;
	bool success = TRUE, found = FALSE;

	//-----------------------------------------------------------------
	// IFFReader started parsing already, we are at start of FORM now.
	//-----------------------------------------------------------------

	success = FormStartWork();

	while (success)
	{
		iffError = ParseIFF(iff, IFFPARSE_STEP);
		cn = CurrentChunk(iff);

		if (iffError == 0)
		{
			if (success = PreChunkWork(cn))
			{
				if (copyThisChunk) success = CopyChunk(cn);
			}

			copyThisChunk = TRUE;
		}
		else if (iffError == IFFERR_EOC)
		{
			if (cn->cn_ID == ID_FORM)
			{
				success = FormEndWork();
				break;
			}
			else success = PostChunkWork(cn);
		}
		else success = IFFProblem(iffError);
	}

	return success;
}

//=============================================================================================
// ChunkCopier::FormStartWork()
//=============================================================================================

bool ChunkCopier::FormStartWork()
{
	Printf("FormStartWork()\n");
	return TRUE;
}

//=============================================================================================
// ChunkCopier::FormEndWork()
//=============================================================================================

bool ChunkCopier::FormEndWork()
{
	Printf("FormEndWork()\n");
	return TRUE;
}

//==============================================================================
// ChunkCopier::PreChunkWork()
//==============================================================================

bool ChunkCopier::PreChunkWork(ContextNode *cn)
{
	char b[6];

	Printf("PreChunkWork('%s')\n", IDtoStr(cn->cn_ID, b));
	return TRUE;
}

//=============================================================================
// ChunkCopier::PostChunkWork()
//=============================================================================

bool ChunkCopier::PostChunkWork(ContextNode *cn)
{
	char b[6];

	Printf("PostChunkWork('%s')\n", IDtoStr(cn->cn_ID, b));
	return TRUE;
}

//=============================================================================
// ChunkCopier::PushChunkFromDataSource()
//=============================================================================

bool ChunkCopier::PushChunkFromDataSource(uint32 chunkid, ChunkDataSource
 *source)
{
	bool success = FALSE;
	int32 error;

	if (!(error = PushChunk(destination->GetIff(), iffType, chunkid,
	 IFFSIZE_UNKNOWN)))
	{
		#warning ChunkCopier can provide its own copy buffer
		success = source->CopyData(destination);
		error = PopChunk(destination->GetIff());

		if (success && (error < 0))
		{
			success = destination->IFFProblem(error);
		}
	}
	else destination->IFFProblem(error);

	return success;
}
