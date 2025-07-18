#include "locale.h"
#include "chunkcopier.h"
#include "sysfile.h"

#include <proto/exec.h>
#include <proto/dos.h>

//=============================================================================
// ChunkDataFile::CopyData()
//=============================================================================

bool ChunkDataFile::CopyData(IFFWriter *dest, void *buffer)
{
	bool success = TRUE;
	int32 bytesread, error;

	while (success)
	{
		bytesread = source.SfRead(buffer, COPYBUF_SIZE);

		if (bytesread > 0)
		{
			error = WriteChunkBytes(dest->GetIff(), buffer, bytesread);
			if (error < 0) success = dest->IFFProblem(error);
		}
		else if (bytesread == 0) break;
		else success = source.SfProblem();
	}

	return success;
}

//=============================================================================
// ChunkDataString::ChunkDataString()
// Unescaped string is always shorter or the same as original, so it can be
// unescaped in-place safely.
//=============================================================================

ChunkDataString::ChunkDataString(char *string) : string((uint8*)string) 
{
	ready = FALSE;
	strsize = 0;
	if (Unescape()) ready = TRUE;
	else PutStr(Ls[MSG_BROKEN_ESCAPE_SEQUENCE]);
}

//=============================================================================
// ChunkDataString::CopyData()
//=============================================================================

bool ChunkDataString::CopyData(IFFWriter *dest, void *buffer)
{
	int32 error;

	error = WriteChunkBytes(dest->GetIff(), string, strsize);
	if (error < 0) return dest->IFFProblem(error);
	else return TRUE;
}

//=============================================================================
// ChunkDataString::Unescape()
//=============================================================================

#define UNESCAPE_WAIT_FOR_ESC_CHAR  1
#define UNESCAPE_FIRST_HEXDIGIT     2
#define UNESCAPE_SECOND_HEXDIGIT    3

static uint8 hex2num(uint8 c)
{
	if ((c >= '0') && (c <= '9')) return c - '0';
	else if ((c >= 'A') && (c <= 'F')) return c - 'A' + 10;
	else if ((c >= 'a') && (c <= 'f')) return c - 'a' + 10;
	else return 0xFF;
}

bool ChunkDataString::Unescape()
{
	bool success = TRUE;
	uint8 c, b, v, *source = string, *dest = string;
	int32 state = UNESCAPE_WAIT_FOR_ESC_CHAR;

	while (c = *source++)
	{
		switch (state)
		{
			case UNESCAPE_WAIT_FOR_ESC_CHAR:
				if (c == '%') state = UNESCAPE_FIRST_HEXDIGIT;
				else *dest++ = c;
			break;

			case UNESCAPE_FIRST_HEXDIGIT:
				b = hex2num(c);
				if (b == 0xFF) return FALSE;
				v = b << 4;
				state = UNESCAPE_SECOND_HEXDIGIT;
			break;

			case UNESCAPE_SECOND_HEXDIGIT:
				b = hex2num(c);
				if (b == 0xFF) return FALSE;
				v += b;
				*dest++ = v;
				state = UNESCAPE_WAIT_FOR_ESC_CHAR;
			break;
		}
	}

	*dest = 0x00;
	strsize = dest - string;

	return success;
}

//=============================================================================
// ChunkCopier::ChunkCopier()
//=============================================================================

ChunkCopier::ChunkCopier(const char *sourceName, const char *destName) :
	IFFReader(sourceName),
	destination(NULL),
	copyThisChunk(TRUE),
	chunkFound(FALSE),
	chunkId(0),
	copyBuffer(NULL)
{
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
				else Problem(Ls[MSG_OUT_OF_MEMORY]);
			}
		}
		else Problem(Ls[MSG_OUT_OF_MEMORY]);
	}
}

//=============================================================================================
// ChunkCopier::~ChunkCopier()
//=============================================================================================

ChunkCopier::~ChunkCopier()
{
	if (copyBuffer) FreeMem(copyBuffer, COPYBUF_SIZE);
	if (destination) delete destination;
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
		success = source->CopyData(destination, copyBuffer);
		error = PopChunk(destination->GetIff());

		if (success && (error < 0))
		{
			success = destination->IFFProblem(error);
		}
	}
	else destination->IFFProblem(error);

	return success;
}

//=============================================================================
// ChunkCopier::FormEndWork()
//=============================================================================

bool ChunkCopier::FormEndWork()
{
	char buf[6];

	if (!chunkFound)
	{
		Printf(Ls[MSG_CHUNK_NOT_FOUND_IN_SOURCE], IDtoStr(chunkId, buf));
	}

	return TRUE;
}
